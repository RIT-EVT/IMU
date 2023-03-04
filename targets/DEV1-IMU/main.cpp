/**
* This is a basic sample of using CAN with the IMU.
*/

#include <Canopen/co_core.h>
#include <Canopen/co_tmr.h>
#include <EVT/dev/Timer.hpp>
#include <EVT/io/CANopen.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/time.hpp>

#include <EVT/dev/MCUTimer.hpp>
#include <IMU.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;
namespace DEV = EVT::core::DEV;

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////

/**
* Interrupt handler to get CAN messages. A function pointer to this function
* will be passed to the EVT-core CAN interface which will in turn call this
* function each time a new CAN message comes in.
*
* NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
* assumed to be intended to be passed as a CANopen message.
*
* @param message[in] The passed in CAN message that was read.
*/

// create a can interrupt handler
void canInterrupt(IO::CANMessage& message, void* priv) {
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
        (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;

    // Log raw received data
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());

    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "%X ", *data);
        data++;
    }

    if (queue != nullptr)
        queue->append(message);
}

///////////////////////////////////////////////////////////////////////////////
// CANopen specific Callbacks. Need to be defined in some location
///////////////////////////////////////////////////////////////////////////////
extern "C" void CONodeFatalError(void) {}

extern "C" void COIfCanReceive(CO_IF_FRM* frm) {}

extern "C" int16_t COLssStore(uint32_t baudrate, uint8_t nodeId) { return 0; }

extern "C" int16_t COLssLoad(uint32_t* baudrate, uint8_t* nodeId) { return 0; }

extern "C" void CONmtModeChange(CO_NMT* nmt, CO_MODE mode) {}

extern "C" void CONmtHbConsEvent(CO_NMT* nmt, uint8_t nodeId) {}

extern "C" void CONmtHbConsChange(CO_NMT* nmt, uint8_t nodeId, CO_MODE mode) {}

extern "C" int16_t COParaDefault(CO_PARA* pg) { return 0; }

extern "C" void COPdoTransmit(CO_IF_FRM* frm) {}

extern "C" int16_t COPdoReceive(CO_IF_FRM* frm) { return 0; }

extern "C" void COPdoSyncUpdate(CO_RPDO* pdo) {}

extern "C" void COTmrLock(void) {}

extern "C" void COTmrUnlock(void) {}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer1>(100);
    timer.startTimer();

    // Setup i2c
    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();

    IMU::IMU imu(i2c);

    /**
     * Initialize the IMU
     */
//    imu.setup();
    uint8_t id = 0;
    i2c.write(0x28, &id, 0);
    i2c.write(0x28, 0x00);
    i2c.read(0x28, &id);

    uint8_t configBytes[2] = {0x3d, 0x00};
    i2c.write(0x28, configBytes, 2);
    EVT::core::time::wait(30);

    uint8_t resetBytes[2] = {0x3F, 0x20};
    i2c.write(0x28, resetBytes, 2);
    time::wait(30);

    do {
        time::wait(10);
    } while(i2c.write(0x28, 0x00) != IO::I2C::I2CStatus::OK);

    id = 0;
    i2c.write(0x28, 0x00);
    i2c.read(0x28, &id);

    time::wait(50);

    uint8_t powerModeBytes[2] = {0x3E, 0x00};
    i2c.write(0x28, powerModeBytes, 2);
    time::wait(10);

    uint8_t pageBytes[2] = {0x07, 0x00};
    i2c.write(0x28, pageBytes, 2);

    uint8_t systemResetResetBytes[2] = {0x3F, 0x00};
    i2c.write(0x28, systemResetResetBytes, 2);

    time::wait(10);

    uint8_t config2Bytes[2] = {0x3D, 0x0C};
    i2c.write(0x28, config2Bytes, 2);
    time::wait(20);

    uint8_t mode;
    i2c.readReg(0x28, 0x3D, &mode);

    /**
    * Initialize CAN
    */
    //Will store CANopen messages that will be populated by the EVT-core CAN interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    // Check to see if the device is connected to the CAN network
    if (result != IO::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to CAN network\r\n");
        return 1;
    } else {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to CAN network\r\n");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////
    // Make drivers
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    IO::getCANopenCANDriver(&can, &canOpenQueue, &canDriver);
    IO::getCANopenTimerDriver(&timer, &timerDriver);
    IO::getCANopenNVMDriver(&nvmDriver);

    canStackDriver.Can = &canDriver;
    canStackDriver.Timer = &timerDriver;
    canStackDriver.Nvm = &nvmDriver;

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[1][CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[4];

    //setup CANopen Node
    CO_NODE_SPEC canSpec = {
        .NodeId = IMU::IMU::NODE_ID,
        .Baudrate = IO::CAN::DEFAULT_BAUD,
        .Dict = imu.getObjectDictionary(),
        .DictLen = imu.getObjectDictionarySize(),
        .EmcyCode = NULL,
        .TmrMem = appTmrMem,
        .TmrNum = 16,
        .TmrFreq = 100,
        .Drv = &canStackDriver,
        .SdoBuf = reinterpret_cast<uint8_t*>(&sdoBuffer[0]),
    };

    CO_NODE canNode;

    CONodeInit(&canNode, &canSpec);
    CONodeStart(&canNode);
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    while (1) {
//        imu.loop();
        uint16_t gyro[3] = { 0, 0 ,0};
        uint8_t buffer[6] = { 0, 0, 0, 0, 0, 0};

        i2c.write(0x28, 0X2E);
        i2c.read(0x28, buffer, 6);

        gyro[0] = buffer[0] | (buffer[1] << 8);
        gyro[1] = buffer[2] | (buffer[3] << 8);
        gyro[2] = buffer[4] | (buffer[5] << 8);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw x: %d", (int16_t) gyro[0]);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw y: %d", (int16_t) gyro[1]);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw z: %d", (int16_t) gyro[2]);


        CONodeProcess(&canNode);
        // Update the state of timer based events
        COTmrService(&canNode.Tmr);
        // Handle executing timer events that have elapsed
        COTmrProcess(&canNode.Tmr);
        // Wait for new data to come in
        time::wait(1000);
    }
}

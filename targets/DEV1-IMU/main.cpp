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

    // We do not need to call bno055.setup() because it is called in the IMU class initializer.
    IMU::BNO055 bno055(i2c);
    IMU::IMU imu(bno055);

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
        imu.process();

        CONodeProcess(&canNode);
        // Update the state of timer based events
        COTmrService(&canNode.Tmr);
        // Handle executing timer events that have elapsed
        COTmrProcess(&canNode.Tmr);
        // Wait for new data to come in
        time::wait(1000);
    }
}

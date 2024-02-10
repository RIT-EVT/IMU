/**
* This is a basic sample of using CAN with the IMU.
*/

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
    auto* queue = (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;

    //print out raw received data
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());
    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "%X ", *data);
        data++;
    }

    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Set up the logger with a UART, logLevel, and clock
    // If timestamps aren't needed, don't set the logger's clock
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::INFO);
    DEV::RTC& rtc = DEV::getRTC();
    log::LOGGER.setClock(&rtc);

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer1>(100);
    timer.startTimer();

    // Setup i2c
    // i2c pins for the breakout board
    //    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    // i2c pins for the final board
    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_6, IO::Pin::PB_7>();

    // We do not need to call bno055.setup() because it is called in the IMU class initializer.
    IMU::BNO055 bno055(i2c, 0x28);
    IMU::IMU imu(bno055);

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////
    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Reserve driver variables
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    CO_NODE canNode;

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    //test that the board is connected to the can network
    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    IO::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    IO::initializeCANopenNode(&canNode, &imu, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    //print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));
    while (1) {
        imu.process();

        IO::processCANopenNode(&canNode);
    }
}

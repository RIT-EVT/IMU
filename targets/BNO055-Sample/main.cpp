#include <EVT/dev/Timer.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>

#include <BNO055.hpp>
#include <EVT/dev/MCUTimer.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;
namespace DEV = EVT::core::DEV;

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
    // i2c pins for the breakout board
//    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    // i2c pins for the final board
    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_6, IO::Pin::PB_7>();

    IMU::BNO055 bno055(i2c, 0x28);
    // The bno055 has a lengthy boot sequence, so it needs a setup function to be called.
    bno055.setup();

    uart.printf("Starting BNO055 Testing...");
    while (1) {
        // Retrieve the Euler X, Y and Z values from the bno055
        int16_t eulerX;
        int16_t eulerY;
        int16_t eulerZ;

        bno055.getEuler(eulerX, eulerY, eulerZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler x: %d.%d", eulerX / 16, (uint16_t) eulerX % 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler y: %d.%d", eulerY / 16, (uint16_t) eulerY % 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler z: %d.%d", eulerZ / 16, (uint16_t) eulerZ % 16);

        // Retrieve the Gyroscope X, Y, and Z values from the bno055
        int16_t gyroX;
        int16_t gyroY;
        int16_t gyroZ;

        bno055.getGyroscope(gyroX, gyroY, gyroZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope x: %d.%d", gyroX / 16, (uint16_t) gyroX % 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope y: %d.%d", gyroY / 16, (uint16_t) gyroY % 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope z: %d.%d", gyroZ / 16, (uint16_t) gyroZ % 16);

        // Retrieve the Linear Acceleration X, Y, and Z values from the bno055
        int16_t linearAccelX;
        int16_t linearAccelY;
        int16_t linearAccelZ;

        bno055.getLinearAccel(linearAccelX, linearAccelY, linearAccelZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration x: %d.%d", linearAccelX / 100, (uint16_t) linearAccelX % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration y: %d.%d", linearAccelY / 100, (uint16_t) linearAccelY % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration z: %d.%d", linearAccelZ / 100, (uint16_t) linearAccelZ % 100);

        // Retrieve the Accelerometer X, Y, and Z values from the bno055
        int16_t accelerometerX;
        int16_t accelerometerY;
        int16_t accelerometerZ;

        bno055.getAccelerometer(accelerometerX, accelerometerY, accelerometerZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer x: %d.%d", accelerometerX / 100, (uint16_t) accelerometerX % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer y: %d.%d", accelerometerY / 100, (uint16_t) accelerometerY % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer z: %d.%d", accelerometerZ / 100, (uint16_t) accelerometerZ % 100);

        // Retrieve the Gravity X, Y, and Z values from the bno055
        int16_t gravityX;
        int16_t gravityY;
        int16_t gravityZ;

        bno055.getGravity(gravityX, gravityY, gravityZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity x: %d.%d", accelerometerX / 100, (uint16_t) accelerometerX % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity y: %d.%d", accelerometerY / 100, (uint16_t) accelerometerY % 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity z: %d.%d", accelerometerZ / 100, (uint16_t) accelerometerZ % 100);

        EVT::core::time::wait(500);
    }
}

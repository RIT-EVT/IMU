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
    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();

    IMU::BNO055 bno055(i2c);
    // The bno055 has a lengthy boot sequence
    bno055.setup();

    while (1) {
        // Retrieve the Euler X, Y and Z values from the bno055
        int16_t eulerX, eulerY, eulerZ;

        bno055.getEuler(eulerX, eulerY, eulerZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw x: %d", (int16_t) eulerX / 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw y: %d", (int16_t) eulerY / 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw z: %d", (int16_t) eulerZ / 16);

        // Retrieve the Gyroscope X, Y, and Z values from the bno055
        int16_t gyroX, gyroY, gyroZ;

        bno055.getGyroscope(gyroX, gyroY, gyroZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw x: %d", (int16_t) gyroX / 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw y: %d", (int16_t) gyroY / 16);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw z: %d", (int16_t) gyroZ / 16);

        // Retrieve the Linear Acceleration X, Y, and Z values from the bno055
        int16_t linearAccelX, linearAccelY, linearAccelZ;

        bno055.getLinearAccel(linearAccelX, linearAccelY, linearAccelZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw x: %d", (int16_t) linearAccelX / 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw y: %d", (int16_t) linearAccelY / 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw z: %d", (int16_t) linearAccelZ / 100);

        // Retrieve the Accelerometer X, Y, and Z values from the bno055
        int16_t accelerometerX, accelerometerY, accelerometerZ;

        bno055.getLinearAccel(linearAccelX, linearAccelY, linearAccelZ);

        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw x: %d", (int16_t) accelerometerX / 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw y: %d", (int16_t) accelerometerY / 100);
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw z: %d", (int16_t) accelerometerZ / 100);
    }
}

#include <IMU.hpp>

namespace IO = EVT::core::IO;

namespace IMU {
IMU::IMU(IO::I2C& i2c) : bno055(i2c) {
    bno055.setup();
}

CO_OBJ_T* IMU::getObjectDictionary() {
    return objectDictionary;
}
void IMU::cycle() {
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw x: %d", (int16_t) euler[0] / 16);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw y: %d", (int16_t) euler[1] / 16);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw z: %d", (int16_t) euler[2] / 16);

//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw x: %d", (int16_t) gyro[0] / 16);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw y: %d", (int16_t) gyro[1] / 16);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw z: %d", (int16_t) gyro[2] / 16);

//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw x: %d", (int16_t) linearAccel[0] / 100);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw y: %d", (int16_t) linearAccel[1] / 100);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw z: %d", (int16_t) linearAccel[2] / 100);

//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer x: %d", (int16_t) accelerometer[0] / 100);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer y: %d", (int16_t) accelerometer[1] / 100);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer z: %d", (int16_t) accelerometer[2] / 100);
}
}// namespace IMU

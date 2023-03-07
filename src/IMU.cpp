#include <IMU.hpp>

namespace IO = EVT::core::IO;

namespace IMU {

IMU::IMU(BNO055& bno055) : bno055(bno055) {
    // Set up the BNO055 device.
    bno055.setup();
}

CO_OBJ_T* IMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t IMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

void IMU::process() {
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

}// namespace IMU

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
    bno055.getEuler(vectorXValues[0], vectorYValues[0], vectorZValues[0]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw x: %d.%d", vectorXValues[0] / 16, (uint16_t) vectorXValues[0] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw y: %d.%d", vectorYValues[0] / 16, (uint16_t) vectorYValues[0] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw z: %d.%d", vectorZValues[0] / 16, (uint16_t) vectorZValues[0] % 16);

    // Retrieve the Gyroscope X, Y, and Z values from the bno055
    bno055.getGyroscope(vectorXValues[1], vectorYValues[1], vectorZValues[1]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw x: %d.%d", vectorXValues[1] / 16, (uint16_t) vectorXValues[1] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw y: %d.%d", vectorYValues[1] / 16, (uint16_t) vectorYValues[1] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw z: %d.%d", vectorZValues[1] / 16, (uint16_t) vectorZValues[1] % 16);

    // Retrieve the Linear Acceleration X, Y, and Z values from the bno055
    bno055.getLinearAccel(vectorXValues[2], vectorYValues[2], vectorZValues[2]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw x: %d.%d", vectorXValues[2] / 100, (uint16_t) vectorXValues[2] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw y: %d.%d", vectorYValues[2] / 100, (uint16_t) vectorYValues[2] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw z: %d.%d", vectorZValues[2] / 100, (uint16_t) vectorZValues[2] % 100);

    // Retrieve the Accelerometer X, Y, and Z values from the bno055
    bno055.getAccelerometer(vectorXValues[3], vectorYValues[3], vectorZValues[3]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw x: %d.%d", vectorXValues[3] / 100, (uint16_t) vectorXValues[3] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw y: %d.%d", vectorYValues[3] / 100, (uint16_t) vectorYValues[3] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer Raw z: %d.%d", vectorZValues[3] / 100, (uint16_t) vectorZValues[3] % 100);

    // Retrieve the gravity X, Y, and Z values from the bno055
    bno055.getAccelerometer(vectorXValues[4], vectorYValues[4], vectorZValues[4]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw x: %d.%d", vectorXValues[4] / 100, (uint16_t) vectorXValues[4] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw y: %d.%d", vectorYValues[4] / 100, (uint16_t) vectorYValues[4] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gravity Raw z: %d.%d", vectorZValues[4] / 100, (uint16_t) vectorZValues[4] % 100);
}

}// namespace IMU

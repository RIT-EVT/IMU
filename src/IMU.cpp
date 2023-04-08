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

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler x: %d.%d", vectorXValues[0] / 16, (uint16_t) vectorXValues[0] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler y: %d.%d", vectorYValues[0] / 16, (uint16_t) vectorYValues[0] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler z: %d.%d", vectorZValues[0] / 16, (uint16_t) vectorZValues[0] % 16);

    // Retrieve the Gyroscope X, Y, and Z values from the bno055
    bno055.getGyroscope(vectorXValues[1], vectorYValues[1], vectorZValues[1]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope x: %d.%d", vectorXValues[1] / 16, (uint16_t) vectorXValues[1] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope y: %d.%d", vectorYValues[1] / 16, (uint16_t) vectorYValues[1] % 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope z: %d.%d", vectorZValues[1] / 16, (uint16_t) vectorZValues[1] % 16);

    // Retrieve the Linear Acceleration X, Y, and Z values from the bno055
    bno055.getLinearAccel(vectorXValues[2], vectorYValues[2], vectorZValues[2]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration x: %d.%d", vectorXValues[2] / 100, (uint16_t) vectorXValues[2] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration y: %d.%d", vectorYValues[2] / 100, (uint16_t) vectorYValues[2] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration z: %d.%d", vectorZValues[2] / 100, (uint16_t) vectorZValues[2] % 100);

    // Retrieve the Accelerometer X, Y, and Z values from the bno055
    bno055.getAccelerometer(vectorXValues[3], vectorYValues[3], vectorZValues[3]);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer x: %d.%d", vectorXValues[3] / 100, (uint16_t) vectorXValues[3] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer y: %d.%d", vectorYValues[3] / 100, (uint16_t) vectorYValues[3] % 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer z: %d.%d", vectorZValues[3] / 100, (uint16_t) vectorZValues[3] % 100);}

}// namespace IMU

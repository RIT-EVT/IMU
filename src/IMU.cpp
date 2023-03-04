#include <EVT/utils/log.hpp>
#include <IMU.hpp>

namespace log = EVT::core::log;
namespace IO = EVT::core::IO;

namespace IMU {
IMU::IMU(IO::I2C& i2C) : i2c(i2C) { }

void IMU::setMode(uint8_t mode) {
    _mode = mode;
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Trying to set mode: 0x%X\r\n", _mode);
    IO::I2C::I2CStatus status = i2c.writeReg(I2C_SLAVE_ADDR, BNO055_OPR_MODE_ADDR, _mode);

    if (status != IO::I2C::I2CStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "i2c Error: %d\r\n", status);
    }

    time::wait(30);

    uint8_t readMode;
    i2c.readReg(I2C_SLAVE_ADDR, BNO055_OPR_MODE_ADDR, &readMode);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Read Mode: 0x%X\r\n", readMode);
}

CO_OBJ_T* IMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t IMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

bool IMU::setup() {
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting Initialization...\r\n");

    uint8_t id = 0;
    i2c.write(I2C_SLAVE_ADDR, &id, 0);
    i2c.write(I2C_SLAVE_ADDR, 0x00);
    i2c.read(I2C_SLAVE_ADDR, &id);

    uint8_t configBytes[2] = {BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG};
    i2c.write(I2C_SLAVE_ADDR, configBytes, 2);
    EVT::core::time::wait(30);

    uint8_t resetBytes[2] = {BNO055_SYS_TRIGGER_ADDR, 0x20};
    i2c.write(I2C_SLAVE_ADDR, resetBytes, 2);
    time::wait(30);

    do {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Was not detected, try again...\r\n");
        time::wait(10);
    } while(i2c.write(I2C_SLAVE_ADDR, 0x00) != IO::I2C::I2CStatus::OK);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be booted now... Checking if we can read...\r\n");

    id = 0;
    i2c.write(I2C_SLAVE_ADDR, 0x00);
    i2c.read(I2C_SLAVE_ADDR, &id);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "ID Read 0x%x\r\n", id);
    if (id != BNO055_ID) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed first initialization... Trying again.\r\n");

        time::wait(1000); // Hold on for boot
        i2c.write(I2C_SLAVE_ADDR, 0x00);
        i2c.read(I2C_SLAVE_ADDR, &id);

        if (id != BNO055_ID) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initialize the IMU. Quitting initialization\r\n");
            return false;
        }
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to i2c!\r\n");

    time::wait(50);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Resetting device...\r\n");
    uint8_t powerModeBytes[2] = {BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL};
    i2c.write(I2C_SLAVE_ADDR, powerModeBytes, 2);
    time::wait(10);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device Reset.\r\n");

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Reset device page.\r\n");
    uint8_t pageBytes[2] = {BNO055_PAGE_ID_ADDR, 0x00};
    i2c.write(I2C_SLAVE_ADDR, pageBytes, 2);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Reset the system reset.\r\n");
    uint8_t systemResetResetBytes[2] = {BNO055_SYS_TRIGGER_ADDR, 0x00};
    i2c.write(I2C_SLAVE_ADDR, systemResetResetBytes, 2);

    time::wait(10);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Set config mode to all data.\r\n");
    uint8_t config2Bytes[2] = {BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF};
    i2c.write(I2C_SLAVE_ADDR, config2Bytes, 2);
    time::wait(20);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "System successfully booted!\r\n");
    return true;
}

void IMU::loop() {
    /** Get the Euler data */
    uint16_t euler[3] = { 0, 0 ,0};
    uint8_t eulerBuffer[6] = { 0, 0, 0, 0, 0, 0};

    i2c.write(0x28, BNO055_EULER_H_LSB_ADDR);
    i2c.read(0x28, eulerBuffer, 6);

    euler[0] = eulerBuffer[0] | (eulerBuffer[1] << 8);
    euler[1] = eulerBuffer[2] | (eulerBuffer[3] << 8);
    euler[2] = eulerBuffer[4] | (eulerBuffer[5] << 8);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw x: %d", (int16_t) euler[0] / 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw y: %d", (int16_t) euler[1] / 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Euler Raw z: %d", (int16_t) euler[2] / 16);

    /** Get the Gyroscope data */
    uint16_t gyro[3] = { 0, 0 ,0};
    uint8_t gyroBuffer[6] = { 0, 0, 0, 0, 0, 0};

    i2c.write(0x28, BNO055_GYRO_DATA_X_LSB_ADDR);
    i2c.read(0x28, gyroBuffer, 6);

    gyro[0] = gyroBuffer[0] | (gyroBuffer[1] << 8);
    gyro[1] = gyroBuffer[2] | (gyroBuffer[3] << 8);
    gyro[2] = gyroBuffer[4] | (gyroBuffer[5] << 8);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw x: %d", (int16_t) gyro[0] / 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw y: %d", (int16_t) gyro[1] / 16);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw z: %d", (int16_t) gyro[2] / 16);

    /** Get the Linear Acceleration data */
    uint16_t linearAccel[3] = { 0, 0 ,0};
    uint8_t linearAccelBuffer[6] = { 0, 0, 0, 0, 0, 0};

    i2c.write(0x28, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR);
    i2c.read(0x28, linearAccelBuffer, 6);

    linearAccel[0] = linearAccelBuffer[0] | (linearAccelBuffer[1] << 8);
    linearAccel[1] = linearAccelBuffer[2] | (linearAccelBuffer[3] << 8);
    linearAccel[2] = linearAccelBuffer[4] | (linearAccelBuffer[5] << 8);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw x: %d", (int16_t) linearAccel[0] / 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw y: %d", (int16_t) linearAccel[1] / 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Linear Acceleration Raw z: %d", (int16_t) linearAccel[2] / 100);

    /** Get the Accelerometer data */
    uint16_t accelerometer[3] = { 0, 0 ,0};
    uint8_t accelerometerBuffer[6] = { 0, 0, 0, 0, 0, 0};

    i2c.write(0x28, BNO055_ACCEL_DATA_X_LSB_ADDR);
    i2c.read(0x28, accelerometerBuffer, 6);

    accelerometer[0] = accelerometerBuffer[0] | (accelerometerBuffer[1] << 8);
    accelerometer[1] = accelerometerBuffer[2] | (accelerometerBuffer[3] << 8);
    accelerometer[2] = accelerometerBuffer[4] | (accelerometerBuffer[5] << 8);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer x: %d", (int16_t) accelerometer[0] / 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer y: %d", (int16_t) accelerometer[1] / 100);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Accelerometer z: %d", (int16_t) accelerometer[2] / 100);
}
}// namespace IMU

#include <BNO055.hpp>

IMU::BNO055::BNO055(IO::I2C& i2C): i2c(i2C) { }

bool IMU::BNO055::setup() {
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

IO::I2C::I2CStatus IMU::BNO055::getEuler(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    uint8_t eulerBuffer[6] = { 0, 0, 0, 0, 0, 0};

    IO::I2C::I2CStatus writeStatus = i2c.write(0x28, BNO055_EULER_H_LSB_ADDR);
    if (writeStatus != IO::I2C::I2CStatus::OK) {
        return writeStatus;
    }

    IO::I2C::I2CStatus readStatus = i2c.read(0x28, eulerBuffer, 6);
    if (readStatus != IO::I2C::I2CStatus::OK) {
        return readStatus;
    }

    xBuffer = eulerBuffer[0] | (eulerBuffer[1] << 8);
    yBuffer = eulerBuffer[2] | (eulerBuffer[3] << 8);
    zBuffer = eulerBuffer[4] | (eulerBuffer[5] << 8);

    return readStatus;
}
IO::I2C::I2CStatus IMU::BNO055::getGyroscope(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    uint8_t gyroscopeBuffer[6] = { 0, 0, 0, 0, 0, 0};

    IO::I2C::I2CStatus writeStatus = i2c.write(0x28, BNO055_GYRO_DATA_X_LSB_ADDR);
    if (writeStatus != IO::I2C::I2CStatus::OK) {
        return writeStatus;
    }

    IO::I2C::I2CStatus readStatus = i2c.read(0x28, gyroscopeBuffer, 6);
    if (readStatus != IO::I2C::I2CStatus::OK) {
        return readStatus;
    }

    xBuffer = gyroscopeBuffer[0] | (gyroscopeBuffer[1] << 8);
    yBuffer = gyroscopeBuffer[2] | (gyroscopeBuffer[3] << 8);
    zBuffer = gyroscopeBuffer[4] | (gyroscopeBuffer[5] << 8);

    return readStatus;
}
IO::I2C::I2CStatus IMU::BNO055::getLinearAccel(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    uint8_t linearAccelBuffer[6] = { 0, 0, 0, 0, 0, 0};

    IO::I2C::I2CStatus writeStatus = i2c.write(0x28, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR);
    if (writeStatus != IO::I2C::I2CStatus::OK) {
        return writeStatus;
    }

    IO::I2C::I2CStatus readStatus = i2c.read(0x28, linearAccelBuffer, 6);
    if (readStatus != IO::I2C::I2CStatus::OK) {
        return readStatus;
    }

    xBuffer = linearAccelBuffer[0] | (linearAccelBuffer[1] << 8);
    yBuffer = linearAccelBuffer[2] | (linearAccelBuffer[3] << 8);
    zBuffer = linearAccelBuffer[4] | (linearAccelBuffer[5] << 8);

    return readStatus;
}

IO::I2C::I2CStatus IMU::BNO055::getAccelerometer(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    uint8_t accelerometerBuffer[6] = { 0, 0, 0, 0, 0, 0};

    IO::I2C::I2CStatus writeStatus = i2c.write(0x28, BNO055_ACCEL_DATA_X_LSB_ADDR);
    if (writeStatus != IO::I2C::I2CStatus::OK) {
        return writeStatus;
    }

    IO::I2C::I2CStatus readStatus = i2c.read(0x28, accelerometerBuffer, 6);
    if (readStatus != IO::I2C::I2CStatus::OK) {
        return readStatus;
    }

    xBuffer = accelerometerBuffer[0] | (accelerometerBuffer[1] << 8);
    yBuffer = accelerometerBuffer[2] | (accelerometerBuffer[3] << 8);
    zBuffer = accelerometerBuffer[4] | (accelerometerBuffer[5] << 8);

    return readStatus;
}

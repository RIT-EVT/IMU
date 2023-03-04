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
    // The board can take up to 850 ms to boot.

    int timeout = 850; // in ms
    while (timeout > 0) {
        uint8_t hold;
        IO::I2C::I2CStatus status = i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &hold);

        if (status == IO::I2C::I2CStatus::OK) {
            log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to i2c!\r\n");
            break;
        }

        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Was not detected, try again...\r\n");
        time::wait(10);
        timeout -= 10;
    }

    if (timeout <= 0) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to IMU over can.\r\n");
        return false;
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be booted now... Checking if we can read...\r\n");

    uint8_t id;
    i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &id);
    if (id != BNO055_ID) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed first initialization... Trying again.\r\n");

        time::wait(1000);// hold on for boot
        i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &id);
        if (id != BNO055_ID) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initialize the IMU\r\n");
            return false;
        }
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device booted!\r\n");

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Putting IMU into Configuration Mode.\r\n");

    // Put the device into configuration mode
    setMode(OPERATION_MODE_CONFIG);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be in config mode, resetting system now...\r\n");

    // Reset the system
    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_SYS_TRIGGER_ADDR, 0x20);
    time::wait(30);

    // Wait for the system boot again 🤦
    id = 0x00;
    IO::I2C::I2CStatus status;

    do {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Checking system boot status...\r\n");
        status = i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &id);
        time::wait(10);
    }
    while(id != BNO055_ID && status == IO::I2C::I2CStatus::OK);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "System booted.\r\n");

    time::wait(50);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Setting the default power mode.\r\n");
    // Set the default power mode
    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_PWR_MODE_ADDR, bno055PowerMode::POWER_MODE_NORMAL);
    time::wait(10);

    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_PAGE_ID_ADDR, 0x00);

    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_SYS_TRIGGER_ADDR, 0x00);
    time::wait(10);

    /* Set the requested operating mode (see section 3.3) */
    setMode(OPERATION_MODE_NDOF);

    time::wait(20);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "IMU Successfully (hopefully) Initialized.\r\n");
    return true;
}

void IMU::loop() {
    uint8_t oprMode;
    i2c.readReg(I2C_SLAVE_ADDR, BNO055_OPR_MODE_ADDR, &oprMode);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Mode: %X", oprMode);

//    uint8_t gyro[3] = { 0, 0 ,0};
//    uint8_t buffer[6] = { 0, 0, 0, 0, 0, 0};
//
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_X_LSB_ADDR, &buffer[0]);
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_X_MSB_ADDR, &buffer[1]);
//
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_Y_LSB_ADDR, &buffer[2]);
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_Y_MSB_ADDR, &buffer[3]);
//
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_Z_LSB_ADDR, &buffer[4]);
//    i2c.readReg(I2C_SLAVE_ADDR, BNO055_GYRO_DATA_Z_MSB_ADDR, &buffer[5]);
//
//    gyro[0] = buffer[0] | (buffer[1] << 8);
//    gyro[1] = buffer[2] | (buffer[3] << 8);
//    gyro[2] = buffer[4] | (buffer[5] << 8);
//
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw x: %d", gyro[0] / 16.0);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw y: %d", gyro[1] / 16.0);
//    log::LOGGER.log(log::Logger::LogLevel::INFO, "Gyroscope Raw z: %d", gyro[2] / 16.0);
}
}// namespace IMU

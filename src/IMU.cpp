#include <EVT/utils/log.hpp>
#include <IMU.hpp>

namespace log = EVT::core::log;
namespace IO = EVT::core::IO;

namespace IMU {
IMU::IMU(IO::I2C& i2C) : i2c(i2C) { }

void IMU::setMode(uint8_t mode) {
    _mode = mode;
    i2c.writeReg(I2C_SLAVE_ADDR, BNO055_OPR_MODE_ADDR, mode);
    EVT::core::time::wait(30);
}

CO_OBJ_T* IMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t IMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

void IMU::setup() {
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting Initialization, waiting 850 ms for boot...\r\n");
    // The board can take up to 850 ms to boot.
    EVT::core::time::wait(850);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be booted now... Checking if we can read...\r\n");

    uint8_t id;
    IO::I2C::I2CStatus status = i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, 0x00, &id);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "I2C::I2CStatus: %d\n\r", status);

    if (status != IO::I2C::I2CStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed read chip ID register with I2C::I2CStatus: %d\n\r", status);
        return;
    }

    log::LOGGER.log(log::Logger::LogLevel::ERROR, "Read (0x%X).\r\n", id);

    if (id != BNO055_ID) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed first initialization... Trying again.\r\n");

        EVT::core::time::wait(100);// hold on for boot
        i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &id);
        if (id != BNO055_ID) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initialize the IMU\r\n");
            return;
        }
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device booted!\r\n");

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Putting IMU into Configuration Mode.\r\n");

    // Put the device into configuration mode
    setMode(bno055OperationMode::OPERATION_MODE_CONFIG);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be in config mode, resetting system now, this should take 850 ms...\r\n");
    // Reset the system
    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_SYS_TRIGGER_ADDR, 0x20);
    EVT::core::time::wait(30);

    // Wait for the system boot again 🤦
    do {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Checking system boot status...\r\n");
        i2c.readReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_CHIP_ID_ADDR, &id);
        EVT::core::time::wait(10);
    }
    while(id != BNO055_ID);


    log::LOGGER.log(log::Logger::LogLevel::INFO, "System booted.\r\n");

    EVT::core::time::wait(50);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Setting the default power mode.\r\n");
    // Set the default power mode
    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_PWR_MODE_ADDR, bno055PowerMode::POWER_MODE_NORMAL);
    EVT::core::time::wait(10);

    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_PAGE_ID_ADDR, 0);

    i2c.writeReg(IMU::IMU::I2C_SLAVE_ADDR, BNO055_SYS_TRIGGER_ADDR, 0x0);
    EVT::core::time::wait(10);

    /* Set the requested operating mode (see section 3.3) */
    setMode(bno055OperationMode::OPERATION_MODE_NDOF);

    EVT::core::time::wait(20);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "IMU Successfully (hopefully) Initialized.\r\n");
}
}// namespace IMU

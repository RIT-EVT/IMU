#include <BNO055.hpp>

IMU::BNO055::BNO055(IO::I2C& i2C, uint8_t i2cSlaveAddress) : i2c(i2C) {
    i2cAddress = i2cSlaveAddress;
}

/**
 * This function is a doozie, most of the issues with it come from the fact that the BNO055 does not use standard i2c.
 * Because of this, none of the standard EVT readReg or writeReg functions work. This function uses the write and read functions
 * from EVT-Core to write specific bytes over i2c to correctly interact with the standard the BNO055 uses. We are not sure
 * about what the board is using, all data was retrieved by using a Saleae device and inspecting the Arduino code.
 *
 * @return a boolean indicating whether or not the device was successfully initialized
 */
bool IMU::BNO055::setup() {
    // TODO: Check at the function call to see if it actually succeeds at setup.
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting Initialization...\r\n");

    // We next send a system reset signal. This resets the device and brings it off the i2c network for period of time.
    // Resetting also restores optimum values for the device to enter sleep or wake up. (section 3.2.2).
    uint8_t resetBytes[2] = {BNO055_SYS_TRIGGER_ADDR, 0x20}; // RST_SYS is bit 5 of the SYS_TRIGGER
    i2c.write(i2cAddress, resetBytes, 2);
    time::wait(30); // might not be necessary as long as sensors are detected

    // This loop, will run until i2c returns a detected device and reports a successful connection. Register 0x00 is the chip id
    // register.
    int timeout = 3000; // ms
    do {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Was not detected at (0x%x), try again...\r\n", i2cAddress);
        time::wait(10);
        timeout -= 10;
    } while (i2c.write(i2cAddress, 0x00) != IO::I2C::I2CStatus::OK && timeout);
    if (!timeout){
        // try one more time
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Trying once more, otherwise failed to detect IMU device with i2c and will quit initialization\r\n");
        i2c.write(i2cAddress, 0x00);
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be booted now... Checking if we can read...\r\n");

    // Read the ID again from chip id register, this is to make sure we are still connected to the correct device.
    uint8_t id = 0;
    i2c.write(i2cAddress, 0x00);
    i2c.read(i2cAddress, &id);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "ID Read 0x%x\r\n", id);
    if (id != BNO055_ID) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed first initialization... Trying again.\r\n");

        time::wait(1000);// Hold on for boot
        i2c.write(i2cAddress, 0x00); // keep reading id;
        i2c.read(i2cAddress, &id);

        if (id != BNO055_ID) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initialize the IMU. Quitting initialization\r\n");
            return false;
        }
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to i2c!\r\n");

    // We need to wait another 50ms for the devie to figure itself out.
    time::wait(50);

    // Check if the current mode of device is configuration mode. If not in configuration mode, write the configuration bytes for the
    // device. This sets the device into config mode which makes most of the registers writeable (section 3.3.1) allowing us to
    // configure its settings.
    uint8_t currMode;
    i2c.write(i2cAddress, BNO055_OPR_MODE_ADDR);
    i2c.read(i2cAddress, &currMode);
    if (currMode != OPERATION_MODE_CONFIG){
        uint8_t configBytes[2] = {BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG};
        i2c.write(i2cAddress, configBytes, 2);
        EVT::core::time::wait(30);
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Setting power mode...\r\n");
    // Set the power mode for the device, by writing to the Power mode address, followed by the normal power mode value.
    // pg 19,
    uint8_t powerModeBytes[2] = {BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL}; // default value
    i2c.write(i2cAddress, powerModeBytes, 2);
    time::wait(10);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Power mode set.\r\n");

    // Reset the device page to 0x00.
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Reset device page.\r\n");
    // Power on page 0 is selected,
    // page id use to identify current selected page and change between page 0 and page 1
    // register map separated into two logical page (page 1 contains sensor specific config, page 0 contains config parameters)
    uint8_t pageBytes[2] = {BNO055_PAGE_ID_ADDR, 0x00};
    i2c.write(i2cAddress, pageBytes, 2);

    // Reset the system reset bits to 0x00, they were 0x20 because that's what we set them too earlier in the function.
    // triggers self test
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Reset the system reset.\r\n");
    uint8_t systemResetResetBytes[2] = {BNO055_SYS_TRIGGER_ADDR, 0x00};
    i2c.write(i2cAddress, systemResetResetBytes, 2);

    time::wait(10);

    // Set the config mode to an operation mode that will report data. All of the values for this can be found in the datasheet.
    // NDOF turns on all sensors on absolute orientation
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Set config mode to all data.\r\n");
    uint8_t config2Bytes[2] = {BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF}; // page 21
    i2c.write(i2cAddress, config2Bytes, 2);
    time::wait(20);

    // If everything above worked, the device has successfully booted.
    log::LOGGER.log(log::Logger::LogLevel::INFO, "System successfully booted!\r\n");
    return true;
}

IO::I2C::I2CStatus IMU::BNO055::getEuler(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    return fetchData(BNO055_EULER_H_LSB_ADDR, xBuffer, yBuffer, zBuffer);
}

IO::I2C::I2CStatus IMU::BNO055::getGyroscope(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    return fetchData(BNO055_GYRO_DATA_X_LSB_ADDR, xBuffer, yBuffer, zBuffer);
}

IO::I2C::I2CStatus IMU::BNO055::getLinearAccel(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    return fetchData(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, xBuffer, yBuffer, zBuffer);
}

IO::I2C::I2CStatus IMU::BNO055::getAccelerometer(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    return fetchData(BNO055_ACCEL_DATA_X_LSB_ADDR, xBuffer, yBuffer, zBuffer);
}

IO::I2C::I2CStatus IMU::BNO055::getGravity(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    return fetchData(BNO055_GRAVITY_DATA_X_LSB_ADDR, xBuffer, yBuffer, zBuffer);
}

IO::I2C::I2CStatus IMU::BNO055::fetchData(uint8_t lowestAddress, uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer) {
    // Create a buffer to read the 6 bytes of data into that we are about to read.
    uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

    // Write the byte for the address we want to read, this is going to be the lowest bit address of the data.
    IO::I2C::I2CStatus writeStatus = i2c.write(i2cAddress, lowestAddress);
    if (writeStatus != IO::I2C::I2CStatus::OK) {
        return writeStatus;
    }

    // Read the next 6 bytes from the lowest address we just wrote into the buffer.
    IO::I2C::I2CStatus readStatus = i2c.read(i2cAddress, buffer, 6);
    if (readStatus != IO::I2C::I2CStatus::OK) {
        return readStatus;
    }

    // Combine the buffer values so we have x, y, and z values.
    xBuffer = buffer[0] | (buffer[1] << 8);
    yBuffer = buffer[2] | (buffer[3] << 8);
    zBuffer = buffer[4] | (buffer[5] << 8);

    return readStatus;
}

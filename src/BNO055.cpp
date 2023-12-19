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
IMU::BNO055::BNO055Status IMU::BNO055::setup() {

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting Initialization...\r\n");

    // We check that BNO055's i2c is activated already. If it is, we check if it is in operational mode.
    // If it is in operational mode, we will manually trigger the board reset.
    if (i2c.write(i2cAddress, 0x00) == IO::I2C::I2CStatus::OK) {
        uint8_t currMode;
        i2c.write(i2cAddress, BNO055_OPR_MODE_ADDR);
        i2c.read(i2cAddress, &currMode);
        if (currMode != OPERATION_MODE_CONFIG) {
            log::LOGGER.log(log::Logger::LogLevel::INFO, "Device is not in configuration mode, resetting device.");
            // We trigger a POR system reset. This resets the device and brings it off the i2c network for period of time.
            // Resetting also restores optimum values for the device to enter sleep or wake up. (section 3.2.2).
            uint8_t resetBytes[2] = {BNO055_SYS_TRIGGER_ADDR, 0x20};// RST_SYS is bit 5 of the SYS_TRIGGER
            i2c.write(i2cAddress, resetBytes, 2);
        }
    }

    // Now board should be in configuration mode, either from start up or reset.
    // We have to wait 650 ms which is the standard time for i2c to start up
    time::wait(650);

    // Check if i2c returns a detected device and reports a successful connection. Read the ID from chip id register (0x00)
    // this is to make sure we are connected to the device
    uint8_t id = 0;
    if (i2c.write(i2cAddress, 0x00) != IO::I2C::I2CStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Failed to detect IMU device with i2c and will quit initialization\r\n");
        return BNO055::BNO055Status::FAIL_INIT;
    }
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Device should be booted now... Checking if we can read...\r\n");
    i2c.read(i2cAddress, &id);
    log::LOGGER.log(log::Logger::LogLevel::INFO, "ID Read 0x%x\r\n", id);
    if (id != BNO055_ID) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed first initialization... Trying again.\r\n");

        time::wait(1000);           // Hold on for boot
        i2c.write(i2cAddress, 0x00);// keep reading id;
        i2c.read(i2cAddress, &id);

        if (id != BNO055_ID) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initialize the IMU. Quitting initialization.\r\n");
            return BNO055::BNO055Status::FAIL_INIT;
        }
    }

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to i2c!\r\n");

    // We need to wait another 50ms for the device to figure itself out.
    time::wait(50);

    uint8_t result;
    // We read the ST_RESULT register that the startup self-test updates once completed.
    // The self-test checks that all sensors are functional.
    i2c.write(i2cAddress, BNO055_ST_RESULT);
    i2c.read(i2cAddress, &result);
    // All four LSB bits of result should be 1 for successful test
    if ((result & 0x0F) != 0x0F) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Self-test failed. Quitting initialization.\r\n");
        return BNO055::BNO055Status::FAIL_SELF_TEST;
    } else {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Self-test passed, all sensors and microcontroller are functioning.\r\n");
    }

    // Set the config mode to an operation mode that will report data. All of the values for this can be found in the datasheet.
    // NDOF turns on all sensors on absolute orientation.
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Set config mode to all data.\r\n");
    uint8_t config2Bytes[2] = {BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF};
    i2c.write(i2cAddress, config2Bytes, 2);
    time::wait(20);

    // If everything above worked, the device has successfully booted.
    log::LOGGER.log(log::Logger::LogLevel::INFO, "System successfully booted!\r\n");
    return BNO055::BNO055Status::OK;
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

#ifndef IMU_BNO055_HPP
#define IMU_BNO055_HPP

#include <EVT/io/I2C.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/time.hpp>

#define BNO055_CHIP_ID_ADDR (0x00)
#define BNO055_ID (0xA0)

/* Page id register definition */
#define BNO055_PAGE_ID_ADDR (0X07)

/* Mode registers */
#define BNO055_OPR_MODE_ADDR (0X3D)
#define BNO055_PWR_MODE_ADDR (0X3E)

#define BNO055_SYS_TRIGGER_ADDR (0X3F)

/** BNO055 Registers **/
#define BNO055_ACCEL_DATA_X_LSB_ADDR (0X08)
#define BNO055_MAG_DATA_X_LSB_ADDR (0X0E)
#define BNO055_GYRO_DATA_X_LSB_ADDR (0X14)
#define BNO055_EULER_H_LSB_ADDR (0X1A)
#define BNO055_QUATERNION_DATA_W_LSB_ADDR (0X20)
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR (0X28)
#define BNO055_GRAVITY_DATA_X_LSB_ADDR (0X2E)

/** Operation mode settings **/
#define OPERATION_MODE_CONFIG (0x00)
#define OPERATION_MODE_ACCONLY (0x01)
#define OPERATION_MODE_MAGONLY (0x02)
#define OPERATION_MODE_GYRONLY (0x03)
#define OPERATION_MODE_ACCMAG (0x04)
#define OPERATION_MODE_ACCGYRO (0x05)
#define OPERATION_MODE_MAGGYRO (0x06)
#define OPERATION_MODE_AMG (0x07)
#define OPERATION_MODE_IMUPLUS (0x08)
#define OPERATION_MODE_COMPASS (0x09)
#define OPERATION_MODE_M4G (0x0A)
#define OPERATION_MODE_NDOF_FMC_OFF (0x0B)
#define OPERATION_MODE_NDOF (0x0C)

/** BNO055 power settings */
#define POWER_MODE_NORMAL (0X00)

namespace IO = EVT::core::IO;

namespace log = EVT::core::log;
namespace time = EVT::core::time;

namespace IMU {

/**
 * A driver class for the BNO055 orientation sensor used in the IMU.
 * Notice:
 * This class uses a non standard (possibly) version of i2c that does not work with the EVT-Core provided `readReg` and `writeReg`.
 * For this reason, all i2c calls in this driver use the raw `write` and `read` functions to directly emulate the code found in the
 * arduino source.
 *
 * Datasheet for BNO055 - https://cdn-shop.adafruit.com/datasheets/BST_BNO055_DS000_12.pdf
 * Arduino Source - https://github.com/adafruit/Adafruit_BNO055
 */
class BNO055 {
public:
    /**
     * Initializer for a BNO055 sensor.
     * Takes in i2c to setup a connection with the board
     *
     * @param i2C an initialized i2c session.
     */
    BNO055(IO::I2C& i2C);

    /**
     * Sends all of the required i2c commands to initialize the chip
     *
     * @return whether the setup succeeded.
     */
    bool setup();

    /**
     * Fetch the euler angle data.
     *
     * @param xBuffer a buffer to store the x data in.
     * @param yBuffer a buffer to store the y data in.
     * @param zBuffer a buffer to store the z data in.
     *
     * @return an i2c status reporting if the fetch worked or not.
     */
    IO::I2C::I2CStatus getEuler(int16_t& xBuffer, int16_t& yBuffer, int16_t& zBuffer);

    /**
     * Fetch the gyroscope data.
     *
     * @param xBuffer a buffer to store the x data in.
     * @param yBuffer a buffer to store the y data in.
     * @param zBuffer a buffer to store the z data in.
     *
     * @return an i2c status reporting if the fetch worked or not.
     */
    IO::I2C::I2CStatus getGyroscope(int16_t& xBuffer, int16_t& yBuffer, int16_t& zBuffer);

    /**
     * Fetch the linear acceleration data.
     *
     * @param xBuffer a buffer to store the x data in.
     * @param yBuffer a buffer to store the y data in.
     * @param zBuffer a buffer to store the z data in.
     *
     * @return an i2c status reporting if the fetch worked or not.
     */
    IO::I2C::I2CStatus getLinearAccel(int16_t& xBuffer, int16_t& yBuffer, int16_t& zBuffer);

    /**
     * Fetch the accelerometer data.
     *
     * @param xBuffer a buffer to store the x data in.
     * @param yBuffer a buffer to store the y data in.
     * @param zBuffer a buffer to store the z data in.
     * 
     * @return an i2c status reporting if the fetch worked or not.
     */
    IO::I2C::I2CStatus getAccelerometer(int16_t& xBuffer, int16_t& yBuffer, int16_t& zBuffer);

private:
    /**
     * The i2c address for the BNO055.
     */
    static constexpr uint8_t I2C_SLAVE_ADDR = 0x28;

    /**
     * The i2c object used for communication.
     */
    IO::I2C& i2c;

    /**
     * Fetch data from the BNO055 using the custom i2c specification used by the device.
     *
     * @param lowestAddress the lowest address to read the data from.
     * @param xBuffer a buffer to store the x data in.
     * @param yBuffer a buffer to store the y data in.
     * @param zBuffer a buffer to store the z data in.
     *
     * @return an i2c status reporting if the fetch worked or not.
     */
    IO::I2C::I2CStatus fetchData(uint8_t lowestAddress, int16_t& xBuffer, int16_t& yBuffer, int16_t& zBuffer);
};

}// namespace IMU

#endif//IMU_BNO055_HPP

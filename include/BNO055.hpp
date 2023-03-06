#include <EVT/io/I2C.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/log.hpp>

#ifndef IMU_BNO055_HPP
#define IMU_BNO055_HPP

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

class BNO055 {
public:
    BNO055(IO::I2C& i2C);
    bool setup();

    IO::I2C::I2CStatus getEuler(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer);
    IO::I2C::I2CStatus getGyroscope(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer);
    IO::I2C::I2CStatus getLinearAccel(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer);
    IO::I2C::I2CStatus getAccelerometer(uint16_t& xBuffer, uint16_t& yBuffer, uint16_t& zBuffer);
private:
    static constexpr uint8_t I2C_SLAVE_ADDR = 0x28;
    EVT::core::IO::I2C& i2c;
};

}

#endif//IMU_BNO055_HPP

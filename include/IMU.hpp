#pragma once

#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/CANopen.hpp>
#include <EVT/utils/time.hpp>
#include <Canopen/co_core.h>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;

namespace IMU {
#define BNO055_CHIP_ID_ADDR (0x00)
#define BNO055_ID (0xA0)

/* Page id register definition */
#define BNO055_PAGE_ID_ADDR (0X07)

/* Mode registers */
#define BNO055_OPR_MODE_ADDR (0X3D)
#define BNO055_PWR_MODE_ADDR (0X3E)

#define BNO055_SYS_TRIGGER_ADDR (0X3F)

/** BNO055 Registers **/
typedef enum {
    /* Page id register definition */
    BNO055_ACCEL_REV_ID_ADDR = 0x01,
    BNO055_MAG_REV_ID_ADDR = 0x02,
    BNO055_GYRO_REV_ID_ADDR = 0x03,
    BNO055_SW_REV_ID_LSB_ADDR = 0x04,
    BNO055_SW_REV_ID_MSB_ADDR = 0x05,
    BNO055_BL_REV_ID_ADDR = 0X06,

    /* Accel data register */
    BNO055_ACCEL_DATA_X_LSB_ADDR = 0X08,
    BNO055_ACCEL_DATA_X_MSB_ADDR = 0X09,
    BNO055_ACCEL_DATA_Y_LSB_ADDR = 0X0A,
    BNO055_ACCEL_DATA_Y_MSB_ADDR = 0X0B,
    BNO055_ACCEL_DATA_Z_LSB_ADDR = 0X0C,
    BNO055_ACCEL_DATA_Z_MSB_ADDR = 0X0D,

    /* Mag data register */
    BNO055_MAG_DATA_X_LSB_ADDR = 0X0E,
    BNO055_MAG_DATA_X_MSB_ADDR = 0X0F,
    BNO055_MAG_DATA_Y_LSB_ADDR = 0X10,
    BNO055_MAG_DATA_Y_MSB_ADDR = 0X11,
    BNO055_MAG_DATA_Z_LSB_ADDR = 0X12,
    BNO055_MAG_DATA_Z_MSB_ADDR = 0X13,

    /* Gyro data registers */
    BNO055_GYRO_DATA_X_LSB_ADDR = 0X14,
    BNO055_GYRO_DATA_X_MSB_ADDR = 0X15,
    BNO055_GYRO_DATA_Y_LSB_ADDR = 0X16,
    BNO055_GYRO_DATA_Y_MSB_ADDR = 0X17,
    BNO055_GYRO_DATA_Z_LSB_ADDR = 0X18,
    BNO055_GYRO_DATA_Z_MSB_ADDR = 0X19,

    /* Euler data registers */
    BNO055_EULER_H_LSB_ADDR = 0X1A,
    BNO055_EULER_H_MSB_ADDR = 0X1B,
    BNO055_EULER_R_LSB_ADDR = 0X1C,
    BNO055_EULER_R_MSB_ADDR = 0X1D,
    BNO055_EULER_P_LSB_ADDR = 0X1E,
    BNO055_EULER_P_MSB_ADDR = 0X1F,

    /* Quaternion data registers */
    BNO055_QUATERNION_DATA_W_LSB_ADDR = 0X20,
    BNO055_QUATERNION_DATA_W_MSB_ADDR = 0X21,
    BNO055_QUATERNION_DATA_X_LSB_ADDR = 0X22,
    BNO055_QUATERNION_DATA_X_MSB_ADDR = 0X23,
    BNO055_QUATERNION_DATA_Y_LSB_ADDR = 0X24,
    BNO055_QUATERNION_DATA_Y_MSB_ADDR = 0X25,
    BNO055_QUATERNION_DATA_Z_LSB_ADDR = 0X26,
    BNO055_QUATERNION_DATA_Z_MSB_ADDR = 0X27,

    /* Linear acceleration data registers */
    BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR = 0X28,
    BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR = 0X29,
    BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR = 0X2A,
    BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR = 0X2B,
    BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR = 0X2C,
    BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR = 0X2D,

    /* Gravity data registers */
    BNO055_GRAVITY_DATA_X_LSB_ADDR = 0X2E,
    BNO055_GRAVITY_DATA_X_MSB_ADDR = 0X2F,
    BNO055_GRAVITY_DATA_Y_LSB_ADDR = 0X30,
    BNO055_GRAVITY_DATA_Y_MSB_ADDR = 0X31,
    BNO055_GRAVITY_DATA_Z_LSB_ADDR = 0X32,
    BNO055_GRAVITY_DATA_Z_MSB_ADDR = 0X33,

    /* Temperature data register */
    BNO055_TEMP_ADDR = 0X34,

    /* Status registers */
    BNO055_CALIB_STAT_ADDR = 0X35,
    BNO055_SELFTEST_RESULT_ADDR = 0X36,
    BNO055_INTR_STAT_ADDR = 0X37,

    BNO055_SYS_CLK_STAT_ADDR = 0X38,
    BNO055_SYS_STAT_ADDR = 0X39,
    BNO055_SYS_ERR_ADDR = 0X3A,

    /* Unit selection register */
    BNO055_UNIT_SEL_ADDR = 0X3B,

    /* Mode registers */
    BNO055_TEMP_SOURCE_ADDR = 0X40,

    /* Axis remap registers */
    BNO055_AXIS_MAP_CONFIG_ADDR = 0X41,
    BNO055_AXIS_MAP_SIGN_ADDR = 0X42,

    /* SIC registers */
    BNO055_SIC_MATRIX_0_LSB_ADDR = 0X43,
    BNO055_SIC_MATRIX_0_MSB_ADDR = 0X44,
    BNO055_SIC_MATRIX_1_LSB_ADDR = 0X45,
    BNO055_SIC_MATRIX_1_MSB_ADDR = 0X46,
    BNO055_SIC_MATRIX_2_LSB_ADDR = 0X47,
    BNO055_SIC_MATRIX_2_MSB_ADDR = 0X48,
    BNO055_SIC_MATRIX_3_LSB_ADDR = 0X49,
    BNO055_SIC_MATRIX_3_MSB_ADDR = 0X4A,
    BNO055_SIC_MATRIX_4_LSB_ADDR = 0X4B,
    BNO055_SIC_MATRIX_4_MSB_ADDR = 0X4C,
    BNO055_SIC_MATRIX_5_LSB_ADDR = 0X4D,
    BNO055_SIC_MATRIX_5_MSB_ADDR = 0X4E,
    BNO055_SIC_MATRIX_6_LSB_ADDR = 0X4F,
    BNO055_SIC_MATRIX_6_MSB_ADDR = 0X50,
    BNO055_SIC_MATRIX_7_LSB_ADDR = 0X51,
    BNO055_SIC_MATRIX_7_MSB_ADDR = 0X52,
    BNO055_SIC_MATRIX_8_LSB_ADDR = 0X53,
    BNO055_SIC_MATRIX_8_MSB_ADDR = 0X54,

    /* Accelerometer Offset registers */
    ACCEL_OFFSET_X_LSB_ADDR = 0X55,
    ACCEL_OFFSET_X_MSB_ADDR = 0X56,
    ACCEL_OFFSET_Y_LSB_ADDR = 0X57,
    ACCEL_OFFSET_Y_MSB_ADDR = 0X58,
    ACCEL_OFFSET_Z_LSB_ADDR = 0X59,
    ACCEL_OFFSET_Z_MSB_ADDR = 0X5A,

    /* Magnetometer Offset registers */
    MAG_OFFSET_X_LSB_ADDR = 0X5B,
    MAG_OFFSET_X_MSB_ADDR = 0X5C,
    MAG_OFFSET_Y_LSB_ADDR = 0X5D,
    MAG_OFFSET_Y_MSB_ADDR = 0X5E,
    MAG_OFFSET_Z_LSB_ADDR = 0X5F,
    MAG_OFFSET_Z_MSB_ADDR = 0X60,

    /* Gyroscope Offset register s*/
    GYRO_OFFSET_X_LSB_ADDR = 0X61,
    GYRO_OFFSET_X_MSB_ADDR = 0X62,
    GYRO_OFFSET_Y_LSB_ADDR = 0X63,
    GYRO_OFFSET_Y_MSB_ADDR = 0X64,
    GYRO_OFFSET_Z_LSB_ADDR = 0X65,
    GYRO_OFFSET_Z_MSB_ADDR = 0X66,

    /* Radius registers */
    ACCEL_RADIUS_LSB_ADDR = 0X67,
    ACCEL_RADIUS_MSB_ADDR = 0X68,
    MAG_RADIUS_LSB_ADDR = 0X69,
    MAG_RADIUS_MSB_ADDR = 0X6A
} adafruit_bno055_reg_t;

/** Vector Mappings **/
typedef enum {
    VECTOR_ACCELEROMETER = BNO055_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_MAGNETOMETER = BNO055_MAG_DATA_X_LSB_ADDR,
    VECTOR_GYROSCOPE = BNO055_GYRO_DATA_X_LSB_ADDR,
    VECTOR_EULER = BNO055_EULER_H_LSB_ADDR,
    VECTOR_LINEARACCEL = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_GRAVITY = BNO055_GRAVITY_DATA_X_LSB_ADDR
} adafruit_vector_type_t;

/** Operation mode settings **/
typedef enum {
    OPERATION_MODE_CONFIG = 0X00,
    OPERATION_MODE_ACCONLY = 0X01,
    OPERATION_MODE_MAGONLY = 0X02,
    OPERATION_MODE_GYRONLY = 0X03,
    OPERATION_MODE_ACCMAG = 0X04,
    OPERATION_MODE_ACCGYRO = 0X05,
    OPERATION_MODE_MAGGYRO = 0X06,
    OPERATION_MODE_AMG = 0X07,
    OPERATION_MODE_IMUPLUS = 0X08,
    OPERATION_MODE_COMPASS = 0X09,
    OPERATION_MODE_M4G = 0X0A,
    OPERATION_MODE_NDOF_FMC_OFF = 0X0B,
    OPERATION_MODE_NDOF = 0X0C
} bno055OperationMode;

/** BNO055 power settings */
typedef enum {
    POWER_MODE_NORMAL = 0X00,
    POWER_MODE_LOWPOWER = 0X01,
    POWER_MODE_SUSPEND = 0X02
} bno055PowerMode;

/**
 * This is an example of a class for a board
 */
class IMU {
public:
    static constexpr uint8_t I2C_SLAVE_ADDR = 0x28;
    static constexpr uint8_t NODE_ID = 0x17;
    IMU(IO::I2C& i2C);

    /**
     * Gets the object dictionary
     *
     * @return an object dictionary
     */
    CO_OBJ_T* getObjectDictionary();

    /**
     * Gets the size of the Object Dictionary
     *
     * @return size of the Object Dictionary
     */
    uint16_t getObjectDictionarySize() const;

    bool setup();

    void setMode(uint8_t mode);

    void loop();
private:
    EVT::core::IO::I2C& i2c;

    uint8_t _mode = 0x00;


    /**
    * 0. VECTOR_EULER_X - vectorXValues[0]
    * 1. VECTOR_GYROSCOPE_X - vectorXValues[1]
    * 2. VECTOR_LINEAR_ACCEL_X - vectorXValues[2]
    * 3. VECTOR_ACCELEROMETER_X - vectorXValues[3]
     */
    uint16_t vectorXValues[4] = {1, 2, 3, 4};

    /**
    * 0. VECTOR_EULER_Y - vectorYValues[0]
    * 1. VECTOR_GYROSCOPE_Y - vectorYValues[1]
    * 2. VECTOR_LINEAR_ACCEL_Y - vectorYValues[2]
    * 3. VECTOR_ACCELEROMETER_Y - vectorYValues[3]
     */
    uint16_t vectorYValues[4] = {1, 2, 3, 4};

    /**
    * 0. VECTOR_EULER_Z - vectorZValues[0]
    * 1. VECTOR_GYROSCOPE_Z - vectorZValues[1]
    * 2. VECTOR_LINEAR_ACCEL_Z - vectorZValues[2]
    * 3. VECTOR_ACCELEROMETER_Z - vectorZValues[3]
     */
    uint16_t vectorZValues[4] = {1, 2, 3, 4};

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 49;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        // Sync ID, defaults to 0x80
        {
            CO_KEY(0x1005, 0, CO_UNSIGNED32 | CO_OBJ_D__R_),
            nullptr,
            (uintptr_t) 0x80,
        },
        /**
         * Information about the hardware , hard coded sample values for now
         * 1: Vendor ID
         * 2: Product Code
         * 3: Revision Number
         * 4: Serial Number
         */
        {
            .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x10,
        },
        {
            .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x11,
        },
        {
            .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x12,
        },
        {
            .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x13,
        },

        /**
         * SDO CAN message IDS.
         * 1: Client -> Server ID, default is 0x600 + NODE_ID
         * 2: Server -> Client ID, default is 0x580 + NODE_ID
         */
        {
            .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x600 + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x580 + NODE_ID,
        },

        /**
         * TPDO 0 Settings
         * 0. The number of sub indxes.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5,
        },
        {
            //180h+Node-ID
            .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0) + NODE_ID,
        },
        {
            //timer triggered
            .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            //no inhibit time
            .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            //send every 2 seconds
            .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },
        /**
         * TPDO 1 Settings
         * 0. The number of sub indxes.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1801, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5,
        },
        {
            //180h+Node-ID
            .Key = CO_KEY(0x1801, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(1) + NODE_ID,
        },
        {
            //timer triggered
            .Key = CO_KEY(0x1801, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            //no inhibit time
            .Key = CO_KEY(0x1801, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            //send every 2 seconds
            .Key = CO_KEY(0x1801, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },
        /**
         * TPDO 2 Settings
         * 0. The number of sub indxes.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1802, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5,
        },
        {
            //180h+Node-ID
            .Key = CO_KEY(0x1802, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(2) + NODE_ID,
        },
        {
            //timer triggered
            .Key = CO_KEY(0x1802, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            //no inhibit time
            .Key = CO_KEY(0x1802, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            //send every 2 seconds
            .Key = CO_KEY(0x1802, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000,
        },
        /**
         * TPDO 0 Mapping
         * 0. The number of mapping objects in the first TPDO
         * 1. Link to VECTOR_EULER_X - vectorXValues[0]
         * 2. Link to VECTOR_GYROSCOPE_X - vectorXValues[1]
         * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorXValues[2]
         * 4. Link to VECTOR_ACCELEROMETER_X - vectorXValues[3]
         */
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4},
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 0, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 1, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 2, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 3, 16),
        },
        /**
         * TPDO 1 Mapping
         * 0. The number of mapping objects in the first TPDO
        * 1. Link to VECTOR_EULER_X - vectorYValues[0]
        * 2. Link to VECTOR_GYROSCOPE_X - vectorYValues[1]
        * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorYValues[2]
        * 4. Link to VECTOR_ACCELEROMETER_X - vectorYValues[3]
         */
        {
            .Key = CO_KEY(0x1A01, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4,
        },
        {
            .Key = CO_KEY(0x1A01, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 0, 16),
        },
        {
            .Key = CO_KEY(0x1A01, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 1, 16),
        },
        {
            .Key = CO_KEY(0x1A01, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 2, 16),
        },
        {
            .Key = CO_KEY(0x1A01, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 3, 16),
        },
        /**
         * TPDO 2 Mapping
         * 0. The number of mapping objects in the first TPDO
         * 1. Link to VECTOR_EULER_X - vectorZValues[0]
         * 2. Link to VECTOR_GYROSCOPE_X - vectorZValues[1]
         * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorZValues[2]
         * 4. Link to VECTOR_ACCELEROMETER_X - vectorZValues[3]
         */
        {
            .Key = CO_KEY(0x1A02, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4,
        },
        {
            .Key = CO_KEY(0x1A02, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 0, 16),
        },
        {
            .Key = CO_KEY(0x1A02, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 1, 16),
        },
        {
            .Key = CO_KEY(0x1A02, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 2, 16),
        },
        {
            .Key = CO_KEY(0x1A02, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 3, 16),
        },

        /**
         * Data Links
         */
        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorXValues[0],
        },
        {
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorXValues[1],
        },
        {
            .Key = CO_KEY(0x2100, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorXValues[2],
        },
        {
            .Key = CO_KEY(0x2100, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorXValues[3],
        },
        {
            .Key = CO_KEY(0x2101, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorYValues[0],
        },
        {
            .Key = CO_KEY(0x2101, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorYValues[1],
        },
        {
            .Key = CO_KEY(0x2101, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorYValues[2],
        },
        {
            .Key = CO_KEY(0x2101, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorYValues[3],
        },
        {
            .Key = CO_KEY(0x2102, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorZValues[0],
        },
        {
            .Key = CO_KEY(0x2102, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorZValues[1],
        },
        {
            .Key = CO_KEY(0x2102, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorZValues[2],
        },
        {
            .Key = CO_KEY(0x2102, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorZValues[3],
        },
        CO_OBJ_DIR_ENDMARK,
    };
};
}// namespace IMU

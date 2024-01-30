#pragma once

#include <BNO055.hpp>
#include <EVT/io/CANopen.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/utils/time.hpp>

#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <co_core.h>
namespace IO = EVT::core::IO;

namespace IMU {

/**
 * The IMU main class that manages a BNO055 for measuring inertia
 */
class IMU : public CANDevice {
public:
    static constexpr uint8_t NODE_ID = 9;

    /**
     * Basic constructor for an IMU instance
     *
     * @param[in] bno055 BNO instance to read data from
     */
    explicit IMU(BNO055 bno055);

    /**
     * Gets the object dictionary
     *
     * @return an object dictionary
     */
    CO_OBJ_T* getObjectDictionary() override;

    /**
     * Gets the size of the Object Dictionary
     *
     * @return size of the Object Dictionary
     */
    uint8_t getNumElements() override;

    /**
    * Get the device's node ID
    *
    * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Handle running the core logic of the IMU
     */
    void process();

private:
    BNO055 bno055;

    /**
    * 0. VECTOR_EULER_X - vectorXValues[0]
    * 1. VECTOR_GYROSCOPE_X - vectorXValues[1]
    * 2. VECTOR_LINEAR_ACCEL_X - vectorXValues[2]
    * 3. VECTOR_ACCELEROMETER_X - vectorXValues[3]
     */
    uint16_t vectorXValues[4] = {};

    /**
    * 0. VECTOR_EULER_Y - vectorYValues[0]
    * 1. VECTOR_GYROSCOPE_Y - vectorYValues[1]
    * 2. VECTOR_LINEAR_ACCEL_Y - vectorYValues[2]
    * 3. VECTOR_ACCELEROMETER_Y - vectorYValues[3]
     */
    uint16_t vectorYValues[4] = {};

    /**
    * 0. VECTOR_EULER_Z - vectorZValues[0]
    * 1. VECTOR_GYROSCOPE_Z - vectorZValues[1]
    * 2. VECTOR_LINEAR_ACCEL_Z - vectorZValues[2]
    * 3. VECTOR_ACCELEROMETER_Z - vectorZValues[3]
     */
    uint16_t vectorZValues[4] = {};

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 58;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {

        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 50),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x01, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 50),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x02, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 50),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x04),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 2, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 3, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 4, PDO_MAPPING_UNSIGNED16),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x01, 0x04),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 2, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 3, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 4, PDO_MAPPING_UNSIGNED16),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x02, 0x04),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 2, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 3, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 4, PDO_MAPPING_UNSIGNED16),

        DATA_LINK_START_KEY_21XX(0x00, 0x04),
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED16, &vectorXValues[0]),
        DATA_LINK_21XX(0x00, 0x02, CO_TUNSIGNED16, &vectorXValues[1]),
        DATA_LINK_21XX(0x00, 0x03, CO_TUNSIGNED16, &vectorXValues[2]),
        DATA_LINK_21XX(0x00, 0x04, CO_TUNSIGNED16, &vectorXValues[3]),

        DATA_LINK_START_KEY_21XX(0x01, 0x04),
        DATA_LINK_21XX(0x01, 0x01, CO_TUNSIGNED16, &vectorYValues[0]),
        DATA_LINK_21XX(0x01, 0x02, CO_TUNSIGNED16, &vectorYValues[1]),
        DATA_LINK_21XX(0x01, 0x03, CO_TUNSIGNED16, &vectorYValues[2]),
        DATA_LINK_21XX(0x01, 0x04, CO_TUNSIGNED16, &vectorYValues[3]),

        DATA_LINK_START_KEY_21XX(0x02, 0x04),
        DATA_LINK_21XX(0x02, 0x01, CO_TUNSIGNED16, &vectorZValues[0]),
        DATA_LINK_21XX(0x02, 0x02, CO_TUNSIGNED16, &vectorZValues[1]),
        DATA_LINK_21XX(0x02, 0x03, CO_TUNSIGNED16, &vectorZValues[2]),
        DATA_LINK_21XX(0x02, 0x04, CO_TUNSIGNED16, &vectorZValues[3]),

        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace IMU

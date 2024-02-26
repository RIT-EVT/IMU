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
 * Main class for the IMU that manages its built-in BNO055 and manages CAN communication.
 */
class IMU : public CANDevice {
public:
    /** The node ID is used to identify the device on the CAN network */
    static constexpr uint8_t NODE_ID = 9;

    /**
     * Basic constructor for an IMU instance. It calls the initialization routine of the BNO055.
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
     * Handle running the core logic of the IMU. This involves calling upon BNO055 to retrieve and log data.
     */
    void process();

private:
    /** The BNO055 sensor of the IMU */
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
    * The object dictionary itself. Will be populated by this object during
    * construction.
    *
    * The plus one is for the special "end of dictionary" marker.
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

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO
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

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace IMU

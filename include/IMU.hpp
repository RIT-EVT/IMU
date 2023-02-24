#pragma once

#include <Canopen/co_core.h>
#include <EVT/io/CANopen.hpp>

namespace IMU {

/**
 * This is an example of a class for a board
 */
class IMU {
public:
    static constexpr uint8_t NODE_ID = 0x17;

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

private:
    /**
     * A vector for the euler data.
     * 0. VECTOR_EULER_X
     * 1. VECTOR_EULER_Y
     * 2. VECTOR_EULER_Z
     */
    uint16_t vectorEuler[3] = { 1, 2, 3};

    /**
     * A vector for the gyroscope data.
     * 0. VECTOR_GYROSCOPE_X
     * 1. VECTOR_GYROSCOPE_Y
     * 2. VECTOR_GYROSCOPE_Z
     */
    uint16_t vectorGyroscope[3]= { 1, 2, 3};

    /**
     * A vector for the linear acceleration data.
     * 0. VECTOR_LINEAR_ACCEL_X
     * 1. VECTOR_LINEAR_ACCEL_Y
     * 2. VECTOR_LINEAR_ACCEL_Z
     */
    uint16_t vectorLinearAccel[3] = { 1, 2, 3};

    /**
     * A vector for the accelerometer data.
     * 0. VECTOR_ACCELEROMETER_X
     * 1. VECTOR_ACCELEROMETER_Y
     * 2. VECTOR_ACCELEROMETER_Z
     */
    uint16_t vectorAccelerometer[3] = { 1, 2, 3};

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
         * TPDO 1 Settings
         * 0. The number of sub indxes.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5
        },
        {   //180h+Node-ID
            .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0) + NODE_ID
        },
        {   //timer triggered
            .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE
        },
        {   //no inhibit time
            .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0
        },
        {   //send every 2 seconds
            .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000
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
            .Key = CO_KEY(0x1801, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5
        },
        {   //180h+Node-ID
            .Key = CO_KEY(0x1801, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(1) + NODE_ID
        },
        {   //timer triggered
            .Key = CO_KEY(0x1801, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE
        },
        {   //no inhibit time
            .Key = CO_KEY(0x1801, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0
        },
        {   //send every 2 seconds
            .Key = CO_KEY(0x1801, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 2000
        },
        /**
         * TPDO 3 Settings
         * 0. The number of sub indxes.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1802, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5
        },
        {   //180h+Node-ID
         .Key = CO_KEY(0x1802, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
         .Type = nullptr,
         .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(2) + NODE_ID
        },
        {   //timer triggered
         .Key = CO_KEY(0x1802, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
         .Type = nullptr,
         .Data = (uintptr_t) 0xFE
        },
        {   //no inhibit time
         .Key = CO_KEY(0x1802, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
         .Type = nullptr,
         .Data = (uintptr_t) 0
        },
        {   //send every 2 seconds
         .Key = CO_KEY(0x1802, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
         .Type = CO_TEVENT,
         .Data = (uintptr_t) 2000
        },
        /**
         * TPDO 1 Mapping
         * 0. The number of mapping objects in the first TPDO
         * 1. Link to VECTOR_EULER_X - vectorEuler[0]
         * 2. Link to VECTOR_GYROSCOPE_X - vectorGyroscope[0]
         * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorLinearAccel[0]
         * 4. Link to VECTOR_ACCELEROMETER_X - vectorAccelerometer[0]
         */
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4
        },
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 0, 16)
        },
        {
            .Key = CO_KEY(0x1A00, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 1, 16)
        },
        {
            .Key = CO_KEY(0x1A00, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 2, 16)
        },
        {
            .Key = CO_KEY(0x1A00, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 3, 16)
        },
        /**
         * TPDO 2 Mapping
         * 0. The number of mapping objects in the first TPDO
        * 1. Link to VECTOR_EULER_X - vectorEuler[1]
        * 2. Link to VECTOR_GYROSCOPE_X - vectorGyroscope[1]
        * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorLinearAccel[1]
        * 4. Link to VECTOR_ACCELEROMETER_X - vectorAccelerometer[1]
         */
        {
            .Key = CO_KEY(0x1A01, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4
        },
        {
            .Key = CO_KEY(0x1A01, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 0, 16)
        },
        {
            .Key = CO_KEY(0x1A01, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 1, 16)
        },
        {
            .Key = CO_KEY(0x1A01, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 2, 16)
        },
        {
            .Key = CO_KEY(0x1A01, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 3, 16)
        },
        /**
         * TPDO 3 Mapping
         * 0. The number of mapping objects in the first TPDO
         * 1. Link to VECTOR_EULER_X - vectorEuler[2]
         * 2. Link to VECTOR_GYROSCOPE_X - vectorGyroscope[2]
         * 3. Link to VECTOR_LINEAR_ACCEL_X - vectorLinearAccel[2]
         * 4. Link to VECTOR_ACCELEROMETER_X - vectorAccelerometer[2]
         */
        {
            .Key = CO_KEY(0x1A02, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4
        },
        {
            .Key = CO_KEY(0x1A02, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 0, 16)
        },
        {
            .Key = CO_KEY(0x1A02, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 1, 16)
        },
        {
            .Key = CO_KEY(0x1A02, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 2, 16)
        },
        {
            .Key = CO_KEY(0x1A02, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2102, 3, 16)
        },

        /**
         * Data Links
         */
        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorEuler[0],
        },
        {
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorGyroscope[0],
        },
        {
            .Key = CO_KEY(0x2100, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorLinearAccel[0],
        },
        {
            .Key = CO_KEY(0x2100, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorAccelerometer[0],
        },
        {
            .Key = CO_KEY(0x2101, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorEuler[1],
        },
        {
            .Key = CO_KEY(0x2101, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorGyroscope[1],
        },
        {
            .Key = CO_KEY(0x2101, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorLinearAccel[1],
        },
        {
            .Key = CO_KEY(0x2101, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorAccelerometer[1],
        },
        {
            .Key = CO_KEY(0x2102, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorEuler[2],
        },
        {
            .Key = CO_KEY(0x2102, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorGyroscope[2],
        },
        {
            .Key = CO_KEY(0x2102, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorLinearAccel[2],
        },
        {
            .Key = CO_KEY(0x2102, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &vectorAccelerometer[2],
        },
        CO_OBJ_DIR_ENDMARK,
    };
};
}// namespace IMU
/*
BO_ 913 N17_TPDO2: 8 IMU
    SG_ VECTOR_ACCELEROMETER_Z : 48|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_LINEAR_ACCEL_Z : 32|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_GYROSCOPE_Z : 16|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_EULER_Z : 0|16@1- (1,0) [0|0] "" Vector__XXX

BO_ 657 N17_TPDO1: 8 IMU
    SG_ VECTOR_ACCELEROMETER_Y : 48|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_LINEAR_ACCEL_Y : 32|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_GYROSCOPE_Y : 16|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_EULER_Y : 0|16@1- (1,0) [0|0] "" Vector__XXX

BO_ 401 N17_TPDO0: 8 IMU
    SG_ VECTOR_ACCELEROMETER_X : 48|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_LINEAR_ACCEL_X : 32|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_GYROSCOPE_X : 16|16@1- (1,0) [0|0] "" Vector__XXX
    SG_ VECTOR_EULER_X : 0|16@1- (1,0) [0|0] "" Vector__XXX
*/
API
###

This documentation outlines the library for IMU.

DEV
***
Devices, representation of hardware that can be interfaced with. In
general, devices are communicated with via some sort of IO interface, but that
is not strictly a rule. An LED is a simplistic example of a device.

IMU
===

.. doxygenclass:: IMU::IMU
   :members:

BNO055
======

.. doxygenclass:: IMU::BNO055
    :members:

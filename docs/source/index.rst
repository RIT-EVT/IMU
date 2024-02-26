.. Documentation master file, created by
   sphinx-quickstart on Sat Sep  4 10:17:15 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

===============================
Welcome to IMU's documentation!
===============================

Introduction
============

The Inertial Measurement Unit measures acceleration and position data for the
bike using the BNO055 chip. This chip uses its sensors (
accelerometers, magnetometer, and gyroscope) to calculate a number of
metrics, including gravity vector, linear acceleration, angular velocity,
Euler angles, and the surrounding magnetic field. The IMU reports these
measurements over CAN, providing general information to the bike on its
orientation and allowing other boards to act accordingly for optimal
performance. The system is based around the built-in BNO055 chip. The firmware
is written for the STM32F334K8U6 microcontroller designed into the DEV1 IMU.

The DEV1 IMU will have the following responsibilities:

1. Measure linear and angular acceleration of the bike in 3D space (X,Y,Z)
2. Report measurements over the DEV1 CAN network

For more information on the BNO055 chip and its features, refer to
its `datasheet <https://www.bosch-sensortec
.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf>`_.

For more information on the IMU and software level API, refer to
the `Read the Docs DEV1 IMU page <https://imu.readthedocs.io>`_.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   api/index.rst
   srs.rst

Docs
====

EVT-core is the RIT-EVT-produced library for interfacing with the STM32f3xx
family of microcontrollers used on the DEV1 IMU and other PCBs made by the
team. For more information on that library, visit
`RTD for EVT-core <https://evt-core.readthedocs.io/en/latest/>`_.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

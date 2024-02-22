# DEV1 IMU

## Introduction

### DEV1 IMU

The Inertial Measurement Unit measures linear and angular acceleration of the bike using the BNO055 chip. More metrics (gravity, linear and angular velocity, pitch/yaw/roll, and the surrounding magnetic field) can be calculated from the two. The IMU also reports any relevant data over CAN. Such data provides general information to the bike on its orientation and allows other boards to act accordingly for optimal performance. The system is based around the built-in BNO055 chip. The firmware is written for the STM32F334K8U6 microcontroller designed into the DEV1 IMU.

The DEV1 IMU will have the following responsibilities:
1. Measure linear and angular acceleration in 3D space (X,Y,Z) of bike
2. Expose the system over the DEV1 CAN network

For more information on the BNO055 chip and its features, refer to its [datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf).

For more information on the IMU and software level API, refer to the [Read the Docs DEV1 IMU page](https://imu.readthedocs.io).

### Documentation

Documentation is handled via Sphinx. To build the documentation, navigate to the docs/ folder and run make html. You can then open the generated docs/build/html/index.html.

To generate a PDF of the Software Requirements Specification, run the command rinoh docs/source/srs.rst --output docs. You can then view the PDF version of the SRS in docs/srs.pdf. The SRS is identical to the one generated via make html.

### Related Projects

The DEV1 BMS is one component of the larger DEV1 project, you can find related projects in the [RIT-EVT](https://github.com/RIT-EVT) GitHub page. Additionally, the STM32f334 driver code is handled via the [EVT-core](https://github.com/RIT-EVT/EVT-core) project.
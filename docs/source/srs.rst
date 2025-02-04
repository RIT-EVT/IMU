======================================================================
Software Requirements Specification for DEV1 Inertial Measurement Unit
======================================================================


Introduction
============

Purpose
-------
The purpose of this document is to detail the software requirements and
constraints for the firmware of the Dirt Electric Vehicle 1 Inertial
Measurement Unit (DEV1 IMU). This document will go into detail on the
requirements necessary for the system as well as detailing the constraints that
the system will be under. The intention is that this document will provide a
means to holistically express the needs of the DEV1 IMU system.

Document Conventions
--------------------
This document was created based on the IEEE template for software requirements
specifications. As such it will mainly adhere to the verbiage and style set by
IEEE convention. Additionally, a glossary has been provided in the Appendix
which covers common phrases that will be used in this document.

Intended Audience and Reading Suggestions
-----------------------------------------
The intention is that this document will be accessible across engineering
disciplines. The DEV1 IMU represents a highly inter-disciplinary project on the
RIT Electric Vehicle Team and as such this document should be accessible to all
involved. Below is a breakdown of the intended audiences and how they may use
the document. This list is not exhaustive, but intended to guide common readers
of this document.

* Electrical Team Members: Members of the electrical team who are designing
  and building the DEV1 IMU. Members of this group may refer to this document
  to ensure requirements and constraints align with their expectations.
  Additionally, this document can be used as a point of reference during
  the hardware/firmware bringup process.
* Firmware Team Members: Developers on the firmware team who are designing,
  developing, and testing the DEV1 IMU firmware. Members of this team will
  need to refer to this document throughout the development process to ensure
  all target needs are met within the agreed upon constraints.
* Integration Team Members: RIT EVT team members who handle systems-level
  integration on the team. These team members may use this document to gain
  and understanding of how the DEV1 IMU will operate. Most critically, how the
  DEV1 IMU will operate within the structure of the DEV1 architecture.

The document is laid out where it is not strictly necessary to read all sections
in their entirety. Instead each section should be self-contained and 
comprehensive within its scope. As such audience members are encouraged to read 
the sections that most pertain to their needs. Additionally, the glossary in the
Appendix may prove useful in situations where phrases may be used in specific 
ways within this document.

Product Scope
-------------

The DEV1 IMU measures inertial data for the DEV1 bike. The DEV1 IMU
reports its measurements over the DEV1 CAN network for other boards
to act accordingly for optimal performance.

Overview
--------

The rest of this software design document will go further into the specifics of 
the requirements while also looking at the constraints of the system. The goal 
is to clarify the use cases of the DEV1 IMU and to specify what the DEV1 IMU 
will do in those cases. Design considerations will not be discussed, however 
notable design constraints will be covered.


Overall Description
===================

Product Perspective
-------------------

The DEV1 IMU plays a role in performance optimization for the DEV1 bike. It
reports measurements such as linear/angular acceleration and Euler angle over the
bike's CAN network, allowing other boards to adjust their behaviors to
maintain the best performance.

The DEV1 IMU is made up of two primary components, the STM32F334K8 microcontroller
and the BNO055 orientation sensor chip. The ST microcontroller handles
processing of data and manages CAN network communication for the IMU. The
BNO055 is used to obtain the orientation measurements.

Currently, the IMU data is not being used by other boards, but can be used for
traction control in the future.

User Interfaces
~~~~~~~~~~~~~~~

Users will rarely interact directly with the DEV1 IMU software. The DEV1 IMU 
software will mainly be interfaced with via CANopen and thus will require 
additional tools to interact with the DEV1 IMU. There is no current plan 
for a team-developed tool to provide an interface for interaction with the 
DEV1 IMU.

Hardware Interfaces
~~~~~~~~~~~~~~~~~~~

The DEV1 IMU is connected to the bike's CANopen network which is made up of a
two-wire differential pair. The board also features a JTAG connector that exposes
an SWD interface for programming and a UART interface for communicating directly
with the microcontroller.

Software Interfaces
~~~~~~~~~~~~~~~~~~~

The DEV1 IMU software reports of measurements over the CANopen network. 
Note that the behavior of the IMU cannot be modified over CANopen.

Communication Interfaces
~~~~~~~~~~~~~~~~~~~~~~~~

The DEV1 IMU communicates over the CANopen network to broadcast its data
to other boards.

Communication between the STM32F334r8 microcontroller and the BNO055 are
handled via I2C, however it does not adhere to standard I2C. For more about
the non-standard I2C communication scheme, read **(WIP, will include I2C
implementation detail in the State of the Software doc)**

Memory Constraints
~~~~~~~~~~~~~~~~~~

The produced software is limited to the 64KB of flash memory that is available
on the STM32F334r8. Therefore the resulting binary must fit within this size.

Operations
~~~~~~~~~~

The DEV1 IMU's main operation is to relay data measured by the BNO055
sensor to the rest of the bike through the CANopen network. This includes
initialization and configuration of the BNO055 chip, and managing the
CANopen network communication.

Site Adaptation
~~~~~~~~~~~~~~~

The DEV1 IMU is intended specifically for the DEV1 system. Therefore, the 
software requirements and design will center around the specifics of the DEV1 
system. No additional adaptations are currently being considered.

Initialization and Configuration of BNO055
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One of the main components of the DEV1 IMU is the BNO055 chip, which requires
proper initialization and configuration before it will collect data. The
following procedure is informed by the BNO055's datasheet which details
correct usage of the chip. After establishing I2C connection with the chip,
the driver class will:

* check that the BNO055's self-test initiated at startup was successful
* switch the BNO055 from configuration mode to NDOF operation mode

The NDOF operation mode calculates fused absolute orientation data from the
accelerometer, gyroscope and magnetometer. More information can be found in
the BNO055 datasheet.

Notification of BNO055 Data
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The BNO055 will collect the following data in 3D space (X,Y,Z):

* Euler angles
* gyroscope measurements
* linear acceleration
* accelerometer measurements

The data is broadcast on the CANopen network for other boards to read.

User Classes and Characteristics
--------------------------------

The IMU chip is an auxiliary board that does not require any user interaction
post-development.

Operating Environments
----------------------
The software will operate on the ST microcontroller present on the DEV1 IMU.
The software environment is embedded with no operating system present. All
development will take place through the EVT-core library and will interact
directly with the ST microcontroller.

User Documentation
------------------

Additional documentation will need to exist for boards that will react to
the DEV1 IMU's reported data. Information for each board will need to include
which data it will use and how it will adjust itself accordingly. This will
provide users with a holistic view of how the DEV1 IMU is used in the bike.

Constraints
-----------

Below are some constraints worth considering. They are a fixed part of the
system.

* Development must be in C/C++
* Communication will take place using CANopen
* EVT-core will be used for low level microcontroller interfacing
* Must be developed for the STM32F334r8
* Resulting binary must fit within the ST microcontroller 64KB flash memory
* Orientation measurements must be performed by the BNO055
* Non-standard I2C communication with the BNO055

Assumptions and Dependencies
----------------------------

It is assumed that the BNO055 chip will behave exactly as described by its
datasheet, unless communication between the BNO055 chip and ST
microcontroller fails.

Apportioning of Requirements
----------------------------

At this point in the life cycle of the DEV1 IMU project, deployment has begun.
Due to the nature of student-run teams, it is unlikely that there will be
further revision to this system, as new students will likely start new projects.
As a result, there are no future requirements planned to be added to this
system.

Specific Requirements
=====================

External Interface Requirements
-------------------------------

* no external interface

Functions
---------

The DEV1 IMU follows a specific procedure for proper initialization and
configuration of the BNO055 chip. A general outline of what the system shall
do is described in the Product Functions section under `Initialization
and Configuration of BNO055`_.

Appendix
========

Glossary
--------

===========   ===========================================
Term          Definition
-----------   -------------------------------------------
IMU           Inertial Measurement Unit
CAN           Controller Area Network
CANopen       Higher-level communication protocol using CAN
DEV1          Dirt Electric Vehicle 1
EVT           Electric Vehicle Team
I2C           Inter-Integrated Circuit
KB            Kilobyte
STM32F334K8   ST Microcontroller selected for this project
===========   ===========================================

References
----------
* `BNO055 Intelligent 9-axis absolute orientation sensor <https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf>`_
* `CANopen - The standardized embedded network <https://www.can-cia.org/canopen/>`_
* `EVT-core <https://evt-core.readthedocs.io/en/latest/>`_
* `STM32F334K8 Mainstream Mixed signals MCUs Arm Cortex-M4 core with DSP and FPU, 64 Kbytes of Flash memory, 72 MHz CPU, CCM, 12-bit ADC 5 MSPS, comparators, op-amp, hr timer <https://www.st.com/en/microcontrollers-microprocessors/stm32f334k8.html#documentation>`_

Revision
--------

========    ============================          ==========
Revision    Description                           Date
--------    ----------------------------          ----------
1           Initial documentation.                2/4/2025
========    ============================          ==========

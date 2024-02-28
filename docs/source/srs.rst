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
and building the DEV1 IMU. Members of this group may refer to this document to
ensure requirements and constraints align with their expectations. Additionally,
this document can be used as a point of reference during the hardware/firmware
bring up and debugging.

* Firmware Team Members: Developers on the firmware team who are designing,
developing, and testing the DEV1 IMU firmware. Members of this team will need to
refer to this document throughout the development process to ensure all target 
needs are met within the agreed upon constraints.

* Integration Team Members: RIT EVT team members who handle systems level
integration on the team. These team members may use this document to gain and 
understanding of how the DEV1 IMU will operate. Most critically, how the DEV1 
IMU will operate within the structure of the DEV1 architecture.

The document is laid out where it is not strictly necessary to read all sections
in their entirety. Instead each section should be self-contained and 
comprehensive within its scope. As such audience members are encouraged to read 
the sections that most pertain to their needs. Additionally, the glossary in the
Appendix may prove useful in situations where phrases may be used in specific 
ways within this document.

Product Scope
-------------

The DEV1 IMU measures acceleration and position data for the DEV1 bike. The
DEV IMU reports its measurements over the DEV1 CAN network to provide a
general overview of the bike's orientation for other boards to act
accordingly for optimal performance.

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

Purpose of the IMU - optimize performance of the bike
configurable components - microcontroller and BNO055
list some boards that will use the data



User Interfaces
~~~~~~~~~~~~~~~

Not that i'm aware of


Hardware Interfaces
~~~~~~~~~~~~~~~~~~~

CANopen network


Software Interfaces
~~~~~~~~~~~~~~~~~~~

CANopen network


Communication Interfaces
~~~~~~~~~~~~~~~~~~~~~~~~

CANopen


Memory Constraints
~~~~~~~~~~~~~~~~~~

The produced software is limited to the 64KB of flash memory that is available
on the STM32F334r8. Therefore the resulting binary must fit within this size.


Operations
~~~~~~~~~~

output measurements, any signals??


Site Adaptation
~~~~~~~~~~~~~~~

Not that i'm aware of


Product Functions
-----------------

I2c communication with BNO055 (but not standard)
Notification of measurements


User Classes and Characteristics
--------------------------------
don't know

Operating Environments
----------------------
ST microcontroller


User Documentation
------------------



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
* non standard i2c

Assumptions and Dependencies
----------------------------

Apportioning of Requirements
----------------------------


Specific Requirements
=====================
all of the intialization for IMU
what to report etc.

External Interface Requirements
-------------------------------


Functions
---------


Appendix
========

Glossary
--------

===========   ===========================================
Term          Definition
-----------   -------------------------------------------
IMU           Inertial Measurement Unit
CAN           Controller Area Network
CANopen       Communication protocol built on CAN
DEV1          Dirt Electric Vehicle Team
EVT           Electrical Vehicle Team
I2C           Inter-Integrated Circuit
KB            Kilobyte
STM32F334K8   ST Microcontroller selected for this project
===========   ===========================================

References
----------
* `BNO055 Intelligent 9-axis absolute orientation sensor <https://www
.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf>`_
* `CANopen - The standardized embedded network <https://www.can-cia.org/canopen/>`_
* `EVT-core <https://evt-core.readthedocs.io/en/latest/>`_
* `STM32F334K8 Mainstream Mixed signals MCUs Arm Cortex-M4 core with DSP and
FPU, 64 Kbytes of Flash memory, 72 MHz CPU, CCM, 12-bit ADC 5 MSPS, comparators, op-amp, hr timer <https://www.st.com/en/microcontrollers-microprocessors/stm32f334k8.html#documentation>`_

Revision
--------

========    ============================          ==========
Revision    Description                           Date
--------    ----------------------------          ----------
1           Initial documentation.
========    ============================          ==========

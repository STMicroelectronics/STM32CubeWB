# STM32CubeWB MCU Firmware

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeWB.svg?color=brightgreen)

> [!IMPORTANT]
> This repository contains **submodules**. Please refer to the ["How to"](README.md#how-to-clone-and-get-updates) section for more details.

## Overview of the STM32Cube MCU offer on GitHub

**STM32Cube** is an original initiative by STMicroelectronics to **simplify** prototyping and development by **reducing** effort, time, and cost. It supports the entire ARM™ Cortex-based STM32 microcontroller portfolio and provides a **comprehensive** software solution including:
  * The CMSIS Core and Device interfaces enabling access to processor core features and device-specific peripherals of STM32 microcontrollers.
  * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
  * The BSP drivers enabling access to peripherals on the STM32 development boards, external to the microcontroller itself.
  * A consistent set of middleware libraries offering standardized, high-level functionalities — such as USB, TCP/IP, file systems, and graphics.
  * A full set of software projects (basic examples, applications, and demonstrations) that showcase specific functionalities or use cases, and provided with support for multiple IDEs.

The **STM32Cube embedded software** is available in two flavors:
  * The **MCU Firmware** _monolithic_ offer, where **all** software components (Drivers, Middleware, Projects, Utilities) are included in a **single** repository for each STM32 series.
  * The **MCU Software Components** _modular_ offer, where **each** software component (mainly Drivers and Middleware) is provided in a **dedicated** repository, allowing users to **select** only the components they need.

The complete list of repositories is available [here](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer/blob/master/README.md#content).

## Repository content

This repository provides the **STM32CubeWB** MCU firmware, including **all** software components (Drivers, Middleware, Projects, Utilities).

> [!NOTE]
> ## Some middleware libraries and projects are unavailable in this repository
> 
> In this repository, the middleware libraries listed below **along with** [this](Projects/README.md#list-of-unavailable-projects) list of projects (demos, applications, and examples) using them, are **not available** as they (the middleware libraries) are subject to some restrictive license terms requiring the user's approval via a "click thru" procedure.
> * `./Middlewares/ST/STM32_Audio`
> 
> If needed, they can be found inside the full firmware package available on our website `st.com` and downloadable from [here](https://www.st.com/en/embedded-software/stm32cubewb.html#get-software). You will be prompted to login or to register in case you have no account.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Release_Notes.html).

## How to clone and get updates

This repository contains **submodules**.
* To **clone** it, run the command below
```
git clone --recursive https://github.com/STMicroelectronics/STM32CubeWB.git
```
* To **pull** the latest updates, run the commands below from the root of the repository
```
git pull; git submodule update --init --recursive
```

> [!CAUTION]
> If the "Download ZIP" option is used instead of the `git clone` command, then the different **submodules** have to be collected and added **manually**.

> [!NOTE]
> The latest version of this firmware available on GitHub may be **ahead** of the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html) or via [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). This is due to the **rolling release**. Please refer to [this](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer/discussions/21) post for more details.

## Boards available

  * STM32WB55
    * [NUCLEO-WB15CC](https://www.st.com/en/evaluation-tools/nucleo-wb15cc.html)
    * [P-NUCLEO-WB55.Nucleo](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html)
    * [P-NUCLEO-WB55.USBDongle](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html)
    * [STM32WB5MM-DK](https://www.st.com/en/evaluation-tools/stm32wb5mm-dk.html)

## Feedback and contributions

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.

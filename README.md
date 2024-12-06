# avreertos-plusplus – An AVR-C++-FreeRTOS project template

## Intro

Have you ever thought about taking the next step away from Arduino to professional use of the AVR microcontrollers? Ideally with modern C++ and an RTOS on top?

Well, here is a template for you to start with. It makes use of [CMake](https://cmake.org/), the [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) and [frt](https://github.com/Floessie/frt) wrapper, and [avr-libstdcpp](https://github.com/modm-io/avr-libstdcpp). As an example it targets the very common ATmega328P, but your real target should be one from the AVR-Dx series. Not only do they have up to 16kB of SRAM, they are also [well supported](https://www.freertos.org/Documentation/02-Kernel/03-Supported-devices/04-Demos/Atmel-now-Microchip/microchip-avr-dx-demo#configuration-and-usage-details) by the FreeRTOS kernel including [tickless idle](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/07-Lower-power-support) mode.

If you are adventurous enough, you can drop avr-libstdcpp from your project and build your own avr-gcc toolchain with a freestanding libstdc++ using the wonderful [crosstool-ng](https://github.com/crosstool-ng/crosstool-ng). But the avr-gcc supplied with your distribution should be enough to get started. If you plan to use avr-libstdcpp, make sure that your compiler is new enough (e.g. avr-gcc 5.4 will not suffice).

## How(-To)

The "third party" components are included as [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules), so best use `git clone --recurse-submodules` when cloning from here.

To build this project template you need CMake and avr-gcc. To test the image, you need avrdude. On Debian you can install those prerequisites with:

```
$ sudo apt-get install cmake gcc-avr avrdude
```

After that, follow the usual CMake way:

```
$ mkdir build
$ cd build
$ cmake ..  -DCMAKE_TOOLCHAIN_FILE=../cmake/avr_gcc_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
$ make -j
```

Suppose you have an ATmega328P Arduino with the standard bootloader at hand, you can now upload the HEX file with avrdude:

```
$ avrdude -p m328p -c arduino -P /dev/ttyUSB0 -b 115200 -U flash:w:example.hex:i
```

## Thanks

Getting FreeRTOS up and running would not have been possible that quickly without the AWS folks at Amazon, who took the kernel out of the classic full distribution and provided a CMake template for using it in your bare-metal project.

Another source of inspiration was the [cmake_avr8_howto](https://github.com/positronic57/cmake_avr8_howto). Kudos to Goce Boshkovski.

The quick hack to enable avr-libc's `printf()` was taken from ["Simple Serial Communications With AVR libc"](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/) by Mika Tuupola. This blocking implementation is for quick debugging only. A better, interrupt-driven solution is included in the FreeRTOS demos, e.g. [here](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/AVR_Dx_IAR/serial/serial.c) for AVR-Dx.
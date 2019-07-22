# tuya-dimmer-mcu-firmware

OSS implementation of the TRIAC and faceplate controller for the Tuya Wi-Fi dimmers

inspired by: [Replacement firmware for tuya dimmer MCU - GitHub](https://github.com/chaosmaster/tuya_mcu)

Features:
* fixed issues with random flickering of the `tuya_mcu`
* Auto mains frequency adjustment
* Protocol modified to skip replies to the external commands. See https://github.com/arendst/Sonoff-Tasmota/issues/4909
* Added "fade-in", "fade-out" effects
* minor cleanup

# Build
* if required, download and install [SDCC](http://sdcc.sourceforge.net/) and Gnu-Make. Add binaries to $(PATH)
* for STM8 programming via SWIM debug interface
    - install libusb-dev (e.g. `sudo apt-get install libusb-1.0-0-dev`)
    - download and make [stm8flash](https://github.com/vdudouyt/stm8flash) source code
    - on Linux grant write access to ST-Link debugger by creating a file `/etc/udev/rules.d/99-stlinkv2.rules` with content  
    `SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3744", MODE="0666"`  
    Note: since several versions of ST-Link exist, check the idProduct of your device via command `usb-devices`
* `make clean all`
* `make flash`

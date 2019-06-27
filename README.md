# tuya-dimmer-mcu-firmware

OSS implementation of the TRIAC and faceplate controller for the Tuya Wi-Fi dimmers

inspired by https://github.com/chaosmaster/tuya_mcu

Features:
* fixed issues with random flickering of the `tuya_mcu`
* Auto mains freuency adjustment
* Protocol modified to skip replies to the external commands. See https://github.com/arendst/Sonoff-Tasmota/issues/4909
* Added "fade-in", "fade-out" effects
* minor cleanup

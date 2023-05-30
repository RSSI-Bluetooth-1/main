Useful information about hardware nRF52840 DK and dongles:
1. DK
Micro USB 2.0 will connect DK to the computer. If older versions of USB is used, computer will not recognize the DK.
When connected to USB, if led is steady (not blinking), it means DK is connected to the computer. (this is needed for flashing DK with software code).
If led is blinking in fast-pace, it means DK is powered on but not connected to the computer.

2. Dongle
Use nRF Connect for Desktop to install Programmer. Programmer is needed to flash dongles with .hex files.
.hex files are generated when building the program in VS Code (in our case, modified Zephyr samples).
When dongle is connected to USB port green led might turn on depending on code last used on it or no led lights might turn on. It's okay.
When dongle

Procedure of data gathering we used:
1. Make sure room is dark, don't turn on lights
2. Face the dongles up/standing
3. 





Data gathering days:
Day 1 - fingerprinting (9 hrs)
Day 2 - three beacons with 100 location combinations (5 hrs)
Day 3 - one beacon with 75 location combinations (2.5 hrs)

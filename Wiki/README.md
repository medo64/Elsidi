# Welcome #

This is wiki page for Elsidi - character LCD driver.

It receives commands and data on standard TTL-level UART interface and outputs them on attached HD44780 compatible character LCD.


## Elsidi features ##

### Serial UART input ###

Data is received on standard serial UART at 9600 bauds via [[Serial interface|simple interface]].

### Hitachi HD44780 LCD controller ###

Elsidi supports all HD44780 compatible controllers in either 4 or 8-bit mode.

### Multiple connector support ###

Elsidi supports all common character LCD connectors. You can use either one-row 16-pin connector, 2x8-pin one or one with 2x9-pin configuration.

### Software controlled contrast ###

LCD contrast is controllable via software and has 64 distinct levels.

### Software controlled backlight ###

LCD backlight has 64 levels and it can be fully controlled by software. It is designed for up to 450 mA of current.

### Full support for 40x4 LCD ###

Major distinguishing feature for Elsidi is full support for 18-pin 40x40 character LCD. To turn it on it is enough to send three-byte command. 


## Future features ##

Elsidi is constant work in progress and thus there are some features planned but
not yet implemented. Implementation highly depends on my free time.

### I2C ###

I2C support is already implemented on board level. Still pending is firmware
support.

### Serial bootloader ###

Having a serial bootloader would simplify firmware updates greatly.

### Custom characters ###

Software is also pending support for defining custom characters (up to 8).
Underlying hardware does support it but firmware is lacking once more.


## Other links ##

* [Command mode](CommandMode.md)
* [Directory structure](DirectoryStructure.md)
* [Serial interface](SerialInterface.md)

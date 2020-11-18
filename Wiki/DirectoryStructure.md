# Directory Structure #

Let's just get acquainted with directory structure used for this project.


## Hardware ##

All hardware related stuff should be found here.

### Board ###

Here we can find current board layout in [DipTrace](http://www.diptrace.com/)
format. 


### Releases ###

In this directory you will find latest working release for given development
branch (current one is L).

If you want to make board yourself easiest route would be to take all files in
given directory and send them to one of PCB manufacturers. I personally prefer
[OSH Park](http://oshpark.com/) but you can select any one you want.

For manufacturing it is enough to send enclosed `zip` file with gerbers. In
`txt` file you will find list of necessary components together with their
DigiKey code. Once everything gets built you should just program device with
enclosed `hex` file and programmer of your choice. I personally prefer
[PICKit 3](http://www.microchip.com/pickit3|PICKit 3) but you can use whatever
you want.


### Resources ###

Here you can find any resources I find useful for e.g. calculating PWM
parameters or something similar.


## Firmware ##

### Source ###

This folder contains current version of firmware for use with Microchip's
[MPLAB X](http://www.microchip.com/mplabx). To recompile it you will
additionally need [XC8 compiler](http://www.microchip.com/mplabx/). If in doubt
just use `hex` file enclosed in Hardware/Release directory.


## Software ##

### ElsidiTest ###

This is small program for testing and configuring board.


## Other links ##

* [Index](README.md)

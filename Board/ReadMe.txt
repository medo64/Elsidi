Elsidi (v6)
========================================================================



Board dimensions
------------------------------------------------------------------------

Board size ...................: 81.3mm x 50.8mm
Hole diameter ................: 1.0mm
Hole diameter (corners) ......: 4.2mm



Parts
------------------------------------------------------------------------

CONN. 2pin 2.54 M       1   JP1                 Jumper (MCLR)
CONN. 4pin pol          1   CN1                 Power
CONN. 2pin pol          1   CN2                 Data
CONN. 2x10pin           1   CN3                 Standard (20x4) and dual
                                                (40x4) LCD.
E 10uF 35V              1   E1                  Input smoothing
LED 3mm red             1   LED                 Power indicator
PIC16F628               1   IC1
R 330                   1   R1                  LED
R 10K                   3   R2 R3 R4            Pull-up
R TRIM 10K, small       1   POT1                Contrast
R TRIM 10K, small       1   POT2                Backlight



Ratings
------------------------------------------------------------------------

Input ........................: 5V / 800mA
Output (LCD) .................: 5V / 500mA (max)



Known issues
------------------------------------------------------------------------

- BUG: Power connector should have slightly bigger holes.
- COMPATIBILITY: In order to maintain compatibility with r3 source code
  RB0 is connected to VDD.



Changes (from v3)
------------------------------------------------------------------------

- BUGFIX: RA4 has pull-up resistor.



Changes (from v2)
------------------------------------------------------------------------

- BUGFIX: 5V and 12V lines are not interchanged anymore.



Changes (from v1)
------------------------------------------------------------------------

- DTR controls MCLR and thus PIC reset (selectable via JP2).
//- DTR off will switch LCD off (via transistor).
- Support for second E line (for 40x4 displays).
- Two-stage linear voltage regulator instead of one stage.
- Uses standard power connector.
- Pin assignment on PIC side is changed.



LCD connector
------------------------------------------------------------------------

LCD can be connected with standard 2x8 pin connector which will fit
inside of 2x10 connector used. If dual LCD support is needed (currently
not supported by software) full 2x10 connection must be used. This was
done in order to provide simple mean of connecting normal LCD's while
leaving option to connect larger LCD open.

Pinout:
    20x  16x  Signal  Function
      1       -
      2       -
      3    1  Vss     Ground.
      4    2  Vdd     Power supply.
      5    3  V0      Contrast adjustment.
      6    4  RS      Register select signal.
      7    5  RW      Read/write select signal.
      8    6  E       Enable (E1) signal.
      9    7  DB0     Data line.
     10    8  DB1     Data line.
     11    9  DB2     Data line.
     12   10  DB3     Data line.
     13   11  DB4     Data line.
     14   12  DB5     Data line.
     15   13  DB6     Data line.
     16   14  DB7     Data line.
     17   15  A       Anode (positive).
     18   16  K       Katode (ground).
     19       E2      Enable (E2) signal.
     20       -



Jumpers
------------------------------------------------------------------------

JP1
~MCLR is keept constantly active via power line. If it is shorted to GND
PIC will reset.

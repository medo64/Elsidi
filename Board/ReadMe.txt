Elsidi (v2)
========================================================================



Board dimensions
------------------------------------------------------------------------

Board size ...................: 81.3mm x 50.8mm
Hole diameter ................: 1.0mm
Hole diameter (corners) ......: 4.2mm



Parts
------------------------------------------------------------------------

1N4007                  7   D1 D2 D3 D4         Rectifier bridge
                            D5 D6               Capacitance protection
                            D7                  Reverse flow protection
7805                    1   IC4
7812                    1   IC3
C 330nF                 4   C1 C2 C3 C4         Ripple rejection
CONN. 2pin              1   PWR25V              Power
CONN. 2pin 2.54 M       1   JP1                 Jumper (12V input)
CONN. 2x2pin 2.54 M     1   JP2                 Jumper (MCLR source)
CONN. 4pin pol          1   PWR5V               Power
CONN. 8pin pol          1   CN2                 RS-232
CONN. 2x10pin           1   LCD1                Standard (20x4) and dual
                                                (40x4) LCD.
E 1uF                   4   E1 E2 E3 E4         MAX232 electrolyts
E 10uF 35V              1   E5                  Input smoothing
LED 3mm red             1   LED                 Power indicator
MAX232                  1   IC2
PIC16F628               1   IC1
R 330                   1   R1                  LED
R 10K                   1   R2                  Pull-up
R TRIM 10K, small       1   POT1                Contrast
R TRIM 10K, small       1   POT2                Backlight

//BC639                 1   T1                  Circuit switch



Ratings
------------------------------------------------------------------------

Input ........................: 5V / 800mA
                             or 12V-25V / 1A

Output (LCD) .................: 5V / 500mA (max)



Known issues
------------------------------------------------------------------------

- BUG: RA4 needs to be connected to power through R 10K.
- BUG: Power connector should have slightly bigger holes.



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



Power supply
------------------------------------------------------------------------

Only 5V voltage is needed. Higher voltage (12V-25V) should be connected
through two-stage linear voltage regulator. Since voltage needs to be at
least cca 3V higher than output, when 12V is supplied, JP1 should be
installed to bypass first stage of voltage regulator. No input or
intermediate electrolyts are provided and 10uF should be enough to
stabilize things enough for normal operation. Because of this two-stage
regulation no cooling should be required.



Jumpers
------------------------------------------------------------------------

JP1
If this jumper is installed, input voltage is taken directly to second
stage of regulator. It needs to be installed when voltage is lower than
14V.

JP2
This is two part jumper - either top or bottom side should be installed.
When top side is installed, ~MCLR is keept constantly active via power
line. If bottom is installed, MCLR is controlled via DTR line. PIC will
behave eraticaly if no jumper is installed!



Silicon errata
------------------------------------------------------------------------

- Missing pull-up resistor:
    RA4 line is missing pull-up resistor. This can be resolved by
    connecting 10K resistor between +5V and RA4.
- GND connection is disconnected:
    GND is not brought to last of bridge diodes. This can be resolved by
    making connection between GND pin on connector and negative side of
    bottom bridge diode.

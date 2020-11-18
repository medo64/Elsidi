# Serial Interface #

Each received character is sent to LCD and then echoed back if it is not one of
escape characters (ASCII `0x08` to `0x0F`). E.g. sending `123` will cause `123`
to appear on LCD.


## Escape characters ##

Escape characters are in ASCII range `0x08`-`0x0F` (`8`-`15` decimal). This
range was selected since only CGRAM characters are here and they can be selected
within `0x00`-`0x07` range.

Each escape sequence will result in `LF` if command has succeeded and it
will echo itself if it fails. 


### 0x08 BS ###

Backspace will cause prompt to be moved into upper left corner. Display will not
be cleared.
 

### 0x09 HT ###

Tab will cause Elsidi to move into [CommandMode](CommandMode.md). Command is
completed only once `LF` or `CR` is returned.


## 0x0A LF ##

Line feed character causes prompt to move to next line. After fourth line no
movement will be made until line is resetted (e.g. via `BS` escape character).

In single device mode these four lines are at: `0x00` (first line), `0x40`
(second line), `0x14` (third line), and `0x54` (fourth line).

In dual device mode (40x4 character display) these four lines are at: `0x00`
(first line), `0x40` (second line), `0x00` (third line), and `0x40` (fourth
line).


### 0x0B VT ###

Clears display.


### 0x0C FF ###

Character that follows is sent to LCD as a HD44780 command without any
interpretation on part of Elsidi. E.g. `FF``0x02` will cause LCD to execute
"Return home" command.

After instruction character, all further characters are interpreted as usual.


### 0x0D CR ###

Same as `LF`.


### 0x0E SO ###

Secondary HD44780-compatible display is selected. This means that  all commands
will be executed via E2 line. In case of 40x4 character LCD this enables
selection of lower two lines.


### 0x0F SI ###

Primary HD44780-compatible display is selected. This means that  all commands
will be executed via E1 line. This is normal mode of operation.


## Other links ##

* [Index](README.md)

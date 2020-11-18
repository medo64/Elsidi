# Command Mode #

Command mode is entered using `HT` character followed by command. What follows
is parameter to command, ending with `LF` (or `CR`).

If command is successful, it will echo `LF`, otherwise it will echo `HT`.


## Commands ##


### None (clear screen) ###

If no command is present screen will be cleared. This command always succeeds.

#### Example ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `LF`                                                        |
| Response: | `LF`                                                             |
| Result:   | Screen is cleared.                                               |


### ? (id) ###

This command echoes version of current firmware. Command receives no parameters.

#### Example ####
|           |                                                                              |
|-----------|------------------------------------------------------------------------------|
| Request:  | `HT` `?` `LF`                                                                |
| Response: | `E` `l` `s` `i` `d` `i` ` ` `L` `2` `0` `1` `3` `-` `0` `1` `-` `2` `5` `LF` |


### ~ (restore defaults) ###

This parameter-less command restores all setting to startup ones. This includes
contrast, backlight, device count, and data interface width.

#### Example ####
|           |                                                                               |
|-----------|-------------------------------------------------------------------------------|
| Request:  | `HT` `~` `LF`                                                                 |
| Response: | `LF`                                                                          |
| Result:   | Contrast, backlight, device count and data interface width as set to default. |


### * (turn display on/off) ###

Based on argument display will be turned on/off or current state will be
returned. Notice that display being off does not have any effect on backlight.

#### Arguments: ####

If there is no argument current display state is returned. 


#### Example: check current display state ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `*` `LF`                                                    |
| Response: | `+` `LF`                                                         |
| Result:   | Current display state (on) is returned.                          |

#### Example: turn on display ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `*` `+` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Display is turned on.                                            |

#### Example: turn off display ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `*` `-` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Display is turned off.                                           |


### $ (turn cursor on/off) ###

Based on argument cursor will be turned on/off or current state will be
returned.

#### Arguments: ####

If there is no argument current cursor state is returned. 


#### Example: check current cursor state ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `$` `LF`                                                    |
| Response: | `-` `LF`                                                         |
| Result:   | Current cursor state (off) is returned.                          |

#### Example: turn on cursor ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `$` `+` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Cursor is turned on.                                             |

#### Example: turn off cursor ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `$` `-` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Cursor is turned off.                                            |


### $ (turn cursor blink on/off) ###

Based on argument cursor blink will be turned on/off or current state will be
returned.

#### Arguments: ####

If there is no argument current cursor blink state is returned. 


#### Example: check current cursor blink state ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT``!``LF`                                                      |
| Response: | `-``LF`                                                          |
| Result:   | Current cursor blink state (off) is returned.                    |

#### Example: turn on cursor ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `$` `+` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Cursor is blinking.                                              |

#### Example: turn off cursor ####
|           |                                                                  |
|-----------|------------------------------------------------------------------|
| Request:  | `HT` `$` `-` `LF`                                                |
| Response: | `LF`                                                             |
| Result:   | Cursor is not blinking.                                          |


## Other links ##

* [Index](README.md)

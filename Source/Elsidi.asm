;Josip Medved <jmedved@jmedved.com> http://www.jmedved.com

;2008-08-26: Initial version.
;2008-12-01: Input is buffered.
;            Escape character is SO (0x0E).
;2008-01-14: Changed reading of busy flag.
;            Changed pinout.
;2010-07-11: Removed CTS.
;            Added/changed new commands (CR, FF, SO, SI)
;2010-08-07: Changed reset procedure in case of USART error.


;Pin configuration
; 1 RA2             LCD RS
; 2 RA3             LCD R/W
; 3 RA4             LCD E1
; 4 MCLR            Reset
; 5 VSS             Ground
; 6 RB0
; 7 RX              Built-in USART
; 8 TX              Built-in USART
; 9 RB3             LCD E2
;10 RB4             LCD DB7
;11 RB5             LCD DB6
;12 RB6             LCD DB5
;13 RB7             LCD DB4
;14 VDD             +5V
;15 RA6             LCD DB3
;16 RA7             LCD DB2
;17 RA0             LCD DB1
;18 RA1             LCD DB0


;-----------------------------------------------------------------------

    LIST            P=16F628A, F=INHX8M
    INCLUDE         "p16f628a.inc"
    ERRORLEVEL      -202
    ERRORLEVEL      -302
;    __CONFIG        _BODEN_ON & _CP_ON & _INTRC_OSC_NOCLKOUT & _LVP_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_ON    ;BrownOutDetectResetEnable & CodeProtectionEnable & InternalClockOscillator & LowVoltageProgrammingDisable & MclrEnable & PowerUpTimerEnable & WatchdogTimerEnable
    __CONFIG        _BODEN_ON & _CP_OFF & _INTRC_OSC_NOCLKOUT & _LVP_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_ON    ;BrownOutDetectResetEnable & CodeProtectionDisable & InternalClockOscillator & LowVoltageProgrammingDisable & MclrEnable & PowerUpTimerEnable & WatchdogTimerEnable

;-----------------------------------------------------------------------

;I/O bits and masks
#DEFINE             LCD_RS_TRIS         TRISA
#DEFINE             LCD_RS_PORT         PORTA
#DEFINE             LCD_RS_BIT          2
#DEFINE             LCD_RW_TRIS         TRISA
#DEFINE             LCD_RW_PORT         PORTA
#DEFINE             LCD_RW_BIT          3
#DEFINE             LCD_E1_TRIS         TRISA
#DEFINE             LCD_E1_PORT         PORTA
#DEFINE             LCD_E1_BIT          4
#DEFINE             LCD_E2_TRIS         TRISB
#DEFINE             LCD_E2_PORT         PORTB
#DEFINE             LCD_E2_BIT          3
#DEFINE             LCD_DB0_TRIS        TRISA
#DEFINE             LCD_DB0_PORT        PORTA
#DEFINE             LCD_DB0_BIT         1
#DEFINE             LCD_DB1_TRIS        TRISA
#DEFINE             LCD_DB1_PORT        PORTA
#DEFINE             LCD_DB1_BIT         0
#DEFINE             LCD_DB2_TRIS        TRISA
#DEFINE             LCD_DB2_PORT        PORTA
#DEFINE             LCD_DB2_BIT         7
#DEFINE             LCD_DB3_TRIS        TRISA
#DEFINE             LCD_DB3_PORT        PORTA
#DEFINE             LCD_DB3_BIT         6
#DEFINE             LCD_DB4_TRIS        TRISB
#DEFINE             LCD_DB4_PORT        PORTB
#DEFINE             LCD_DB4_BIT         7
#DEFINE             LCD_DB5_TRIS        TRISB
#DEFINE             LCD_DB5_PORT        PORTB
#DEFINE             LCD_DB5_BIT         6
#DEFINE             LCD_DB6_TRIS        TRISB
#DEFINE             LCD_DB6_PORT        PORTB
#DEFINE             LCD_DB6_BIT         5
#DEFINE             LCD_DB7_TRIS        TRISB
#DEFINE             LCD_DB7_PORT        PORTB
#DEFINE             LCD_DB7_BIT         4

#DEFINE             USART_SPBRG_1200    .207                            ;1200 -0.1% (1201.92 @4MHz)
#DEFINE             USART_SPBRG_2400    .103                            ;2400 -0.1% (2403.85 @4MHz)
#DEFINE             USART_SPBRG_4800    .51                             ;4800 -0.1% (4807.69 @4MHz)
#DEFINE             USART_SPBRG_9600    .25                             ;9600 -0.2% (9615.38 @4MHz)
#DEFINE             USART_SPBRG_19200   .12                             ;19200 -0.2% (19230.77 @4MHz)

#DEFINE             BUFFER_START_0      0x0020
#DEFINE             BUFFER_END_0        0x0070
#DEFINE             BUFFER_START_1      0x00A0
#DEFINE             BUFFER_END_1        0x00F0
#DEFINE             BUFFER_START_2      0x0120
#DEFINE             BUFFER_END_2        0x0150

#DEFINE             FLAG_EXECUTE_INSTRUCTION  0
#DEFINE             FLAG_SECONDARY_DISPLAY    1
#DEFINE             FLAG_BUFFER_LITERAL       0

;-----------------------------------------------------------------------

    ORG 0x2100      ;--- EEPROM DATA ---
                    DE          0x45, 0x6C, 0x73, 0x69, 0x64, 0x69, 0x20, 0x20, 0x5B, 0x72, 0x65, 0x76, 0x47, 0x5D               ;Elsidi  [revG]
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x77, 0x77, 0x77, 0x2E, 0x6A, 0x6D, 0x65, 0x64, 0x76, 0x65, 0x64, 0x2E, 0x63, 0x6F, 0x6D         ;www.jmedved.com
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x32, 0x30, 0x31, 0x30, 0x2D, 0x30, 0x37, 0x2D, 0x31, 0x39                                       ;2010-07-19        (last modification)

                    DE          0x0E                                    ;SO (switch to secondary display)

                    DE          0x45, 0x6C, 0x73, 0x69, 0x64, 0x69, 0x20, 0x20, 0x5B, 0x72, 0x65, 0x76, 0x47, 0x5D, 0x20, 0x2A   ;Elsidi  [revG] *
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x77, 0x77, 0x77, 0x2E, 0x6A, 0x6D, 0x65, 0x64, 0x76, 0x65, 0x64, 0x2E, 0x63, 0x6F, 0x6D         ;www.jmedved.com
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x09                                                                                             ;HT                (next line)
                    DE          0x32, 0x30, 0x31, 0x30, 0x2D, 0x30, 0x37, 0x2D, 0x31, 0x39                                       ;2010-07-19        (last modification)

                    DE          0x0A                                                                                             ;Execute

;-----------------------------------------------------------------------

    CBLOCK 0x70     ;--- RAM ---                                        ;common for all banks
                    backupW_WAIT
                    backupW_PROCESS_BYTE
                    backupW_EXECUTE_BYTE
                    backupW_WAIT_NONBUSY
                    iWaitLoop0_WAIT
                    iWaitLoop1_WAIT
                    iEepromAddress
                    bufferFlags
                    executeFlags
                    currLine
                    dbData
                    bufferIndex0
                    bufferIndex1
                    bufferIndex2
                    iBufferIndex
    ENDC

;-----------------------------------------------------------------------

    ORG 0x0000      ;--- RESET VECTOR ---
                    GOTO        STARTUP

;-----------------------------------------------------------------------

    ORG 0x0004      ;--- INTERUPT VECTOR ---
                    RETFIE                                              ;return from interrupt

;-----------------------------------------------------------------------

    ORG 0x0010      ;--- PROGRAM ---
STARTUP
                    ;Interrupt settings
                    BCF         INTCON, GIE                             ;disable all interrupts

                    ;Setting watchdog (33023A: 11.6.1 Switching Prescaler Assignment)
                    CLRWDT
                    CLRF        TMR0                                    ;clear TMR0 and prescaler
                    BSF         STATUS, RP0                             ;bank 1
                    BSF         OPTION_REG, PSA                         ;TIMER0 is used for WDT
                    BSF         OPTION_REG, PS0                         ;Set longest possible time (2.3 s = 128 * 18 ms)
                    BSF         OPTION_REG, PS1                         ;Set longest possible time (2.3 s = 128 * 18 ms)
                    BSF         OPTION_REG, PS2                         ;Set longest possible time (2.3 s = 128 * 18 ms)
                    BCF         STATUS, RP0                             ;bank 0
                    CLRWDT

                    ;Set features
                    BCF         OPTION_REG, T0CS                        ;TIMER0 is internal
                    BSF         CMCON, CM2                              ;disable comparators
                    BSF         CMCON, CM1                              ;disable comparators
                    BSF         CMCON, CM0                              ;disable comparators

                    ;Port initialisation
                    CLRF        PORTA                                   ;clear PORTA
                    CLRF        PORTB                                   ;clear PORTB
                    CALL        USART_TXD_SETUP                         ;TXD setup
                    CALL        USART_RXD_SETUP                         ;RXD setup
                    CALL        LCD_PIN_SETUP                           ;set LCD pins as output

                    ;Startup flashing
                    BCF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E1 off
                    BCF         LCD_E2_PORT, LCD_E2_BIT                 ;turn E2 off
                    CLRWDT
                    BCF         LCD_RS_PORT, LCD_RS_BIT
                    BCF         LCD_RW_PORT, LCD_RW_BIT
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    BSF         LCD_RS_PORT, LCD_RS_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_RS_PORT, LCD_RS_BIT
                    BSF         LCD_RW_PORT, LCD_RW_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_RW_PORT, LCD_RW_BIT
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    BSF         LCD_DB1_PORT, LCD_DB1_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT
                    BSF         LCD_DB2_PORT, LCD_DB2_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT
                    BSF         LCD_DB3_PORT, LCD_DB3_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT
                    BSF         LCD_DB4_PORT, LCD_DB4_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT
                    BSF         LCD_DB5_PORT, LCD_DB5_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT
                    BSF         LCD_DB6_PORT, LCD_DB6_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT
                    BSF         LCD_DB7_PORT, LCD_DB7_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BSF         LCD_RS_PORT, LCD_RS_BIT
                    BSF         LCD_RW_PORT, LCD_RW_BIT
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT
                    BSF         LCD_DB1_PORT, LCD_DB1_BIT
                    BSF         LCD_DB2_PORT, LCD_DB2_BIT
                    BSF         LCD_DB3_PORT, LCD_DB3_BIT
                    BSF         LCD_DB4_PORT, LCD_DB4_BIT
                    BSF         LCD_DB5_PORT, LCD_DB5_BIT
                    BSF         LCD_DB6_PORT, LCD_DB6_BIT
                    BSF         LCD_DB7_PORT, LCD_DB7_BIT
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT
                    CALL        WAIT_200MS
                    CLRWDT
                    BCF         LCD_RS_PORT, LCD_RS_BIT
                    BCF         LCD_RW_PORT, LCD_RW_BIT
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    CALL        WAIT_200MS

                    ;Set buffer
                    MOVLW       BUFFER_START_0
                    MOVWF       bufferIndex0                            ;Loads variable with starting address.
                    MOVLW       BUFFER_START_1
                    MOVWF       bufferIndex1                            ;Loads variable with starting address.
                    MOVLW       BUFFER_START_2
                    MOVWF       bufferIndex2                            ;Loads variable with starting address.
                    CLRF        executeFlags
                    CLRF        bufferFlags
                    CLRF        currLine                                ;Sets current line to 0.

                    ;Initialize display
                    CALL        INIT_LCD_VIEW
                    MOVLW       0x0E
                    CALL        INIT_LCD_VIEW
                    MOVLW       0x0F

                    ;Execute EEPROM
                    CLRF        iEepromAddress                          ;clears variable

STARTUP_EEPROM_READ
                    CLRWDT
                    MOVFW       iEepromAddress                          ;move iEepromAddress to W
                    CALL        EEPROM_READ                             ;read iEepromAddress from EEPROM¸
                    CALL        PROCESS_BYTE                            ;processes byte in W
                    INCF        iEepromAddress, F                       ;iEepromAddress += 1

                    XORLW       0x0A                                    ;check for LF
                    BTFSC       STATUS, Z                               ;skip next if !LF
                    GOTO        STARTUP_EEPROM_READ_DONE                ;done with EEPROM read
                    GOTO        STARTUP_EEPROM_READ                     ;next character

STARTUP_EEPROM_READ_DONE

                    MOVLW       0x0A                                    ;LF
                    CALL        USART_TXD                               ;sends feedback

MAIN
                    CLRWDT                                              ;starting point
                    CALL        USART_RXD                               ;reads next byte from serial port
                    CALL        PROCESS_BYTE                            ;processes byte in W

                    XORLW       0x0A                                    ;check for LF
                    BTFSS       STATUS, Z                               ;skip next if LF
                    GOTO        MAIN

                    MOVLW       0x0A                                    ;LF
                    CALL        USART_TXD                               ;sends feedback

                    GOTO        MAIN

;-----------------------------------------------------------------------

EEPROM_READ         ;--- Reads EEPROM at address in W, result is returned in W ---
                    BSF         STATUS, RP0                             ;bank 1
                    MOVWF       EEADR                                   ;set address
                    BSF         EECON1, RD                              ;EE Read
                    MOVF        EEDATA, W                               ;W = EEDATA

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

LCD_DB_SET         ;--- Sets DB lines to content in W ---
                    MOVWF       dbData                                  ;moves W to F
                    BTFSC       dbData, 0                               ;next line executes if bit is on
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT               ;turn output bit on
                    BTFSS       dbData, 0                               ;next line executes if bit is off
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT               ;turn output bit off
                    BTFSC       dbData, 1                               ;next line executes if bit is on
                    BSF         LCD_DB1_PORT, LCD_DB1_BIT               ;turn output bit on
                    BTFSS       dbData, 1                               ;next line executes if bit is off
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT               ;turn output bit off
                    BTFSC       dbData, 2                               ;next line executes if bit is on
                    BSF         LCD_DB2_PORT, LCD_DB2_BIT               ;turn output bit on
                    BTFSS       dbData, 2                               ;next line executes if bit is off
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT               ;turn output bit off
                    BTFSC       dbData, 3                               ;next line executes if bit is on
                    BSF         LCD_DB3_PORT, LCD_DB3_BIT               ;turn output bit on
                    BTFSS       dbData, 3                               ;next line executes if bit is off
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT               ;turn output bit off
                    BTFSC       dbData, 4                               ;next line executes if bit is on
                    BSF         LCD_DB4_PORT, LCD_DB4_BIT               ;turn output bit on
                    BTFSS       dbData, 4                               ;next line executes if bit is off
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT               ;turn output bit off
                    BTFSC       dbData, 5                               ;next line executes if bit is on
                    BSF         LCD_DB5_PORT, LCD_DB5_BIT               ;turn output bit on
                    BTFSS       dbData, 5                               ;next line executes if bit is off
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT               ;turn output bit off
                    BTFSC       dbData, 6                               ;next line executes if bit is on
                    BSF         LCD_DB6_PORT, LCD_DB6_BIT               ;turn output bit on
                    BTFSS       dbData, 6                               ;next line executes if bit is off
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT               ;turn output bit off
                    BTFSC       dbData, 7                               ;next line executes if bit is on
                    BSF         LCD_DB7_PORT, LCD_DB7_BIT               ;turn output bit on
                    BTFSS       dbData, 7                               ;next line executes if bit is off
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT               ;turn output bit off

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

LCD_PIN_SETUP       ;--- Sets all LCD lines ---
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         LCD_E1_TRIS, LCD_E1_BIT                 ;set E1 as output
                    BCF         LCD_E2_TRIS, LCD_E2_BIT                 ;set E2 as output
                    BCF         LCD_RS_TRIS, LCD_RS_BIT                 ;set RS as output
                    BCF         LCD_RW_TRIS, LCD_RW_BIT                 ;set R/W as output
                    BCF         STATUS, RP0                             ;bank 0
                                                                        ;continue to set DB as outputs - just fall down

;-----------------------------------------------------------------------

LCD_DB_OUTPUT_MODE  ;--- Sets DB lines to output ---
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         LCD_DB0_TRIS, LCD_DB0_BIT
                    BCF         LCD_DB1_TRIS, LCD_DB1_BIT
                    BCF         LCD_DB2_TRIS, LCD_DB2_BIT
                    BCF         LCD_DB3_TRIS, LCD_DB3_BIT
                    BCF         LCD_DB4_TRIS, LCD_DB4_BIT
                    BCF         LCD_DB5_TRIS, LCD_DB5_BIT
                    BCF         LCD_DB6_TRIS, LCD_DB6_BIT
                    BCF         LCD_DB7_TRIS, LCD_DB7_BIT

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

LCD_DB_INPUT_MODE   ;--- Sets DB lines to input ---
                    MOVLW       .0                                      ;Sets W to 0x00
                    CALL        LCD_DB_SET                              ;set DB0-DB7
                    BSF         STATUS, RP0                             ;bank 1
                    BSF         LCD_DB0_TRIS, LCD_DB0_BIT
                    BSF         LCD_DB1_TRIS, LCD_DB1_BIT
                    BSF         LCD_DB2_TRIS, LCD_DB2_BIT
                    BSF         LCD_DB3_TRIS, LCD_DB3_BIT
                    BSF         LCD_DB4_TRIS, LCD_DB4_BIT
                    BSF         LCD_DB5_TRIS, LCD_DB5_BIT
                    BSF         LCD_DB6_TRIS, LCD_DB6_BIT
                    BSF         LCD_DB7_TRIS, LCD_DB7_BIT

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

INIT_LCD_VIEW
                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE
                    MOVLW       0x38
                    CALL        EXECUTE_BYTE                             ;Function set (11****)

                    MOVLW       .6                                       ;longer wait (at least 4.1*2 is needed - 6 + 4)
                    CALL        WAIT

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE
                    MOVLW       0x38
                    CALL        EXECUTE_BYTE                             ;Function set (11****) - it really needs to be set second time (of four)
                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE
                    MOVLW       0x38
                    CALL        EXECUTE_BYTE                             ;Function set (11****) - it really needs to be set third time (of four)
                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x38
                    CALL        EXECUTE_BYTE                             ;Function set (111000) set 8-bit operation, 2-line display and 5x8 font

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x08
                    CALL        EXECUTE_BYTE                             ;Display off (1000) - turn off display

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x01
                    CALL        EXECUTE_BYTE                             ;clear screen

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x06
                    CALL        EXECUTE_BYTE                             ;Entry mode set (110)

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x14
                    CALL        EXECUTE_BYTE                             ;sets both address and shift incrementing by one

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x02
                    CALL        EXECUTE_BYTE                             ;Return home (10)

                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Instruction mode
                    MOVLW       0x0C
                    CALL        EXECUTE_BYTE                             ;Display control (1100) - turn on display

                    BANKSEL     0
                    RETURN;

;-----------------------------------------------------------------------

PROCESS_BYTE        ;--- Process byte in W ---
                    MOVWF       backupW_PROCESS_BYTE                    ;saves W

                    BTFSC       bufferFlags, FLAG_BUFFER_LITERAL        ;is next character literal?
                    GOTO        PROCESS_BYTE_LITERAL                    ;take next char as literal

                    XORLW       0x08
                    BTFSC       STATUS, Z                               ;BS (0x08)?
                    GOTO        PROCESS_BYTE_DONE                       ;Reserved character, ignore.
                    XORLW       0x08                                    ;Restore W

                    XORLW       0x0A
                    BTFSC       STATUS, Z                               ;LF (0x0A)?
                    GOTO        PROCESS_BYTE_LF
                    XORLW       0x0A                                    ;Restore W

                    XORLW       0x0B
                    BTFSC       STATUS, Z                               ;VT (0x0B)?
                    GOTO        PROCESS_BYTE_DONE                       ;Reserved character, ignore.
                    XORLW       0x0B                                    ;Restore W

                    XORLW       0x0C
                    BTFSC       STATUS, Z                               ;FF (0x0C)?
                    BSF         bufferFlags, FLAG_BUFFER_LITERAL        ;next character is to be copied directly
                    XORLW       0x0C                                    ;Restore W

                    GOTO        PROCESS_BYTE_SAVE                       ;Save byte

PROCESS_BYTE_LITERAL
                    BCF         bufferFlags, FLAG_BUFFER_LITERAL        ;buffer is in normal mode
                    GOTO        PROCESS_BYTE_SAVE                       ;save next character directly to buffer since we are in instruction mode

PROCESS_BYTE_SAVE
                    MOVFW       bufferIndex0       
                    XORLW       BUFFER_END_0
                    BTFSS       STATUS, Z                               ;W(bufferIndex0) != BUFFER_END_0
                    GOTO        PROCESS_BYTE_SAVE_0

                    MOVFW       bufferIndex1       
                    XORLW       BUFFER_END_1
                    BTFSS       STATUS, Z                               ;W(bufferIndex1) != BUFFER_END_1
                    GOTO        PROCESS_BYTE_SAVE_1

                    MOVFW       bufferIndex2       
                    XORLW       BUFFER_END_2
                    BTFSS       STATUS, Z                               ;W(bufferIndex2) != BUFFER_END_2
                    GOTO        PROCESS_BYTE_SAVE_2

                    GOTO        PROCESS_BYTE_DONE                       ;buffer overflow happened

PROCESS_BYTE_SAVE_0
                    BANKSEL     0
                    MOVF        bufferIndex0, W
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex0, F                         ;Moves buffer pointer one place forward
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_SAVE_1
                    BANKSEL     0
                    MOVF        bufferIndex1, W
                    BANKISEL    BUFFER_START_1
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex1, F                         ;Moves buffer pointer one place forward
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_SAVE_2
                    BANKSEL     0
                    MOVF        bufferIndex2, W
                    BANKISEL    BUFFER_START_2
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex2, F                         ;Moves buffer pointer one place forward
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_LF                                                         ;executes content of buffer
                    MOVFW       bufferIndex0
                    XORLW       BUFFER_START_0
                    BTFSC       STATUS, Z                               ;W(bufferIndex0) == BUFFER_START_0
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_LF_0
                    BANKSEL     0
                    MOVLW       BUFFER_START_0
                    MOVWF       iBufferIndex                            ;put buffer index at start

PROCESS_BYTE_LF_0_NEXT
                    XORLW       BUFFER_END_0
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == BUFFER_END_0
                    GOTO        PROCESS_BYTE_LF_1

                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    XORWF       bufferIndex0, W
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == bufferIndex0
                    GOTO        PROCESS_BYTE_LF_DONE

                    MOVFW       iBufferIndex                            ;W = iBufferIndex

                    BANKISEL    BUFFER_START_0
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       INDF                                    ;Load data from INDF
                    CALL        EXECUTE_BYTE                            ;Executes byte in W
                    INCF        iBufferIndex, F                         ;next character
                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    GOTO        PROCESS_BYTE_LF_0_NEXT

PROCESS_BYTE_LF_1
                    BANKSEL     0
                    MOVLW       BUFFER_START_1
                    MOVWF       iBufferIndex                            ;put buffer index at start

PROCESS_BYTE_LF_1_NEXT
                    XORLW       BUFFER_END_1
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == BUFFER_END_2
                    GOTO        PROCESS_BYTE_LF_2

                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    XORWF       bufferIndex1, W
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == bufferIndex1
                    GOTO        PROCESS_BYTE_LF_DONE

                    MOVFW       iBufferIndex                            ;W = iBufferIndex

                    BANKISEL    BUFFER_START_1
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       INDF                                    ;Load data from INDF
                    CALL        EXECUTE_BYTE                            ;Executes byte in W
                    INCF        iBufferIndex, F                         ;next character
                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    GOTO        PROCESS_BYTE_LF_1_NEXT

PROCESS_BYTE_LF_2
                    BANKSEL     0
                    MOVLW       BUFFER_START_2
                    MOVWF       iBufferIndex                            ;put buffer index at start

PROCESS_BYTE_LF_2_NEXT
                    XORLW       BUFFER_END_2
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == BUFFER_END_2
                    GOTO        PROCESS_BYTE_LF_DONE

                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    XORWF       bufferIndex2, W
                    BTFSC       STATUS, Z                               ;W(iBufferIndex) == bufferIndex2
                    GOTO        PROCESS_BYTE_LF_DONE

                    MOVFW       iBufferIndex                            ;W = iBufferIndex

                    BANKISEL    BUFFER_START_2
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       INDF                                    ;Load data from INDF
                    CALL        EXECUTE_BYTE                            ;Executes byte in W
                    INCF        iBufferIndex, F                         ;next character
                    MOVFW       iBufferIndex                            ;W = iBufferIndex
                    GOTO        PROCESS_BYTE_LF_2_NEXT

PROCESS_BYTE_LF_DONE
                    BANKSEL     0
                    MOVLW       BUFFER_START_0
                    MOVWF       bufferIndex0                            ;Loads variable with starting address.
                    MOVLW       BUFFER_START_1
                    MOVWF       bufferIndex1                            ;Loads variable with starting address.
                    MOVLW       BUFFER_START_2
                    MOVWF       bufferIndex2                            ;Loads variable with starting address.

                    CLRF        executeFlags
                    CLRF        bufferFlags
                    CLRF        currLine                                ;Sets current line to 0.

                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_DONE
                    MOVFW       backupW_PROCESS_BYTE                    ;restores W
                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

EXECUTE_BYTE        ;--- Executes byte in W ---
                    MOVWF       backupW_EXECUTE_BYTE                    ;saves W

                    BTFSC       executeFlags, FLAG_EXECUTE_INSTRUCTION
                    GOTO        EXECUTE_BYTE_INSTRUCTION                ;isInInstructionMode != 0

                    XORLW       0x09                                    ;HT (Next line).
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_HT                       ;W == 0x09
                    XORLW       0x09

                    XORLW       0x0C                                    ;FF
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_FF                       ;W == 0x0C
                    XORLW       0x0C

                    XORLW       0x0D                                    ;CR
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_CR                       ;W == 0x0D
                    XORLW       0x0D

                    XORLW       0x0E                                    ;SO
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_SO                       ;W == 0x0E
                    XORLW       0x0E

                    XORLW       0x0F                                    ;SI
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_SI                       ;W == 0x0F
                    XORLW       0x0F

                    GOTO        EXECUTE_BYTE_DATA

EXECUTE_BYTE_INSTRUCTION
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    BCF         executeFlags, FLAG_EXECUTE_INSTRUCTION  ;next byte will not be in instruction mode
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    MOVLW       .10
                    CALL        WAIT
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_HT   ;Next line
                    INCF        currLine, F                             ;increment current line
                    MOVFW       currLine                                ;move current line to W

                    XORLW       .1
                    BTFSC       STATUS, Z                               ;check whether W was 1
                    GOTO        EXECUTE_SWITCH_HT_1
                    XORLW       .1

                    XORLW       .2
                    BTFSC       STATUS, Z                               ;check whether W was 2
                    GOTO        EXECUTE_SWITCH_HT_2                     ;W == 0x02
                    XORLW       .2

                    XORLW       .3
                    BTFSC       STATUS, Z                               ;check whether W was 2
                    GOTO        EXECUTE_SWITCH_HT_3                     ;W == 0x03
                    XORLW       .3

                    MOVLW       .4                                      ;line was beyond 3, set to 4
                    MOVWF       currLine                                ;
                    GOTO        EXECUTE_BYTE_DONE                       ;

EXECUTE_SWITCH_HT_1 ;line 1
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVLW       0x80                                    ;W = 0x80 (set DDRAM address)
                    IORLW       0x40                                    ;W = 0x80 || 0x40
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    MOVLW       .2
                    CALL        WAIT
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_HT_2 ;line 2
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVLW       0x80                                    ;W = 0x80 (set DDRAM address)
                    IORLW       0x14                                    ;W = 0x80 || 0x14
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    MOVLW       .2
                    CALL        WAIT
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_HT_3 ;line 3
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVLW       0x80                                    ;W = 0x80 (set DDRAM address)
                    IORLW       0x54                                    ;W = 0x80 || 0x54
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    MOVLW       .2
                    CALL        WAIT
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_FF   ;Instruction mode
                    BSF         executeFlags, FLAG_EXECUTE_INSTRUCTION  ;go into instruction mode
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_CR   ;Return home
                    CLRF        currLine                                ;Sets current line to 0.
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVLW       0x02                                    ;W = 0x02 (return home)
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    MOVLW       .5
                    CALL        WAIT
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_SO   ;Select secondary display
                    BSF         executeFlags, FLAG_SECONDARY_DISPLAY    ;secondary display will be used
                    CLRF        currLine                                ;Sets current line to 0.
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_SWITCH_SI   ;Select primary display
                    BCF         executeFlags, FLAG_SECONDARY_DISPLAY    ;secondary display will be used
                    CLRF        currLine                                ;Sets current line to 0.
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_BYTE_DATA
                    BSF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to data
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    CALL        LCD_SEND                                ;set DB0-DB7 and toggles E
                    GOTO        EXECUTE_BYTE_DONE

EXECUTE_BYTE_DONE
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

LCD_SEND            ;Sends data to LCD
                    ;sets DB lines to content in W
                    MOVWF       dbData                                  ;moves W to F
                    BTFSC       dbData, 0                               ;next line executes if bit is on
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT               ;turn output bit on
                    BTFSS       dbData, 0                               ;next line executes if bit is off
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT               ;turn output bit off
                    BTFSC       dbData, 1                               ;next line executes if bit is on
                    BSF         LCD_DB1_PORT, LCD_DB1_BIT               ;turn output bit on
                    BTFSS       dbData, 1                               ;next line executes if bit is off
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT               ;turn output bit off
                    BTFSC       dbData, 2                               ;next line executes if bit is on
                    BSF         LCD_DB2_PORT, LCD_DB2_BIT               ;turn output bit on
                    BTFSS       dbData, 2                               ;next line executes if bit is off
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT               ;turn output bit off
                    BTFSC       dbData, 3                               ;next line executes if bit is on
                    BSF         LCD_DB3_PORT, LCD_DB3_BIT               ;turn output bit on
                    BTFSS       dbData, 3                               ;next line executes if bit is off
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT               ;turn output bit off
                    BTFSC       dbData, 4                               ;next line executes if bit is on
                    BSF         LCD_DB4_PORT, LCD_DB4_BIT               ;turn output bit on
                    BTFSS       dbData, 4                               ;next line executes if bit is off
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT               ;turn output bit off
                    BTFSC       dbData, 5                               ;next line executes if bit is on
                    BSF         LCD_DB5_PORT, LCD_DB5_BIT               ;turn output bit on
                    BTFSS       dbData, 5                               ;next line executes if bit is off
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT               ;turn output bit off
                    BTFSC       dbData, 6                               ;next line executes if bit is on
                    BSF         LCD_DB6_PORT, LCD_DB6_BIT               ;turn output bit on
                    BTFSS       dbData, 6                               ;next line executes if bit is off
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT               ;turn output bit off
                    BTFSC       dbData, 7                               ;next line executes if bit is on
                    BSF         LCD_DB7_PORT, LCD_DB7_BIT               ;turn output bit on
                    BTFSS       dbData, 7                               ;next line executes if bit is off
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT               ;turn output bit off

                    BTFSS       executeFlags, FLAG_SECONDARY_DISPLAY    ;next line executes if bit is off
                    CALL        LCD_SEND_TOGGLE_E1
                    BTFSC       executeFlags, FLAG_SECONDARY_DISPLAY    ;next line executes if bit is on
                    CALL        LCD_SEND_TOGGLE_E2

                    BANKSEL     0
                    RETURN

LCD_SEND_TOGGLE_E1  ;pulses enable
                    BSF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E on
                    NOP                                                 ;let some breathing room for slower LCDs.
                    BCF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E off
                    BANKSEL     0
                    RETURN

LCD_SEND_TOGGLE_E2  ;pulses enable
                    BSF         LCD_E2_PORT, LCD_E2_BIT                 ;turn E on
                    NOP                                                 ;let some breathing room for slower LCDs.
                    BCF         LCD_E2_PORT, LCD_E2_BIT                 ;turn E off
                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

USART_RXD_SETUP     ;--- Setup RXD ---
                    BSF         STATUS, RP0                             ;bank 1
                    BSF         TRISB, PORTB                            ;set RX as input
                    MOVLW       USART_SPBRG_9600                        ;9600 bps (@4MHz)
                    MOVWF       SPBRG
                    BSF         TXSTA, BRGH                             ;sets transmit BRGH (high speed mode)
                    BCF         TXSTA, SYNC                             ;asynchronous transmit mode
                    BCF         STATUS, RP0                             ;bank 0
                    BSF         RCSTA, SPEN                             ;receive enabled
                    BSF         RCSTA, CREN                             ;enable continous receive

                    BANKSEL     0
                    RETURN;

;-----------------------------------------------------------------------

USART_RXD          ;--- Blocking read from USART into W ---
                    CLRWDT

                    BANKSEL     PIR1
                    BTFSS       PIR1, RCIF                              ;test RX flag status
                    GOTO        USART_RXD                               ;loop while nothing is waiting

                    BANKSEL     RCSTA
                    BTFSC       RCSTA, FERR                             ;test framing error flag
                    GOTO        USART_RXD_RESET                         ;go to error clearing
                    BTFSC       RCSTA, OERR                             ;test overrun error flag
                    GOTO        USART_RXD_RESET                         ;go to error clearing

                    BANKSEL     RCREG
                    MOVFW       RCREG                                   ;read data into W

                    BANKSEL     0
                    RETURN

USART_RXD_RESET
                    BANKSEL     RCSTA
                    BCF         RCSTA, CREN                             ;disable continous receive, also clears errors
                    BSF         RCSTA, CREN                             ;enable continous receive

                    BANKSEL     RCREG
                    MOVFW       RCREG                                   ;read data into W
                    MOVFW       RCREG                                   ;read data into W
                    MOVFW       RCREG                                   ;read data into W (to clear FIFO)

                    BANKSEL     RCSTA
                    BCF         RCSTA, SPEN                             ;Disable USART.
                    BSF         RCSTA, SPEN                             ;Enable USART

                    GOTO        USART_RXD                               ;Loop some more.

;-----------------------------------------------------------------------

USART_TXD_SETUP     ;--- Setup TXD ---
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         TRISB, 2                                ;set TX as output
                    MOVLW       USART_SPBRG_9600                        ;9600 bps (@4MHz)
                    MOVWF       SPBRG
                    BSF         TXSTA, BRGH                             ;sets transmit BRGH (high speed mode)
                    BCF         TXSTA, SYNC                             ;asynchronous transmit mode
                    BCF         STATUS, RP0                             ;bank 0
                    BSF         RCSTA, SPEN                             ;receive enabled
                    BSF         STATUS, RP0                             ;bank 1
                    BSF         TXSTA, TXEN                             ;transmit enabled, sets TXIF also
                    BCF         STATUS, RP0                             ;bank 0

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

USART_TXD        ;--- Blocking write from W to USART ---
                    BANKSEL     PIR1
USART_TXD_WAIT_START
                    CLRWDT
                    BTFSS       PIR1, TXIF                              ;test TX buffer flag status 
                    GOTO        USART_TXD_WAIT_START                    ;loop until buffer clear for new data

                    BANKSEL     TXREG
                    MOVWF       TXREG                                   ;write character in W to TXREGister

                    BANKSEL     TXSTA
USART_TXD_WAIT_DONE
                    CLRWDT
                    BTFSS       TXSTA, TRMT                             ;test whether tx is complete    
                    GOTO        USART_TXD_WAIT_DONE                     ;wait until byte is sent

                    BANKSEL     0
                    RETURN

;-----------------------------------------------------------------------

WAIT                ;--- Waits for number of ms in W ---                ;delays for number of ms defined in W - based on 4000 KHz clock - 1 instruction: 4/4000, = 0.001 ms
                    MOVWF       iWaitLoop1_WAIT                         ;total delay is ((3W + 2) * 0.001)  + (W * 0.998)
WAIT_LOOP_1
                    MOVLW       .102                                    ;1   ;total of (6n + 2) * 0.001 ms per loop n=166: 0.998 ms per loop      
                    MOVWF       iWaitLoop0_WAIT                         ;1          
WAIT_LOOP_0
                    CLRWDT                                              ;1*n
                    NOP                                                 ;1*n
                    NOP                                                 ;1*n
                    DECFSZ      iWaitLoop0_WAIT, F                      ;1*(n-1) + 2
                    GOTO        WAIT_LOOP_0                             ;2*(n-1)    ;skiped if zero

                    DECFSZ      iWaitLoop1_WAIT, F
                    GOTO        WAIT_LOOP_1                             ;skipped if zero

                    RETURN                                              

;-----------------------------------------------------------------------

WAIT_200MS          ;--- Delay of 200 ms ---                            ;duration: 200.209 ms (0.007 + 200.202)
                    MOVWF       backupW_WAIT                            ;0.001 ms * 1
                    MOVLW       .200                                    ;0.001 ms * 1
                    CALL        WAIT                                    ;0.001 ms * 2
                    MOVFW       backupW_WAIT                            ;0.001 ms * 1
                    RETURN                                              ;0.001 ms * 2

;-----------------------------------------------------------------------

WAIT_NONBUSY        ;--- Waits until LCD executes command ---
                    MOVLW       .5
                    BTFSS       LCD_RS_PORT, LCD_RS_BIT
                    CALL        WAIT
                    RETURN

                    MOVWF       backupW_WAIT_NONBUSY
                    CALL        LCD_DB_INPUT_MODE                       ;set DB to input
                    MOVLW       .10                                     ;wait for things to settle
                    CALL        WAIT
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BSF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to read
WAIT_NONBUSY_LOOP                    
                    CLRWDT
                    BSF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E on
                    BTFSC       LCD_DB7_PORT, LCD_DB7_BIT               ;check busy status
                    GOTO        WAIT_NONBUSY_LOOP                       ;loop while busy
                    BCF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E off
                    CALL        LCD_DB_OUTPUT_MODE                      ;resume normal DB
                    MOVFW       backupW_WAIT_NONBUSY
                    RETURN

;-----------------------------------------------------------------------

                    END

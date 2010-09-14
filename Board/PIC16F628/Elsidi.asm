;Josip Medved <jmedved@jmedved.com> http://www.jmedved.com

;2008-08-26: Initial version.
;2008-12-01: Input is buffered.
;            Escape character is SO (0x0E).
;2008-01-14: Chnaged reading of busy flag


;Pin configuration
; 1 RA2             LCD DB7
; 2 RA3             LCD DB5
; 3 RA4
; 4 MCLR            Reset
; 5 VSS             Ground
; 6 RB0             CTS
; 7 RX              Built-in USART
; 8 TX              Built-in USART
; 9 RB3             LCD DB6
;10 RB4             LCD DB0
;11 RB5             LCD R/W
;12 RB6             LCD DB2
;13 RB7             LCD DB4
;14 VDD             +5V
;15 RA6             LCD RS
;16 RA7             LCD E
;17 RA0             LCD DB1
;18 RA1             LCD DB3


;-----------------------------------------------------------------------

    LIST            P=16F628A, F=INHX8M
    INCLUDE         "p16f628a.inc"
    ERRORLEVEL      -202
    ERRORLEVEL      -302
    __CONFIG        _BODEN_ON & _CP_ON & _INTRC_OSC_NOCLKOUT & _LVP_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_ON    ;BrownOutDetectResetEnable & CodeProtectionEnable & InternalClockOscillator & LowVoltageProgrammingDisable & MclrEnable & PowerUpTimerEnable & WatchdogTimerEnable
;    __CONFIG        _BODEN_ON & _CP_OFF & _INTRC_OSC_NOCLKOUT & _LVP_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_ON    ;BrownOutDetectResetEnable & CodeProtectionEnable & InternalClockOscillator & LowVoltageProgrammingDisable & MclrEnable & PowerUpTimerEnable & WatchdogTimerEnable

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

#DEFINE             USART_CTS_TRIS      TRISB
#DEFINE             USART_CTS_PORT      PORTB
#DEFINE             USART_CTS_BIT       0

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

;-----------------------------------------------------------------------

    ORG 0x2100      ;--- EEPROM DATA ---
                    DE          0x0E, 0x38                              ;set 8-bit operation, 2-line display and 5x8 font
                    DE          0x0E, 0x0C                              ;turn on display
                    DE          0x0E, 0x14                              ;sets both address and shift incrementing by one
                    DE          0x0E, 0x01                              ;clear screen
                    DE          0x45, 0x6C, 0x73, 0x69, 0x64, 0x69, 0x20, 0x72, 0x33, 0x20, 0x30, 0x39, 0x30, 0x31                           ;Elsidi r3 0901
                    DE          0x0E, 0xA8, 0x77, 0x77, 0x77, 0x2E, 0x6A, 0x6D, 0x65, 0x64, 0x76, 0x65, 0x64, 0x2E, 0x63, 0x6F, 0x6D         ;www.jmedved.com
;                   DE          0x41, 0x6E, 0x69, 0x74, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x4A, 0x6F, 0x73, 0x69, 0x70               ;Anita      Josip
;                   DE          0x0E, 0xA8, 0x41, 0x6E, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x47, 0x61, 0x62, 0x72, 0x69, 0x65, 0x6C   ;Ana      Gabriel
                    DE          0x0E, 0x02                              ;returns home
                    DE          0x0A

;-----------------------------------------------------------------------

    CBLOCK 0x70     ;--- RAM ---
                    backupW_WAIT
                    backupW_PROCESS_BYTE
                    backupW_EXECUTE_BYTE
                    backupW_CTS
                    backupW_WAIT_NONBUSY
                    iWaitLoop0_WAIT
                    iWaitLoop1_WAIT
                    iEepromAddress
					isBufferInCopyNextCharacterMode
                    isInInstructionMode
                    isInLiteralMode
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
                    CALL        USART_CTS_SETUP                         ;CTS setup
                    CALL        USART_TXD_SETUP                         ;TXD setup
                    CALL        USART_RXD_SETUP                         ;RXD setup
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         LCD_E1_TRIS, LCD_E1_BIT                 ;set E as output
                    BCF         LCD_RS_TRIS, LCD_RS_BIT                 ;set RS as output
                    BCF         LCD_RW_TRIS, LCD_RW_BIT                 ;set R/W as output
                    BCF         STATUS, RP0                             ;bank 0
                    CALL        LCD_DB_OUTPUT_MODE                      ;set DB0-DB7 as output

                    ;Startup flashing
                    ;BSF         LCD_E_PORT, LCD_E_BIT                  ;turn E on
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
                    BCF         LCD_RS_PORT, LCD_RS_BIT
                    BSF         LCD_RW_PORT, LCD_RW_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_RW_PORT, LCD_RW_BIT
                    BSF         LCD_DB0_PORT, LCD_DB0_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB0_PORT, LCD_DB0_BIT
                    BSF         LCD_DB1_PORT, LCD_DB1_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB1_PORT, LCD_DB1_BIT
                    BSF         LCD_DB2_PORT, LCD_DB2_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB2_PORT, LCD_DB2_BIT
                    BSF         LCD_DB3_PORT, LCD_DB3_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB3_PORT, LCD_DB3_BIT
                    BSF         LCD_DB4_PORT, LCD_DB4_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB4_PORT, LCD_DB4_BIT
                    BSF         LCD_DB5_PORT, LCD_DB5_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB5_PORT, LCD_DB5_BIT
                    BSF         LCD_DB6_PORT, LCD_DB6_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB6_PORT, LCD_DB6_BIT
                    BSF         LCD_DB7_PORT, LCD_DB7_BIT
                    CALL        WAIT_200MS
                    BCF         LCD_DB7_PORT, LCD_DB7_BIT
                    CALL        WAIT_200MS
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
                    CLRF        isInInstructionMode
                    CLRF        isInLiteralMode
					CLRF        isBufferInCopyNextCharacterMode

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
                    BCF         STATUS, RP0                             ;bank 0
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
                    RETURN

;-----------------------------------------------------------------------

LCD_DB_INPUT_MODE  ;--- Sets DB lines to input ---
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
                    BCF         STATUS, RP0                             ;bank 0
                    RETURN

;-----------------------------------------------------------------------

LCD_DB_OUTPUT_MODE ;--- Sets DB lines to output ---
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         LCD_DB0_TRIS, LCD_DB0_BIT
                    BCF         LCD_DB1_TRIS, LCD_DB1_BIT
                    BCF         LCD_DB2_TRIS, LCD_DB2_BIT
                    BCF         LCD_DB3_TRIS, LCD_DB3_BIT
                    BCF         LCD_DB4_TRIS, LCD_DB4_BIT
                    BCF         LCD_DB5_TRIS, LCD_DB5_BIT
                    BCF         LCD_DB6_TRIS, LCD_DB6_BIT
                    BCF         LCD_DB7_TRIS, LCD_DB7_BIT
                    BCF         STATUS, RP0                             ;bank 0
                    RETURN

;-----------------------------------------------------------------------

PROCESS_BYTE        ;--- Process byte in W ---
                    MOVWF       backupW_PROCESS_BYTE                    ;saves W

                    BTFSS       isBufferInCopyNextCharacterMode, 0      ;if buffer is not in instruction mode - continue with special characters check
                    GOTO        PROCESS_BYTE_CHECK

					CALL        PROCESS_BYTE_SAVE                       ;save next character directly to buffer since we are in instruction mode
                    BCF         isBufferInCopyNextCharacterMode, 0      ;buffer is in normal mode
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_CHECK
                    XORLW       0x08
                    BTFSC       STATUS, Z                               ;BS (0x08)?
                    GOTO        PROCESS_BYTE_RESERVED
                    XORLW       0x08                                    ;Restore W

                    XORLW       0x09
                    BTFSC       STATUS, Z                               ;HT (0x09)?
                    GOTO        PROCESS_BYTE_RESERVED
                    XORLW       0x09                                    ;Restore W

                    XORLW       0x0A
                    BTFSC       STATUS, Z                               ;LF (0x0A)?
                    GOTO        PROCESS_BYTE_LF
                    XORLW       0x0A                                    ;Restore W

                    XORLW       0x0B
                    BTFSC       STATUS, Z                               ;VT (0x0B)?
                    GOTO        PROCESS_BYTE_RESERVED
                    XORLW       0x0B                                    ;Restore W

                    XORLW       0x0C
                    BTFSC       STATUS, Z                               ;FF (0x0C)?
                    GOTO        PROCESS_BYTE_RESERVED
                    XORLW       0x0C                                    ;Restore W

                    XORLW       0x0D
                    BTFSC       STATUS, Z                               ;CR (0x0D)?
                    GOTO        PROCESS_BYTE_RESERVED
                    XORLW       0x0D                                    ;Restore W

                    XORLW       0x0E
                    BTFSC       STATUS, Z                               ;SO (0x0E)?
                    GOTO        PROCESS_BYTE_SO
                    XORLW       0x0E                                    ;Restore W

                    XORLW       0x0F
                    BTFSC       STATUS, Z                               ;SI (0x0F)?
                    GOTO        PROCESS_BYTE_SI
                    XORLW       0x0F                                    ;Restore W

                    CALL        PROCESS_BYTE_SAVE                       ;Just a normal byte
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

                    CLRF        isInInstructionMode
                    CLRF        isInLiteralMode
					CLRF        isBufferInCopyNextCharacterMode

                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_SO                                                         ;activates instruction mode for next character
                    BSF         isBufferInCopyNextCharacterMode, 0      ;next character is to be copied directly
                    CALL        PROCESS_BYTE_SAVE                       ;just save it, we will deal with it within execution phase
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_SI                                                         ;activates instruction mode for next character
                    BSF         isBufferInCopyNextCharacterMode, 0      ;next character is to be copied directly
                    CALL        PROCESS_BYTE_SAVE                       ;just save it, we will deal with it within execution phase
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_RESERVED                                                   ;reserved for future use
                    GOTO        PROCESS_BYTE_DONE

PROCESS_BYTE_DONE
                    MOVFW       backupW_PROCESS_BYTE                    ;restores W
                    RETURN


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

                    RETURN                                              ;buffer overflow happened

PROCESS_BYTE_SAVE_0
                    BANKSEL     0
                    MOVF        bufferIndex0, W
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex0, F                         ;Moves buffer pointer one place forward
                    RETURN

PROCESS_BYTE_SAVE_1
                    BANKSEL     0
                    MOVF        bufferIndex1, W
                    BANKISEL    BUFFER_START_1
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex1, F                         ;Moves buffer pointer one place forward
                    RETURN

PROCESS_BYTE_SAVE_2
                    BANKSEL     0
                    MOVF        bufferIndex2, W
                    BANKISEL    BUFFER_START_2
                    MOVWF       FSR                                     ;Current location goes into FSR
                    MOVFW       backupW_PROCESS_BYTE
                    MOVWF       INDF                                    ;Save data into INDF
                    INCF        bufferIndex2, F                         ;Moves buffer pointer one place forward
                    RETURN

;-----------------------------------------------------------------------

EXECUTE_BYTE        ;--- Executes byte in W ---
                    MOVWF       backupW_EXECUTE_BYTE                    ;saves W

                    BTFSC       isInLiteralMode, 0
                    GOTO        EXECUTE_BYTE_DATA                       ;isInLiteralMode != 0

                    BTFSC       isInInstructionMode, 0
                    GOTO        EXECUTE_BYTE_INSTRUCTION                ;isInInstructionMode != 0

                    XORLW       0x0E                                    ;SO
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_SO                       ;W == 0x0E
                    XORLW       0x0E

                    XORLW       0x0F                                    ;SI
                    BTFSC       STATUS, Z                               ;skip next if escape is detected 
                    GOTO        EXECUTE_SWITCH_SI                       ;W == 0x0F
                    XORLW       0x0F

                    GOTO        EXECUTE_BYTE_DATA

EXECUTE_SWITCH_SO
                    BSF         isInInstructionMode, 0                  ;go into instruction mode
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    RETURN

EXECUTE_SWITCH_SI
                    BSF         isInLiteralMode, 0                      ;go into literal mode
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    RETURN

EXECUTE_BYTE_DATA
                    BSF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to data
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    CALL        LCD_DB_SET                              ;set DB0-DB7
                    CALL        TOGGLE_E                                ;execute
                    BCF         isInLiteralMode, 0                      ;next byte will not be in literal mode
                    CALL        WAIT_NONBUSY
                    RETURN
                    
EXECUTE_BYTE_INSTRUCTION
                    BCF         LCD_RS_PORT, LCD_RS_BIT                 ;set RS to command
                    BCF         LCD_RW_PORT, LCD_RW_BIT                 ;set RW to write
                    MOVFW       backupW_EXECUTE_BYTE                    ;restores W
                    CALL        LCD_DB_SET                              ;set DB0-DB7
                    CALL        TOGGLE_E                                ;execute
                    BCF         isInInstructionMode, 0                    ;next byte will not be in instruction mode
                    CALL        WAIT_NONBUSY
                    RETURN

;-----------------------------------------------------------------------

TOGGLE_E            ;Pulses enable
                    BSF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E on
                    NOP                                                 ;let some breathing room for slower LCDs.
                    BCF         LCD_E1_PORT, LCD_E1_BIT                 ;turn E off
                    RETURN

;-----------------------------------------------------------------------

USART_CTS_SETUP     ;--- Sets CTS line---
                    BSF         STATUS, RP0                             ;bank 1
                    BCF         USART_CTS_TRIS, USART_CTS_BIT           ;set CTS as output
                    BCF         STATUS, RP0                             ;bank 0
                    BCF         USART_CTS_PORT, USART_CTS_BIT           ;turns off CTS until ready
                    RETURN;

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
                    RETURN;

;-----------------------------------------------------------------------

USART_RXD          ;--- Blocking read from USART into W ---
                    BSF         USART_CTS_PORT, USART_CTS_BIT           ;sets CTS
USART_RXD_LOOP
                    CLRWDT

                    BANKSEL     PIR1
                    BTFSS       PIR1, RCIF                              ;test RX flag status
                    GOTO        USART_RXD_LOOP                          ;loop while nothing is waiting

                    BANKSEL     RCSTA
                    BTFSC       RCSTA, FERR                             ;test framing error flag
                    GOTO        USART_RXD_LOOP                          ;just go to next valid byte (if any)
                    BTFSC       RCSTA, OERR                             ;test overrun error flag
                    GOTO        USART_RXD_ERROR                         ;go to error clearing

                    BANKSEL     RCREG
                    MOVFW       RCREG                                   ;read data into W

                    BANKSEL     0
                    BCF         USART_CTS_PORT, USART_CTS_BIT           ;clear CTS, we are processing data
                    RETURN
USART_RXD_ERROR
                    BCF         RCSTA, CREN                             ;disable continous receive, also clears errors
                    BSF         RCSTA, CREN                             ;enable continous receive
                    GOTO        USART_RXD_LOOP                          ;loop some more

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

WAIT_1MS            ;--- Delay of 1 ms ---                              ;duration: 1.010 ms (0.007 + 5.007)
                    MOVWF       backupW_WAIT                            ;0.001 ms * 1
                    MOVLW       .1                                      ;0.001 ms * 1
                    CALL        WAIT                                    ;0.001 ms * 2
                    MOVFW       backupW_WAIT                            ;0.001 ms * 1
                    RETURN                                              ;0.001 ms * 2

;-----------------------------------------------------------------------

WAIT_5MS            ;--- Delay of 5 ms ---                              ;duration: 5.014 ms (0.007 + 1.003)
                    MOVWF       backupW_WAIT                            ;0.001 ms * 1
                    MOVLW       .5                                      ;0.001 ms * 1
                    CALL        WAIT                                    ;0.001 ms * 2
                    MOVFW       backupW_WAIT                            ;0.001 ms * 1
                    RETURN                                              ;0.001 ms * 2

;-----------------------------------------------------------------------

WAIT_200MS          ;--- Delay of 200 ms ---                            ;duration: 200.209 ms (0.007 + 200.202)
                    MOVWF       backupW_WAIT                            ;0.001 ms * 1
                    MOVLW       .200                                    ;0.001 ms * 1
                    CALL        WAIT                                    ;0.001 ms * 2
                    MOVFW       backupW_WAIT                            ;0.001 ms * 1
                    RETURN                                              ;0.001 ms * 2

;-----------------------------------------------------------------------

WAIT_NONBUSY        ;--- Waits until LCD executes command ---
                    MOVLW       .10
                    CALL        WAIT
                    RETURN

                    MOVWF       backupW_WAIT_NONBUSY
                    CALL        LCD_DB_INPUT_MODE                       ;set DB to input
                    MOVLW       .1                                      ;wait for things to settle
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

#include "p18f8722.inc"
; CONFIG1H
  CONFIG  OSC = HSPLL, FCMEN = OFF, IESO = OFF
; CONFIG2L
  CONFIG  PWRT = OFF, BOREN = OFF, BORV = 3
; CONFIG2H
  CONFIG  WDT = OFF, WDTPS = 32768
; CONFIG3L
  CONFIG  MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
; CONFIG3H
  CONFIG  CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF, MCLRE = ON
; CONFIG4L
  CONFIG  STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
; CONFIG5L
  CONFIG  CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CP4 = OFF, CP5 = OFF
  CONFIG  CP6 = OFF, CP7 = OFF
; CONFIG5H
  CONFIG  CPB = OFF, CPD = OFF
; CONFIG6L
  CONFIG  WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRT4 = OFF
  CONFIG  WRT5 = OFF, WRT6 = OFF, WRT7 = OFF
; CONFIG6H
  CONFIG  WRTC = OFF, WRTB = OFF, WRTD = OFF
; CONFIG7L
  CONFIG  EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTR4 = OFF
  CONFIG  EBTR5 = OFF, EBTR6 = OFF, EBTR7 = OFF
; CONFIG7H
  CONFIG  EBTRB = OFF
  
  
	    UDATA_ACS
t1		res 1	; used in delay
t2		res 1	; used in delay
t3	        res 1	; used in delay

which_pins	udata 0X22
which_pins
		
ORG     0x00
goto    main
 
init 
    CLRF    PORTA
    CLRF    PORTB
    CLRF    PORTC
    CLRF    PORTD
    CLRF    PORTE
    CLRF    FSR0
    CLRF    INDF0
    CLRF    WREG
    CLRF    TRISB
    CLRF    TRISC
    CLRF    TRISD
    CLRF t1
    CLRF t2
    CLRF t3
    MOVLW   b'00011111'
    MOVWF   ADCON1
    MOVLW   b'00010000'
    MOVWF   TRISA
    clrf    LATE
    MOVLW   b'00011000'
    MOVWF   TRISE
    movlw b'00001111'
    movwf LATA
    movwf LATB
    movwf LATC
    movwf LATD
  
    movlw b'00000000'
    movwf which_pins
    
    lfsr FSR0, 0F88h
    clrf INDF0    
return
    
DELAY	; Time Delay Routine with 3 nested loops
    MOVLW 20	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0xCF  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0xFA	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return

DELAY2	; Time Delay Routine with 3 nested loops
    MOVLW 80	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop23:
	MOVLW 0xCF  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop22:
	    MOVLW 0xFA	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop21:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop21 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop22 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop23 ; ELSE Keep counting down
		return
main:
    call init
    call DELAY2
    CLRF LATA
    CLRF LATB
    CLRF LATC
    CLRF LATD
    call DELAY
    call state1
     
state1
    bsf STATUS, 0
    loop1: 
	movlw 08Ch
	CPFSEQ FSR0L
        goto loop2
	call WaitforRA4
    loop2:
        btfsc PORTA,4
	call RA4pressed
        MOVF POSTINC0
        btfsc which_pins, 3
	CLRF which_pins
    loop3:
        bsf STATUS, 0
        RLCF which_pins
	movf which_pins,0
        movwf INDF0
	call DELAY
	btfsc which_pins, 3
	goto loop1
	btfss PORTA,4
	goto loop3
	call RA4pressed
	
WaitforRA4
    btfss PORTA,4
    goto WaitforRA4
    call RA4pressed
    
RA4pressed
    btfsc PORTA,4
    goto RA4pressed
    call state2

state2
    btfsc PORTE,3
    call re3pressed
    btfsc PORTE,4
    call re4pressed
    goto state2
    
re3pressed
    btfsc PORTE,3
    goto re3pressed
    MOVF POSTDEC0
    goto state1
  
re4pressed
    btfsc PORTE,4
    goto re4pressed
    MOVF POSTINC0
    call state3
 
state3
    bcf STATUS, 0 
    loop1_3: 
        MOVF POSTDEC0
	movlw 088h
	CPFSEQ FSR0L
        goto loop2_3
	call WaitforRA4
    loop2_3:   
        btfsc PORTA,4
	call RA4pressed
        btfsc which_pins, 0
	goto loop3_3
	movlw b'00001111'
        movwf which_pins
    loop3_3:
        btfss which_pins, 0
	goto loop1_3
	RRCF which_pins
	BCF which_pins, 7
	movf which_pins, 0
	movwf INDF0
        call DELAY
	btfss PORTA,4
	goto loop3_3
        call RA4pressed
end
    
    

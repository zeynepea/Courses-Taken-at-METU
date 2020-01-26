; Zeynep ERDO?AN 2171577
; Ay?enur BÜLBÜL 2171403
    
LIST    P=18F8722

#INCLUDE <p18f8722.inc> 
    
    config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF
wheretf_sp equ 0x22
tim1L equ 0x23    
tim1H equ 0x24
onvar equ 0x25
counter equ 0x26
 keeperA equ 0x27
 keeperB equ 0x28
 keeperC equ 0x29
 keeperD equ 0x30
 keeperE equ 0x31
 keeperF equ 0x32
 
 laserB equ 0x33
 laserC equ 0x34
 laserD equ 0x35
 laserE equ 0x36
 laserF equ 0x37
 
 astrA equ 0x38
 astrB equ 0x39
 astrC equ 0x40
 astrD equ 0x41
 astrE equ 0x42
 astrF equ 0x43
 timon equ 0x21
 timyiot equ 0x44
 timyiel equ 0x45
 score0 equ 0x46
 sckeeper equ 0x47
 score1 equ 0x48
 score2 equ 0x49
 score3 equ 0x50
 L1 equ 0x51
 L2 equ 0x52
 
 
ORG 0X00
GOTO init
ORG 0X08


timer_int:
      incf counter
      movlw d'10'
      cpfseq timon
      goto delay500
      movlw d'20'
      cpfseq timyiot
      goto delay400
      movlw d'20'
      cpfseq timyiel
      goto delay300
      goto delay200
      delay500:
	movff counter, WREG
	sublw d'100'
	btfsc STATUS, Z
	goto cont
	goto timer_exit
      delay400:
	movff counter, WREG
	sublw d'80'
	btfsc STATUS, Z
	goto cont
	goto timer_exit
      delay300:
	movff counter, WREG
	sublw d'60'
	btfsc STATUS, Z
	goto cont
	goto timer_exit
      delay200:
	movff counter, WREG
	sublw d'40'
	btfsc STATUS, Z
	goto cont
	goto timer_exit
      cont:
      goto dojob
  
      
     
timer_exit:
      bcf     INTCON, 2           ;Clear TMROIF
      movlw   d'61'               ;256-61=195; 195*256*5 = 249600 instruction cycle;
      movwf   TMR0
      retfie  FAST
      
dojob:     
     call compare_dims
     call shift_astr
     call compare_dims
     call shift_laser
     call compare_dimsor
     call create_ast
     call light_ast
     call ast_shift
     clrf counter     
     goto timer_exit
     
Table1:
    ADDWF PCL
    RETLW b'00111111' ;0
    RETLW b'00000110' ;1
    RETLW b'01011011' ;2
    RETLW b'01001111' ;3
    RETLW b'01100110' ;4
    RETLW b'01101101' ;5
    RETLW b'01111101' ;6
    RETLW b'00000111' ;7
    RETLW b'01111111' ;8
    RETLW b'01100111' ;9
init
    ;Disable interrupts
    clrf    INTCON
    clrf    INTCON2

    ;Configure Input/Interrupttim1L Ports
    movlw   b'00010000' ; w_reg =  b'00010000'
    movwf   TRISB   ; TRISB = =w_reg = b'00010000' 
    bcf     INTCON2, 7  ;Pull-ups are enabled - clear INTCON2<7>
    clrf    PORTB

    ;Initialize Timer0
    movlw   b'01000111' ;Disable Timer0 by setting TMR0ON to 0 (for now)
                        ;Configure Timer0 as an 8-bit timer/counter by setting T08BIT to 1
                        ;Timer0 increment from internal clock with a prescaler of 1:256.
    movwf   T0CON ; T0CON = b'01000111'

    
    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1
    clrf LATA
    clrf PORTA
    clrf LATB
    clrf PORTB
    clrf LATC
    clrf PORTC
    clrf LATD
    clrf PORTD
    clrf LATE
    clrf PORTE
    clrf LATF
    clrf PORTF
    clrf wheretf_sp
    clrf counter
    clrf STATUS
    clrf PORTG
    clrf TRISG
    clrf LATG
    clrf LATH
    clrf LATJ
    clrf PORTH
    clrf PORTJ
    clrf TRISH
    clrf TRISJ
    
    clrf keeperF
    clrf keeperE
    clrf keeperD
    clrf keeperC
    clrf keeperB
    clrf keeperA
    clrf astrF
    clrf astrE
    clrf astrD
    clrf astrC
    clrf astrB
    clrf astrA
    clrf laserF
    clrf laserE
    clrf laserD
    clrf laserC
    clrf laserB
    clrf timon
    clrf timyiot
    clrf timyiel
    clrf score0
    clrf score1
    clrf score2
    clrf score3
    clrf sckeeper
    clrf L1
    clrf L2
  
    
    movlw b'00000000' 
    movwf TRISA
    movwf TRISB
    movwf TRISC
    movwf TRISD
    movwf TRISE
    movwf TRISF
    
    movlw b'001111'
    movwf TRISG
    
    movlw b'00011111'
    movwf ADCON1
    
    
    bsf T1CON,0
    ;timer1 16-bit
    bsf T1CON,7
    bsf T1CON,6
    clrf tim1L
    clrf tim1H
    clrf onvar
    
    movlw	d'61'               ;256-61=195; 195*256*5 = 249600 instruction cycle;
    movwf	TMR0
    
    loop:
      btfss PORTG,0
      goto loop
      nex:
      btfsc PORTG,0
      goto nex
      movlw b'000001000'
      movwf wheretf_sp
      movff wheretf_sp, PORTA
      movff TMR1L,tim1L
      movff TMR1H,tim1H
      ;Enable interrupts
    movlw   b'11100000' ;Enable Global, peripheral, Timer0 and RB interrupts by setting GIE, PEIE, TMR0IE and RBIE bits to 1
    movwf   INTCON

      ;call create_ast
      goto main

main
    call display
    call move_up
    call move_down
    call fire_laser
    call display
    goto main
  
    
move_up
      btfss PORTG,3
      return
      next:
      btfsc PORTG,3
      goto next
      btfsc wheretf_sp,0
      return
      rrncf wheretf_sp
      return

    
move_down
      btfss PORTG,2
      return
      next2:
      btfsc PORTG,2
      goto next2
      btfsc wheretf_sp,5
      return
      rlncf wheretf_sp
      return
      
fire_laser
      btfss PORTG,1
      return
      next3:
      btfsc PORTG,1
      goto next3
      movff wheretf_sp, laserB
      call comp_b
      return
      
create_ast
      movlw d'10'
      cpfseq timon
      goto incto
      movlw d'20'
      cpfseq timyiot
      goto incyiot
      movlw d'20'
      cpfseq timyiel
      goto incyiel
      goto devam
      incto:
	incf timon
	goto devam
      incyiot:
	incf timyiot
	goto devam
      incyiel:
	incf timyiel
	goto devam
	
devam:
      movlw d'10'
      cpfseq onvar
      goto create
      goto complement
      complement:
      clrf onvar
      comf tim1L
      comf tim1H
      create:
      incf onvar
      btfss tim1L,0
      goto __0
      goto __1
	__0:
	    btfss tim1L,1
	    goto _00
	    goto _10
	__1:
	    btfss tim1L,1
	    goto _01
	    goto _11
	_00:
	    btfss tim1L,2
	    goto _000
	    goto _100
	_10:
	    btfss tim1L,2
	    goto _010
	    goto _110
	_01:
	    btfss tim1L,2
	    goto _001
	    goto _101
	_11:
	    btfss tim1L,2
	    goto _011
	    goto _111
	_000:
	    bsf astrF,0
	    return
	_001:
	    bsf astrF,1
	    return
	_010:
	    bsf astrF,2
	    return
	_011:
	    bsf astrF,3
	    return
	_100:
	    bsf astrF,4
	    return
	_101:
	    bsf astrF,5
	    return
	_110:
	    bsf astrF,0
	    return
	_111:
	    bsf astrF,1
	    return
light_ast
	movf laserF, 0
	xorwf astrF,0
	movwf LATF
	return
ast_shift	    
	    btfss tim1H,0
	    goto h0l_
	    goto h1l_
	    h0l_:
		btfss tim1L,0
		goto h0l0
		goto h0l1
	    h1l_:
		btfss tim1L,0
		goto h1l0
		goto h1l1
	    h0l0:
		bcf STATUS,0
		rrcf tim1L
		rrcf tim1H
		return
	    h1l1:
		bsf STATUS,0
		rrcf tim1L
		rrcf tim1H
		return
	    h0l1:
		bcf STATUS,0
		rrcf tim1L
		bsf STATUS,0
		rrcf tim1H
		return
	    h1l0:
		bcf STATUS,0
		rrcf tim1H
		bsf STATUS,0
		rrcf tim1L
		return
		
   shift_astr
        clrf keeperA
	clrf keeperB
	clrf keeperC
	clrf keeperD
	clrf keeperE
	clrf keeperF
	movff astrA,keeperA
        movff astrB,keeperB
	movff astrC,keeperC
	movff astrD,keeperD
	movff astrE,keeperE
	movff astrF,keeperF
	clrf astrF
	movff keeperF,astrE
	movff keeperE,astrD
	movff keeperD,astrC
	movff keeperC,astrB
	movff keeperB,astrA

	return
	
    shift_laser
        clrf keeperA
	clrf keeperB
	clrf keeperC
	clrf keeperD
	clrf keeperE
	clrf keeperF
        movff laserB,keeperB
	movff laserC,keeperC
	movff laserD,keeperD
	movff laserE,keeperE
	movff laserF,keeperF
	clrf laserB
	movff keeperB,laserC
	movff keeperC,laserD
	movff keeperD,laserE
	movff keeperE,laserF
	return
    
compare_dims
	call comp_a
	call comp_b
	call comp_c
	call comp_d
	call comp_e
	call comp_f
	return
	
comp_b
	movff astrB, sckeeper
	movf laserB,0
	xorwf astrB,0
	movwf LATB
	andwf astrB
	andwf laserB
	movf sckeeper,0
	cpfseq astrB
	goto noteq
	return
	

comp_c
	movff astrC, sckeeper
	movf laserC,0
	xorwf astrC,0
	movwf LATC
	andwf astrC
	andwf laserC
	movf sckeeper,0
	cpfseq astrC
	goto noteq
	return
	
comp_d
	movff astrD,sckeeper
	movf laserD,0
	xorwf astrD,0
	movwf LATD
	andwf astrD
	andwf laserD
	movf sckeeper,0
	cpfseq astrD
	goto noteq
	return
	
comp_e
	movff astrE,sckeeper
	movf laserE,0 
	xorwf astrE,0
	movwf LATE
	andwf astrE
	andwf laserE
	movf sckeeper,0
	cpfseq astrE
	goto noteq
	return
	
comp_f
	movff astrF,sckeeper
	movf laserF, 0
	xorwf astrF,0
	movwf LATF
	andwf astrF
	andwf laserF
	movf sckeeper,0
	cpfseq astrF
	goto noteq
	return
	
    noteq:
      movlw d'09'
      cpfseq score0
      goto s0
      cpfseq score1
      goto s1
      cpfseq score2
      goto s2
      goto s3
      s0:
        incf score0
	return
      s1:
        incf score1
	clrf score0
	return
      s2:
        incf score2
	clrf score1
	clrf score0
	return
      s3:
        incf score3
	clrf score2
	clrf score1
	clrf score0
	return

display
    call display0
    call bigDELAY
    call display1
    call bigDELAY
    call display2
    call bigDELAY
    call display3
    call bigDELAY
    return
display0 
    movlw   b'00001000' 
    movwf   LATH
    MOVF score0,0
    ADDWF score0,0
    call Table1
    movwf LATJ,0
    return
display1    
    movlw   b'00000100' 
    movwf   LATH
    MOVF score1,0
    ADDWF score1,0
    call Table1
    movwf LATJ,0
    return
display2     
    movlw   b'00000010' 
    movwf   LATH
    MOVF score2,0
    ADDWF score2,0
    call Table1
    movwf LATJ,0
    return
display3     
    movlw   b'00000001' 
    movwf   LATH
    MOVF score3,0
    ADDWF score3,0
    call Table1
    movwf LATJ,0
    return
    
 DELAY                            ; Time Delay Routines
     MOVLW 50                        ; Copy 50 to W
     MOVWF L2                    ; Copy W into L2
 LOOP2
     MOVLW 255                   ; Copy 255 into W
     MOVWF L1                    ; Copy W into L1
 LOOP1
     decfsz L1,F                    ; Decrement L1. If 0 Skip next instruction
         GOTO LOOP1                ; ELSE Keep counting down
     decfsz L2,F                    ; Decrement L2. If 0 Skip next instruction
         GOTO LOOP2                ; ELSE Keep counting down
     return   
    

 bigDELAY
        CALL DELAY
        return   
	

compare_dimsor
	call comp_bor
	call comp_cor
	call comp_dor
	call comp_eor
	call comp_for
	return
	
comp_bor
	movf laserB,0
	iorwf astrB,0
	movwf LATB
	andwf astrB
	andwf laserB
	return

comp_cor
	movf laserC,0
	iorwf astrC,0
	movwf LATC
	andwf astrC
	andwf laserC
	return
comp_dor
	movf laserD,0
	iorwf astrD,0
	movwf LATD
	andwf astrD
	andwf laserD
	return
comp_eor
	movf laserE,0 
	iorwf astrE,0
	movwf LATE
	andwf astrE
	andwf laserE
	return
comp_for
	movf laserF, 0
	iorwf astrF,0
	movwf LATF
	andwf astrF
	andwf laserF
	return
	
comp_a       ;for porta
test0:
       btfss astrA,0
       goto sp00
       goto sp01
        sp00:
	   btfss wheretf_sp,0
	   goto ara0
	   bsf PORTA,0
	   goto test1
	ara0:
           bcf PORTA,0
	   goto test1
	sp01:
	   btfss wheretf_sp,0
	   goto break0
	   call theEND
	break0:
           bsf PORTA,0
	   goto test1
test1:
       btfss astrA,1
       goto sp10
       goto sp11
        sp10:
	   btfss wheretf_sp,1
	   goto ara1
	   bsf PORTA,1
	   goto test2
	ara1:
           bcf PORTA,1
	   goto test2
	sp11:
	   btfss wheretf_sp,1
	   goto break1
	   call theEND
	break1:
           bsf PORTA,1
	   goto test2
	   
test2:
       btfss astrA,2
       goto sp20
       goto sp21
        sp20:
	   btfss wheretf_sp,2
	   goto ara2
	   bsf PORTA,2
	   goto test3
	ara2:
           bcf PORTA,2
	   goto test3
	sp21:
	   btfss wheretf_sp,2
	   goto break2
	   call theEND
	break2:
           bsf PORTA,2
	   goto test3
	  
test3:
       btfss astrA,3
       goto sp30
       goto sp31
        sp30:
	   btfss wheretf_sp,3
	   goto ara3
	   bsf PORTA,3
	   goto test4
	ara3:
           bcf PORTA,3
	   goto test4
	sp31:
	   btfss wheretf_sp,3
	   goto break3
	   call theEND
	break3:
           bsf PORTA,3
	   goto test4
	   
test4:
       btfss astrA,4
       goto sp40
       goto sp41
        sp40:
	   btfss wheretf_sp,4
	   goto ara4
	   bsf PORTA,4
	   goto test5
	ara4:
           bcf PORTA,4
	   goto test5
	sp41:
	   btfss wheretf_sp,4
	   goto break4
	   call theEND
	break4:
           bsf PORTA,4
	   goto test5
	   
test5:
       btfss astrA,5
       goto sp50
       goto sp51
        sp50:
	   btfss wheretf_sp,5
	   goto ara5
	   bsf PORTA,5
	   return
	ara5:
           bcf PORTA,5
	   return
	sp51:
	   btfss wheretf_sp,5
	   goto ret
	   call theEND
	ret:
	    bsf PORTA,5
	    return

theEND
	    call init
	    
END      






/*/* 
 * Zeynep ERDO?AN 2171577
 * Ay?enur BÜLBÜL 2171403
 *
 * In this project there are 4 interrupts.
 * 
 * First one is Timer0 which is in line 319. 
 * In every 1.5 ms timer0 calls seven segment display and every time it updates(also may remain same) a different digit. 
 * This way instead of writing a delay function for seven segment display we have used timer0.
 * In every 100 ms timer0 starts adc convertion.
 * In every 250 ms timer0 sets a blink flag whichwe check in our main function.
 * 
 * Second on is Timer1 which is in line 354
 * In every 1000 ms timer1 decrements 90 sec and sets a finish flag if time is up which we check in main function.
 * 
 * Third one is RB6-7 interrupt which is in line 307
 * This first check if it is RB6 or RB7 then calls that action function accordingly.
 * 
 * The last one is ADC interrrupt which is in line 349
 * This interrupt sets adc_done flag which we handle in main function and then set to 0 afterwards. 
 */

#include <p18cxxx.h>
//#include <p18f8722.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "LCD.h"

/*
 * 
 */
int sevenseg[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67}; // lookup table for 7 Segment display
int start_flag,unit_flag,start_vote,vote_this_flag;
int button_state_RE1;
int last_portb;
int adc_done;
int ten,unit,voters;
char c,h;
char x,y;
char* id,*name;
int people[8]={0,0,0,0,0,0,0,0};
int highest_vote=0;
int highest_id;
int ikiyuz=0;
int yuz=0;
int blinkflag=0;
int tmr1=0;
int value;
int finish=0;
int count;
int i=0;


void init_table(int i){
    LATJ = 0x40;
    
    if(i==0){
       
        LATH = 0x08;
        
    }
    else if(i==1){
        
        LATH = 0x04;
    }
   
    else if(i==2){
       
    LATH = 0x02; 
    }
    
    else if(i==3){
       
        LATH = 0x01; 
    }
}
void seven_seg_display(int i){//called in timer0 interrupt
    if(highest_vote==0){
        init_table(i);
        return;
    }
    
    int m = highest_id/10;
    int n = highest_id%10;
    int k = highest_vote/10;
    int l = highest_vote%10;
    if(i==0){
        LATJ = sevenseg[l];
        LATH = 0x08;
        
    }
    else if(i==1){
        LATJ = sevenseg[k];
        LATH = 0x04;
    }
   
    else if(i==2){
       LATJ = sevenseg[n];
    LATH = 0x02; 
    }
    
    else if(i==3){
       LATJ = sevenseg[m];
        LATH = 0x01; 
    }
   
}
void display_vote(){ //display votes on led
    if(ADRES<=127){
        int a=people[0]/10;
        int b = people[0]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }
    else if(128<=ADRES && ADRES<=255){
        int a=people[1]/10;
        int b = people[1]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE);
        WriteDataToLCD(y);
    }else if(256<=ADRES && ADRES<=383){
        int a=people[2]/10;
        int b = people[2]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }else if(384<=ADRES && ADRES<=511){
        int a=people[3]/10;
        int b = people[3]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }else if(512<=ADRES && ADRES<=639){
        int a=people[4]/10;
        int b = people[4]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }else if(640<=ADRES && ADRES<=767){
        int a=people[5]/10;
        int b = people[5]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }else if(768<=ADRES && ADRES<=895){
        int a=people[6]/10;
        int b = people[6]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE);
        WriteDataToLCD(y);
    }else if(896<=ADRES && ADRES<=1023){
        int a=people[7]/10;
        int b = people[7]%10;
        x = a + '0';
        y = b + '0';
        WriteCommandToLCD(0xCD); 
        WriteDataToLCD(x);
        WriteCommandToLCD(0xCE); 
        WriteDataToLCD(y);
    }
}
void vote_this(){//increment candidate' votes dependingon adc and rb7
    if(ADRES<=127){
        people[0]++;
        if (people[0]>highest_vote){
            highest_vote = people[0];
            highest_id = 18;
        }
    }
    else if(128<=ADRES && ADRES<=255){
        people[1]++;
        if (people[1]>highest_vote){
            highest_vote = people[1];
            highest_id = 64;
        }
        
    }else if(256<=ADRES && ADRES<=383){
        people[2]++;
        if (people[2]>highest_vote){
            highest_vote = people[2];
            highest_id = 34;
        }
       
    }else if(384<=ADRES && ADRES<=511){
        people[3]++;
        if (people[3]>highest_vote){
            highest_vote = people[3];
            highest_id = 23;
        }
        
    }else if(512<=ADRES && ADRES<=639){
        people[4]++;
        if (people[4]>highest_vote){
            highest_vote = people[4];
            highest_id = 33;
        }
        
    }else if(640<=ADRES && ADRES<=767){
        people[5]++;
        if (people[5]>highest_vote){
            highest_vote = people[5];
            highest_id = 67;
        }
        
    }else if(768<=ADRES && ADRES<=895){
        people[6]++;
        if (people[6]>highest_vote){
            highest_vote = people[6];
            highest_id = 63;
        }
    }else if(896<=ADRES && ADRES<=1023){
        people[7]++;
        if (people[7]>highest_vote){
            highest_vote = people[7];
            highest_id = 99;
        }
    }
}

void rb6Action(){
    if(unit_flag==0){//increment tens digit
        if(ten==9){
            ten=0;
            //voters = voters;
            voters=0;
        }
            
        else{
            ten++;
            voters=voters+10;
        }
            
        WriteCommandToLCD(0xC1);  
        c=ten+'0';
        WriteDataToLCD(c);
    }
    else if(unit_flag==1){//increment unit digit
        if(unit==9){
            unit=0;
            voters=voters-9;
        }
            
        else{
            unit++;
            voters++;
        }
            
        WriteCommandToLCD(0xC2); 
        h=unit+'0';
        WriteDataToLCD(h);
    }
    
    
}
void rb7Action(){
    unit_flag++;
    if(unit_flag==2){//start voting
        start_vote=1;
        
        PIE1=0x01;
        TMR1=0x6700;
        value=90;
    }
    else if(unit_flag>2){ //vote candidates
        if(finish==0){
            if(count==voters)
            finish=1;
            else{
                count++;
                vote_this();
            }
        }
        
         
        
        
        
    }
}
void __interrupt ()  my_interrupt(void){
    
    if(RBIF==1){ //rb6-7 interrupt
        last_portb = PORTB;
        PORTB = last_portb;
        if((last_portb&0x40)==0x40){
            rb6Action();
        }
        else if((last_portb&0x80)==0x80){
            rb7Action();
        }
        RBIF = 0;
        
    }
    if(T0IF==1){ //timer0 counts 1.5 msecond
        if(start_vote){
        seven_seg_display(i);
        i++;
        if(i==4)
            i=0;}
        if(yuz<63)//to get 100ms
            yuz++;
        
        else if(yuz==63){
            PIE1bits.ADIE = 1;     // adc interrupt enable
            ADCON0bits.GO = 1;     // Set go/done bit
            ADCON0bits.ADON = 1;   // adc enable
            yuz=0;
        }
        if(ikiyuz<159) //to get 250ms
            ikiyuz++;
        else if(ikiyuz==159){
            blinkflag=(!blinkflag);
            ikiyuz=0;
        }
        
        T0IF=0;
        /*
         * 1.5ms=1500us=15000 ins cycle
         * prescale = 256
         * 255-15000/256 ~= 195
         */
        TMR0=195;  
    }
    if(ADIF==1){ //adc interrupt
        adc_done=1;
        ADIF=0;
        
    }
    if(TMR1IF==1){    //timer1 interrupt
        if(tmr1<255){ //1*256*6700*255
            tmr1++;
        }
        else{
            tmr1=0;
            if(value==0){
                finish=1;
            }
            else{
                if(finish==0)
                value--;
            }
        }
        TMR1=0x6700;
        TMR1IF=0;
    }
}


char* whichCandidate(){ //read adc value and find candidate's name
    if(ADRES<=127){
        return "Poe   ";
    }
    else if(128<=ADRES && ADRES<=255){
        return "Galib ";
    }else if(256<=ADRES && ADRES<=383){
        return "Selimi";
    }else if(384<=ADRES && ADRES<=511){
        return "Nesimi";
    }else if(512<=ADRES && ADRES<=639){
        return "Hatayi";
    }else if(640<=ADRES && ADRES<=767){
        return "Zweig ";
    }else if(768<=ADRES && ADRES<=895){
        return "Nabi  ";
    }else if(896<=ADRES && ADRES<=1023){
        return "Austen";
    }
}
char* whichID(){//read adc value and find candidate's ID
    if(ADRES<=127){
        return "18";
    }
    else if(128<=ADRES && ADRES<=255){
        return "64";
    }else if(256<=ADRES && ADRES<=383){
        return "34";
    }else if(384<=ADRES && ADRES<=511){
        return "23";
    }else if(512<=ADRES && ADRES<=639){
        return "33";
    }else if(640<=ADRES && ADRES<=767){
        return "67";
    }else if(768<=ADRES && ADRES<=895){
        return "63";
    }else if(896<=ADRES && ADRES<=1023){
        return "99";
    }
}


void init_interrupt(){
    RCONbits.IPEN = 0;  // priorities disabled
    
    // timer0
    TMR0=195; // enable timer 0 int
    T0CON = 0xC7;        // configure timer0 int
    TMR0IE = 1;        // init tmr0
    
  
    //PIE1=0x01; //enable timer1
    T1CON=0xC5;
    
    // portb
    INTCONbits.RBIE = 1;  //enable portb int
    INTCONbits.RBIF = 1;
    INTCON2 = 0x00;       

    // adc
    ADCON1 = 0x00;        // configure voltage reference
    ADCON0 = 0x30;        // select ch12
    // make right justified 
    ADCON2 = 0x8C;        //acquisition 2-Tad, sampling rate Fosc/4 -10Hz-
    ADON=1;
}
void init_port(){
    vote_this_flag=0;
    adc_done=0;
    start_vote=0;
    unit_flag=0;
    ten=0;
    unit=0;
    start_flag = 0;
    button_state_RE1=0;
    TRISE = 0x02; //re1
    TRISB = 0xC0; //rb6-7
   
    TRISH = 0x10; 
    TRISJ = 0x00; // select as output for all pins
    value=90; //90sn
    count=0; //number of voted voters
    voters=0; // total number of people that are expected to vote
    
}
void button_task_RE1(){
    switch (button_state_RE1)
        {
            case 0: 
                if (PORTEbits.RE1) {
                    button_state_RE1 = 1;
                    
                }
                break;

            case 1: 
                if (PORTEbits.RE1 == 0) {
                    button_state_RE1 = 0;
                    start_flag = 1; //start the game
                   
                }
                break;
       }
}

void main(void) {
    INTCON = 0x00;
    InitLCD();
    init_interrupt();
    init_port();
    
    PEIE=1;//enanable
    GIE=1;
    ClearLCDScreen();
    WriteCommandToLCD(0x80);//first line
    WriteStringToLCD(" #Electro Vote# ");
    WriteCommandToLCD(0xC0); // second line
    WriteStringToLCD(" ############## ");
    
    
    while(1){//wait till re1 pressed & wait 3 seconds afterwards
        button_task_RE1();
        //wait until the RE1 button pushed
        
        if(start_flag==1){
           
           
            // wait for 3 seconds
        for(int u=0; u<2130; u++){ 
            for(int i=0; i<1000; i++){
            }
         }
            ClearLCDScreen();
            WriteCommandToLCD(0x80);//first line
            WriteStringToLCD(" #Enter Voters# ");
            WriteCommandToLCD(0xC0); // second line
            WriteStringToLCD(" 00");
            break;
       }
    }
    start_flag=0; 
    
   
    while(1){//wait till the number of voters entered
        
        if(start_vote){
           ClearLCDScreen();
            WriteCommandToLCD(0x80);//first line
            WriteStringToLCD("  Time left :90 ");
            WriteCommandToLCD(0xC0); // second line
            if(blinkflag==0)
                WriteStringToLCD("  ");
            else
                WriteStringToLCD(" >");;
           
            break;
        }
    }
    
    while(1){//voting phase
        if(voters==0)
            finish=1;
        if(count==voters){
            finish=1;
        }
        if(value==0)
            finish=1;
        
        WriteCommandToLCD(0xC0); // second line
        if(finish==0){
            if(blinkflag==0)
                WriteStringToLCD("  ");
            else
                WriteStringToLCD(" >");
        }
        else if(finish==1){
            WriteStringToLCD(" >");
        }
            
        if(adc_done==1){
            id=whichID();
            name = whichCandidate();
            adc_done=0;
            WriteCommandToLCD(0xC2); // second line
            WriteStringToLCD(id);
            WriteCommandToLCD(0xC5); // second line
            WriteStringToLCD(name);
            WriteCommandToLCD(0xCC); // second line
            WriteDataToLCD(':');
            display_vote();
        }
        WriteCommandToLCD(0x8D); // second line
        int first=value/10;
        int second=value%10;
        char num1;
        char num2;
        num1=first+'0';
        num2=second+'0';
        WriteDataToLCD(num1);
        WriteCommandToLCD(0x8E);
        WriteDataToLCD(num2);
        WriteCommandToLCD(0x8F);
        WriteDataToLCD(' ');
       
    }
    
    // wait for 3 seconds
}








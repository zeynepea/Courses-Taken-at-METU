#include "common.h"
// it works correctly after loading the program you need to reload the program
//this homework we have two tasks. our second task (tsk_task1) only handles 
//receiving go command. It waits untill similator sens GO message, then sets
//an alarm called ACTIVATED_ALARM mainwhile task0 waits for this alarm.
//task0 handles recieving stop response and encoder response and sending
//Moving commands and end command. Robot follows right walls untill it explores
//entire mazes. If entire maze  is explored ourmap 2d array's all entires are 1. 
//and a variable called son is set to 1. After that robot starts to follow the left wall
//untill it reaches destination.
//between line 61-69 it checks if 2d array's entry are all 1(all map explored.)
//between line 204-315 if not all maze explored, it sends move commands following right wall
//between line 318-427 if all maze explored, it sends move commands following left wall
//between liine 85-196 it calculates obstacles locations and send MAP command according to relative
//sencor data.
//between line 433-439 it checks if all map explored and robot loc is target loc. and sends END command.

// we were supposed to update the LCD module as a second task but we run out of time so just added
// go command as a second task.
 /**********************************************************************/
char data1[40];
char data_size1;

char movef[3] = "$F:";
char turnr[3] = "$R:";
char turnl[3] = "$L:";
char stop[3] = "$S:";


int x=25,y=25;
int yon =0;

int angle=90;
int velocity=50;
int keeper=0;

int ourmap[4][16];/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 * 
 * Important announcement every 5secs :)
 * 
 **********************************************************************/
TASK(TASK0) 
{   
   // SetRelAlarm(ALARM_TSK0, 100, 80);

    
    int u,t;
    

    WaitEvent(ACTIVATE_EVENT);
    ClearEvent(ACTIVATE_EVENT);
    transmitBuffer_push(stop,3);
    data_size1 = receiveBuffer_pop(data1);
    
    for(u=0;u<4;u++){
        for(t=0;t<16;t++){
            ourmap[u][t]=0;
        }
    }
        while(1) {  
            char map[6];
            char en[5]="$END:";
            int son=0;
            int o,p;
            map[2]=0;
            map[3]=0;
            SetRelAlarm(ALARM_TSK0, 100, 40);
            for(o=0;o<4;o++){
                for(p=0;p<16;p++){
                    if(ourmap[o][p]==1)
                        keeper++;
                }
            }
            
            if(keeper==64){
                son++;
            }
            keeper=0;
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            transmitBuffer_push(stop,3);
            data_size1 = receiveBuffer_pop(data1);
           
            
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);

            map[2]=(x-25)/50;
            map[3]=(y-25)/50;
            ourmap[map[2]][map[3]]=1;
            map[0]='$';
            map[1]='M';

            map[4]=0;
            map[5]=':';

            transmitBuffer_push(map,6); //send current loc.

            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            //this switch calculates of robot's left loc.
            switch (yon) 
            {
                case 0: //robot facing down 
                    map[2]=(x-25)/50;
                    map[3]=(y-25+50)/50;
                    break;
                case 1: //robot facing right
                    map[2]=(x-25-50)/50;
                    map[3]=(y-25)/50;                       
                    break;
                case 2: //robot facing up 
                    map[2]=(x-25)/50;
                    map[3]=(y-25-50)/50;
                    break;
                case 3: //robot facing left
                    map[2]=(x-25+50)/50;
                    map[3]=(y-25)/50;
                    break;                         
            }
            map[0]='$';
            map[1]='M';

            map[4]=data1[4];
            
            map[5]=':';
            if(map[2]>=0 && map[2]<4 && map[3]>=0 && map[3]<16 ){
               
                transmitBuffer_push(map,6); //send left location
                ourmap[map[2]][map[3]]=1;
            }
            
            
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            //this switch calculates of robot's right loc.
            switch (yon)
            {
                case 0: 
                    map[2]=(x-25)/50;
                    map[3]=(y-25-50)/50;
                    break;
                case 1: 
                    map[2]=(x-25+50)/50;
                    map[3]=(y-25)/50;                       
                    break;
                case 2:
                    map[2]=(x-25)/50;
                    map[3]=(y-25+50)/50;
                    break;
                case 3: 
                    map[2]=(x-25-50)/50;
                    map[3]=(y-25)/50;
                    break;                         
            }
            map[0]='$';
            map[1]='M';

            map[4]=data1[2];
            map[5]=':';
            if(map[2]>=0 && map[2]<4 && map[3]>=0 && map[3]<16 ){
                
                transmitBuffer_push(map,6); // send right location
                ourmap[map[2]][map[3]]=1;
            }
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            //this switch calculates of robot's rear loc.
            switch (yon)
            {
                case 0: 
                    map[2]=(x-25-50)/50;
                    map[3]=(y-25)/50;
                    break;
                case 1: 
                    map[2]=(x-25)/50;
                    map[3]=(y-25-50)/50;                       
                    break;
                case 2: 
                    map[2]=(x-25+50)/50;
                    map[3]=(y-25)/50;
                    break;
                case 3: 
                    map[2]=(x-25)/50;
                    map[3]=(y-25+50)/50;
                    break;                         
            }
            map[0]='$';
            map[1]='M';

            map[4]=data1[3];
            map[5]=':';
            if(map[2]>=0 && map[2]<4 && map[3]>=0 && map[3]<16 ){
                
                transmitBuffer_push(map,6); // send rear location
                ourmap[map[2]][map[3]]=1;
            } 
            
            
            
            
            
            
            
            //if not all maze explored.(son==0) send move commands following right wall
            if ((data_size1==5)&&data1[0]=='D'&&son==0){
                if(data1[2] != 1){ //turn right move forward 
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(turnr,3);
                    data_size1 = receiveBuffer_pop(data1);
                    
                    while(angle-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(turnr,3);
                        data_size1 = receiveBuffer_pop(data1);
                        angle = angle - data1[1];
                    }
                    angle = 90;
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(movef,3);
                    data_size1 = receiveBuffer_pop(data1);
                    while(velocity-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(movef,3);
                        data_size1 = receiveBuffer_pop(data1);
                        velocity = velocity - data1[1];
                        
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    velocity = 50;
                    yon = (yon+3)%4;
                    //calculates robot loc. after moving forward
                    switch (yon)
                    {
                        case 0:
                            x=x+50;
                            break;
                        case 1:
                            y=y+50;
                            break;
                        case 2:
                            x=x-50;
                            break;
                        case 3:
                            y=y-50;
                            break;                         
                    }
                   
                }
                else if(data1[1]== 0){//move forward

                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(movef,3);
                    data_size1 = receiveBuffer_pop(data1);
                    while(velocity-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(movef,3);
                        data_size1 = receiveBuffer_pop(data1);
                        velocity = velocity - data1[1];
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    velocity = 50;
                    
                    //calculates robot loc. after moving forward
                    switch (yon)
                    {
                        case 0:
                            x=x+50;
                            break;
                        case 1:
                            y=y+50;
                            break;
                        case 2:
                            x=x-50;
                            break;
                        case 3:
                            y=y-50;
                            break;                         
                    }
                    
                }
                
                else{ //move left
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(turnl,3);
                    data_size1 = receiveBuffer_pop(data1);    
                    while(angle-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(turnl,3);
                        data_size1 = receiveBuffer_pop(data1);
                        angle = angle - data1[1];
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    angle = 90;
                    yon = (yon+1)%4;
                    
                }
                
                
            }
            //if all maze explored.(son==1) send move commands following left wall
            else if ((data_size1==5)&&data1[0]=='D'&&son==1){
                if(data1[4] != 1){  //turn left move forward
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(turnl,3);
                    data_size1 = receiveBuffer_pop(data1);
                    
                    while(angle-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(turnl,3);
                        data_size1 = receiveBuffer_pop(data1);
                        angle = angle - data1[1];
                    }
                    angle = 90;
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(movef,3);
                    data_size1 = receiveBuffer_pop(data1);
                    while(velocity-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(movef,3);
                        data_size1 = receiveBuffer_pop(data1);
                        velocity = velocity - data1[1];
                        
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    velocity = 50;
                    yon = (yon+1)%4;

                    switch (yon)
                    {
                        case 0:
                            x=x+50;
                            break;
                        case 1:
                            y=y+50;
                            break;
                        case 2:
                            x=x-50;
                            break;
                        case 3:
                            y=y-50;
                            break;                         
                    }
                   
                }
                else if(data1[1]== 0){ //move forward

                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(movef,3);
                    data_size1 = receiveBuffer_pop(data1);
                    while(velocity-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(movef,3);
                        data_size1 = receiveBuffer_pop(data1);
                        velocity = velocity - data1[1];
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    velocity = 50;
                    switch (yon)
                    {
                        case 0:
                            x=x+50;
                            break;
                        case 1:
                            y=y+50;
                            break;
                        case 2:
                            x=x-50;
                            break;
                        case 3:
                            y=y-50;
                            break;                         
                    }
                    
                }
                
                else{ //move right
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(turnr,3);
                    data_size1 = receiveBuffer_pop(data1);    
                    while(angle-data1[1]>0){
                        WaitEvent(ALARM_EVENT);
                        ClearEvent(ALARM_EVENT);
                        transmitBuffer_push(turnr,3);
                        data_size1 = receiveBuffer_pop(data1);
                        angle = angle - data1[1];
                    }
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    transmitBuffer_push(stop,3);
                    data_size1 = receiveBuffer_pop(data1);
                    angle = 90;
                    yon = (yon+3)%4;
                    
                }
                
                
            }
            
            //if all map is explored and robot loc is target loc. send end command to similator.
           if(son==1&&x==25&&y==775){
               WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);
                transmitBuffer_push(en,5);
                break;
           }             
        }
    
	TerminateTask();
}


/* End of File : tsk_task0.c */
#include "common.h"
//this task just recieves GO command and send ACTIVE_EVENT to task0


char data2[40];
char data_size2;

TASK(TASK1) 
{
    
    SetRelAlarm(ALARM_TSK1, 100, 80);
    //PIE1bits.RCIE= 1;
	PIE1bits.RC1IE= 1;
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        data_size2 = receiveBuffer_pop(data2);
        if(data_size2 != 0)
        {   
            if(data_size2==2 && data2[0] == 'G'&& data2[1] == 'O'  )  {  
                SetEvent(TASK0_ID, ACTIVATE_EVENT);
                break;}
        }
        
	}
    TerminateTask();
}

/* End of File : tsk_task1.c */
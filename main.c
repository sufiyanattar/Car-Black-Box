/*
 * File:   main.c
 * Author: Sufiyan Attar
 *
 * Created on March 28, 2024, 2:05 PM
 */



#include "main.h"


init_config()
{

    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_timer();
    init_i2c();
	init_ds1307();
    init_uart();
    PEIE = 1;
    
    /* Enabling global interrupt */
    GIE = 1;
    /*
    for(int i=0;i<4;i++)
    {
        write_external_eeprom(add++,'1');
    }
    */
    add = 110;
    for(int i=0;i<4;i++)
    {
        pass[i] = read_external_eeprom(add++);
    }
    pass[4] = '\0';
}

void main(void) {
    
    init_config();
    get_time();
    event_capture();
    
    static long int delay = 0;
    while (1)
    {
        key = read_switches(STATE_CHANGE);
        
        while (key == MK_SW11)
        {
            key = read_switches(LEVEL_CHANGE);
            delay++;
            
            if(delay > 50000)
            {
                key = MK_SW11_LONG_PRESS;
                delay = 0;
                break;
            }
        }
        
        if (delay < 50000 && delay > 30)
        {
            key = MK_SW11;
            delay = 0;
        }
        
        while (key == MK_SW12)
        {
            key = read_switches(LEVEL_CHANGE);
            delay++;
            
            if(delay > 50000)
            {
                key = MK_SW12_LONG_PRESS;
                delay = 0;
                break;
            }
        }
        
        if (delay < 50000 && delay > 30)
        {
            key = MK_SW12;
            delay = 0;
        }
        
        
        if(main_f == DASHBOARD)
        {
            dashboard();
        }
        else if(main_f == PASSWORD)
        {
            password();
        }
        else if(main_f == MENU)
        {
            menu(key);
        }
        else if(main_f == VIEWLOG)
        {
            view_log(key);
        }
        else if(main_f == CLEARLOG)
        {
            clear_log();
        }
        else if (main_f == CHANGEPASS)
        {
            change_pass(key);
        }
        else if (main_f == DOWNLOADLOG)
        {
            download_log();
        }
        else if (main_f == SETTIME)
        {
            
            settime(key);
        }
    }  
}

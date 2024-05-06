/*
 * File:   black_box.c
 * Author: Sufiyan Attar
 *
 * Created on March 28, 2024, 2:26 PM
 */


#include <xc.h>
#include "main.h"


static int i = 0;
unsigned int speed;
static address = 0x00;

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}
void dashboard()
{
    
    speed = read_adc(CHANNEL4);
    speed /= 10.25;

    static int collision_detected = 0;


    if (key == MK_SW11) 
    {
        CLEAR_DISP_SCREEN;
        main_f = PASSWORD;
        key = ALL_RELEASED;
        return;
    }

    if (key == MK_SW1) {
        if (collision_detected) 
        {
            i = 1;
            collision_detected = 0;
        } 
        else 
        {
            i++;
            if (i > 6)
                i = 6;
        }
        event_capture();
    }
    else if (key == MK_SW2)
    {
        if (collision_detected) {
            i = 1;
            collision_detected = 0;
        }
        else {
            i--;
            if (i < 0)
                i = 0;
        }
        event_capture();
    }
    else if (key == MK_SW3) 
    {
        i = 7;
        collision_detected = 1;
        event_capture();
    }
    
    clcd_print("TIME      EV  SP",LINE1(0));
    
    get_time();
    
    clcd_print(time,LINE2(0));
    clcd_print("  ",LINE2(8));
    clcd_print(gears[i],LINE2(10));
    clcd_print("  ",LINE2(12));
    clcd_putch((speed/10)+48,LINE2(14));
    clcd_putch((speed%10)+48,LINE2(15));    
}

void event_capture()
{
    ++count1;
    static unsigned char data[10];

    for (int j = 0; j < 8; j++) 
    {
        if (time[j] != ':')
            write_external_eeprom(address++, time[j]);
    }

    for (int j = 0; j < 2; j++) 
    {
        write_external_eeprom(address++, gears[i][j]);
    }

    write_external_eeprom(address++, (speed / 10) + 48);
    write_external_eeprom(address++, (speed % 10) + 48);

    if (address > 99)
        address = 0x00;
}

void password()
{
    char user_password[5];
    
    if (attempt > 0) 
    {
        clcd_print("ENTER PASSWORD", LINE1(0));
        if (count++ <= 500) 
        {
            clcd_putch('_', LINE2(k));
        }
        else if (count++ > 500 && count < 1000) 
        {
            clcd_putch(' ', LINE2(k));
        }
        else 
        {
            count = 0;
        }

        if (key == MK_SW11)
        {
            user_password[pos] = '0';
            clcd_putch('*', LINE2(k++));
            pos++;
        }

        else if (key == MK_SW12) 
        {
            user_password[pos] = '1';
            clcd_putch('*', LINE2(k++));
            pos++;
        }

        if (pos == 4) 
        {
            user_password[pos] = '\0';
            pos = 0;
            k = 5;
            if (strcmp(user_password, pass) == 0) 
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASSWORD IS", LINE1(2));
                clcd_print("MATCHED", LINE2(4));
                
                for (unsigned long int i = 500000; i--;);
                CLEAR_DISP_SCREEN;
                main_f = 2;
            }
            else
            {
                --attempt;
                CLEAR_DISP_SCREEN;
                clcd_putch(attempt + 48, LINE1(0));
                clcd_print("ATTEMPT IS", LINE1(2));
                clcd_print("LEFT", LINE2(4));
                for (unsigned long int i = 500000; i--;);
                CLEAR_DISP_SCREEN;
            }
        }
    }
    if (attempt == 0) 
    {
        attempt = 3;
        CLEAR_DISP_SCREEN;
        clcd_print("PASSWORD FAILED", LINE1(0));
        for (unsigned long int delay = 500000;delay--;);
        CLEAR_DISP_SCREEN;
        
        sec = 120;

        while(sec)
        {
            clcd_putch((sec / 100) + 48, LINE1(4));
            clcd_putch(((sec / 10) % 10) + 48, LINE1(5));
            clcd_putch((sec % 10) + 48, LINE1(6));

            clcd_print("SEC", LINE1(8));
            clcd_print("REMAINING", LINE2(3));
        }
        
        CLEAR_DISP_SCREEN;
        main_f = 0;
             
    }
}

void menu(char key)
{
    static int j=0;
    
    if (flag == 0) 
    {
        clcd_putch('*', LINE1(0));
        clcd_print(array_menu[m], LINE1(2));
        clcd_print(array_menu[m + 1], LINE2(2));
    } 
    else if (flag == 1) 
    {
        clcd_putch('*', LINE2(0));
        clcd_print(array_menu[m], LINE1(2));
        clcd_print(array_menu[m + 1], LINE2(2));
    }

    if (key == MK_SW12)
    {
        CLEAR_DISP_SCREEN;
        flag = 1;
        j++;
        
        if (j>=4)
        {
            j=4;
        }
        
        m++;
        if (once) 
        {
            m--;
            once = 0;
        }
        if (m > 3) 
        {
            m = 3;
        }
    }
    else if (key == MK_SW11) 
    {
        CLEAR_DISP_SCREEN;
        flag = 0;
        j--;
        
        if (j<=0)
        {
            j=0;
        }
        m--;
        if (!once) {
            m++;
            once = 1;
        }
        if (m < 0) {
            m = 0;
        }
    }
    
    if (key == MK_SW11_LONG_PRESS && j == 0) 
    {
        CLEAR_DISP_SCREEN;
        main_f = VIEWLOG;
        address = 0;
        key = ALL_RELEASED;
    } 
    else if (key == MK_SW11_LONG_PRESS && j == 1) 
    {
        main_f = DOWNLOADLOG;
    }
    else if (key == MK_SW11_LONG_PRESS && j == 2) 
    {
        main_f = CLEARLOG;
    }
    else if (key == MK_SW11_LONG_PRESS && j == 3)
    {
        main_f = SETTIME;
    }
    else if (key == MK_SW11_LONG_PRESS && j == 4) 
    {
        CLEAR_DISP_SCREEN;
        main_f = CHANGEPASS;
    }
    else if(key == MK_SW12_LONG_PRESS)
    {
        k=5;
        main_f = DASHBOARD;
        key = ALL_RELEASED;
        attempt = 3;
    }
    
}

void view_log(char key)
{
   static char data[10];
   static int temp_addr,no_of_events = 1;
    
    if(count1 == 0)
    {
        clcd_print("LOGS EMPTY",LINE1(0));
    }
    else if(count >= 10)
    {
        count1 = 10;
    }
    
    if ( no_of_events <= count1 && count1 > 0 ) 
    {
        clcd_print("#   TIME   EV SP", LINE1(0));
        
        if (key == MK_SW11) 
        {
            no_of_events--; 
            if (no_of_events < 1)
            {
                no_of_events = 1;
            }
            else
            {
                 if (address >= 10) 
                 {
                   address = address - 10;
                 }    
            }
        }
        else if (key == MK_SW12) 
        {
            no_of_events++;
            
            if (no_of_events > count1)
            {
                no_of_events = count1;
            }
            else
            {
                if (address <= 89)
                {
                address = address + 10;
                }
            }
        }

        temp_addr = address;

        for (int i = 0; i < 10; i++) 
        {
            data[i] = read_external_eeprom(temp_addr++);
        }

        clcd_putch((no_of_events-1) + '0', LINE2(0));
        clcd_putch(' ', LINE2(1));
        clcd_putch(data[0], LINE2(2));
        clcd_putch(data[1], LINE2(3));
        clcd_putch(':', LINE2(4));
        clcd_putch(data[2], LINE2(5));
        clcd_putch(data[3], LINE2(6));
        clcd_putch(':', LINE2(7));
        clcd_putch(data[4], LINE2(8));
        clcd_putch(data[5], LINE2(9));
        clcd_putch(' ', LINE2(10));
        clcd_putch(data[6], LINE2(11));
        clcd_putch(data[7], LINE2(12));
        clcd_putch(' ', LINE2(13));
        clcd_putch(data[8], LINE2(14));
        clcd_putch(data[9], LINE2(15));
        
        if(key == MK_SW12_LONG_PRESS)
        {
            main_f = MENU;
            key = ALL_RELEASED;
            CLEAR_DISP_SCREEN;
        }
    }
    if(key == MK_SW12_LONG_PRESS)
    {
            main_f = MENU;
            key = ALL_RELEASED;
            CLEAR_DISP_SCREEN;
    }   

    if(key == MK_SW12_LONG_PRESS)
    {
            main_f = MENU;
            key = ALL_RELEASED;
            CLEAR_DISP_SCREEN;
    }   
}

void clear_log(void)
{
    address = 0;
    count1 = 0;
    CLEAR_DISP_SCREEN;
    clcd_print("CLEARING LOG...",LINE1(0));
    for(int long delay = 500000;delay--;);
    CLEAR_DISP_SCREEN;
    clcd_print("LOGS CLEARED",LINE1(0));
    for(int long delay = 500000;delay--;);
    CLEAR_DISP_SCREEN;
    main_f = MENU;
}

void download_log()
{
    static int flag = 1;
    if(count1 == 0)
    {
        if(flag)
        {
            CLEAR_DISP_SCREEN;
            flag = 0;
        }
        clcd_print("LOGS EMPTY",LINE1(0));
        if(key == MK_SW12_LONG_PRESS)
        {
            main_f = MENU;
            key = ALL_RELEASED;
            CLEAR_DISP_SCREEN;
        }      
    }
    else
    {
        static int address = 0,k = 0,j;
        static char download_data[10];
        for(int i=0;i<count1;i++)
        {
            for(j=0;j<14;j++)
            {
                if (j == 2 || j == 5)
                {
                    download_data[j] = ':';
                }
                else if (j == 8 || j == 11)
                {
                    download_data[j] = ' ';
                }
                else
                {
                    download_data[j] = read_external_eeprom(address++);
                }
            }
            download_data[j] = '\0';
            puts(download_data);
            puts("\n\r");
            
        }
        CLEAR_DISP_SCREEN;
        clcd_print("DOWNLOADING...",LINE1(0));
        for(int long delay=500000;delay--; );
        CLEAR_DISP_SCREEN;
        clcd_print("DOWNLOADED!",LINE1(0));
        for(int long delay=500000;delay--; );
        
        main_f = DASHBOARD;
        CLEAR_DISP_SCREEN;
    }

}

void change_pass(char key)
{
    static char pass1[5],pass2[5];

    static int flag = 1;
    static int pos1 = 0,pos2=0;
    if (pos1 < 4)
    {
        clcd_print("ENTER PASSWORD", LINE1(0));
        if (count++ <= 500) 
        {
            clcd_putch('_', LINE2(p));
        }
        else if (count++ > 500 && count < 1000) 
        {
            clcd_putch(' ', LINE2(p));
        }
        else 
        {
            count = 0;
        }

        if (key == MK_SW11)
        {
            pass1[pos1] = '0';
            clcd_putch('*', LINE2(p++));
            pos1++;
        }

        else if (key == MK_SW12) 
        {
            pass1[pos1] = '1';
            clcd_putch('*', LINE2(p++));
            pos1++;
        }
    }
    else if (pos1 == 4 && pos2 < 4)
    {
        if (flag)
        {
            CLEAR_DISP_SCREEN;
            flag = 0;
        }
        clcd_print("REENTER PASSWORD", LINE1(0));
        if (count++ <= 500) 
        {
            clcd_putch('_', LINE2(q));
        }
        else if (count++ > 500 && count < 1000) 
        {
            clcd_putch(' ', LINE2(q));
        }
        else 
        {
            count = 0;
        }

        if (key == MK_SW11) 
        {
            pass2[pos2] = '0';
            clcd_putch('*', LINE2(q++));
            pos2++;
        } 
        else if (key == MK_SW12) 
        {
            pass2[pos2] = '1';
            clcd_putch('*', LINE2(q++));
            pos2++;
        } 
    }
    else
    {
        pass1[pos1] = '\0';
        
        pass2[pos2] = '\0';
        if (pos1 == 4 && pos2 == 4) 
        {
            pos1 = 0;
            pos2 = 0;
            p = 5;
            q = 5;
            flag = 1;
            if (!strcmp(pass1, pass2)) 
            {
                strcpy(pass,pass1);
                pass[5] = '\0';
                add = 110; 
                for(int i=0;i<4;i++)
                {
                    write_external_eeprom(add++,pass1[i]);
                }
                CLEAR_DISP_SCREEN;
                clcd_print("PASSWORD CHANGED", LINE1(0));
                clcd_print("SUCCESSFULLY", LINE2(0));

                for (unsigned long int delay = 500000; delay--;);
                CLEAR_DISP_SCREEN;
                main_f = MENU;
            } 
            else 
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASSWORD NOT", LINE1(0));
                clcd_print("MATCHING", LINE2(0));

                for (unsigned long int delay = 500000; delay--;);
                CLEAR_DISP_SCREEN;
                main_f = MENU;
            }
        }
    }
}

void settime(char key)
{
    static int once = 1,field = 1,delay = 0,hour = 0,min = 0,sec = 0,buf;
      
    clcd_print("HH:MM:SS",LINE1(4));
    
    delay++;
    
    if(once)
    {
        CLEAR_DISP_SCREEN;
        get_time();
        hour = ((time[0]-48)*10) + (time[1]-48);
        min = ((time[3]-48)*10) + (time[4]-48);
        sec = ((time[6]-48)*10) + (time[7]-48);
        once = 0;
    }
       
    if(key == MK_SW12)
    {
        field++;
        if(field > 3)
            field = 1;
    }
    
    if(field == 1)
    {
        
        if(delay < 500)
        {
            clcd_putch(' ',LINE2(10));
            clcd_putch(' ',LINE2(11));
        }
        else if(delay >= 500 && delay < 1000)
        {
            clcd_print(time,LINE2(4));
        }
        else
        {
            delay = 0;
        }
        
        if(key == MK_SW11)
        {
            sec++;
            if(sec > 59)
                sec = 00;
            time[7] = (sec % 10) + 48;
            time[6] = (sec / 10) + 48; 
        }
    }
    
    if(field == 2)
    {
        
        if(delay < 500)
        {
            clcd_putch(' ',LINE2(7));
            clcd_putch(' ',LINE2(8));
        }
        else if(delay >= 500 && delay < 1000)
        {
            clcd_print(time,LINE2(4));
        }
        else
        {
            delay = 0;
        }
        
        if(key == MK_SW11)
        {
            min++;
            if(min > 59)
                min = 00;
            time[4] = (min % 10) + 48;
            time[3] = (min / 10) + 48; 
        }
    }
    
    if(field == 3)
    {
        
        if(delay < 500)
        {
            clcd_putch(' ',LINE2(4));
            clcd_putch(' ',LINE2(5));
        }
        else if(delay >= 500 && delay < 1000)
        {
            clcd_print(time,LINE2(4));
        }
        else
        {
            delay = 0;
        }
        
        if(key == MK_SW11)
        {
            hour++;
            if(hour > 23)
                hour = 0;
            time[1] = (hour % 10) + 48;
            time[0] = (hour / 10) + 48; 
        }
    }
    
    if(key == MK_SW11_LONG_PRESS)  
    {
        buf = (time[0]-48) << 4 | (time[1] - 48);
        write_ds1307(HOUR_ADDR,buf);
        buf = 0;
        buf = (time[3]-48) << 4 | (time[4] - 48);
        write_ds1307(MIN_ADDR,buf);
        buf = 0;
        buf = (time[6]-48) << 4 | (time[7] - 48);
        write_ds1307(SEC_ADDR,buf);
        
        once = 1;
        CLEAR_DISP_SCREEN;
        main_f = MENU;   
    }
    else if(key == MK_SW12_LONG_PRESS)
    {
        CLEAR_DISP_SCREEN;
        main_f = MENU;
    }

    
}
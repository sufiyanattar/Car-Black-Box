#ifndef MAIN
#define MAIN

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "eeprom.h"
#include "matrix_keypad.h"
#include "timer.h"
#include <string.h>
#include "uart.h"

#define DASHBOARD               0
#define PASSWORD                1
#define MENU                    2
#define VIEWLOG                 3
#define DOWNLOADLOG             4
#define CLEARLOG                5
#define SETTIME                 6
#define CHANGEPASS              7



void dashboard();               
void event_capture();
void password();
void menu(char key);
void view_log(char key);
void download_log();
void clear_log(void);
void settime(char key);
void change_pass(char key);

void get_time();

char gears[8][3] = {"ON","GN","GR","G1","G2","G3","G4","CO"};
int key,attempt = 3;
char main_f = 0;

int k=5,p=5,q=5,pos = 0;

unsigned int wait = 0;
unsigned int count = 0;
unsigned int count1=0;

unsigned char clock_reg[3];
unsigned char time[9];

unsigned int sec;

int m=0;
char pass[5];
char array_menu[5][20] = {"VIEW LOG","DOWNLOAD LOG","CLEAR LOG","SET TIME","CHANGE PASSWORD"};

int add = 110;
int flag = 0;
int once = 1;
#endif
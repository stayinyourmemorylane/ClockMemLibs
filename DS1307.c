/*
 * DS1307.h
 *
 * Created: 3/12/2013 11:45:26 AM
 *  Author: mfolz
 */ 
/// DS1307 Clock module 
#define F_CPU 16000000UL
#include <avr/delay.h>


#include "DS1307.h"
#include "../32x16 Libs/HT1632C.h" // used for displaying time on the 32x16 display
#include "../../Macros_Functions.h"

char *Days[] = { "Sunday ", "Monday ", "Tuesday ", "Wednesday ", "Thursday ", "Friday ", "Saturday "};
char *Month[]= { "January ", "February ", "March ", "April ", "May ", "June ", "July ", "August ", "September ", "October ", "November ", "December "};


void DS1307_Copy_Date(){
	struct DS1307_Properties *point_time = &Time;
	char date[3];
	point_time->Day = (TWI_Read(DS_Date));
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date,  Days[(TWI_Read(DS_WeekDay)-1)]);
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date,  Month[(TWI_Read(DS_Month)-1)]);
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date,  IntToArray(bcdtobin(TWI_Read(DS_Date)),1)); // add Days + month put it in display_date 
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date,   DateExtension(bcdtobin(TWI_Read(DS_Date))) );
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date, " ");
	DS_ReadTime_Delay; 
	stringcopy(point_time->Display_Date, IntToArray((bcdtobin(TWI_Read(DS_Year))+1998),3));
	DS_ReadTime_Delay; 
}


// Display Library Read any address prefer 0x00- 0x05
void DS1307_ReadTime(int x, int color, int address){
	struct DS1307_Properties *point_time = &Time;
	struct HT1632C_Properties *point_properties = &Current;
	DS_ReadTime_Delay; 
	point_time->CurrentTime = TWI_Read(address);
	HT1632_Write_Char(x,point_properties->yPosition,color,  (point_time->CurrentTime>>4)+0x30);
	HT1632_Write_Char(x+6,point_properties->yPosition,color, ((point_time->CurrentTime & 0x0F))+0x30);
	DS_ReadTime_Delay; 
}

void DS1307_WriteTime(int address, int time){
	I2C_StartWrite();
	I2C_SendByte(0xD0);
	I2C_SendByte(address);
	I2C_SendByte(time);
	I2C_TerminateTransmission();	
}

void DS1307_SetTime(int seconds, int minutes, int hours, int weekday, int date, int months, int year){
	DS1307_WriteTime(0x00,bin_to_bcd(seconds));
	DS1307_WriteTime(0x01,bin_to_bcd(minutes));
	DS1307_WriteTime(0x02,bin_to_bcd(hours));
	DS1307_WriteTime(0x03,bin_to_bcd(weekday));
	DS1307_WriteTime(0x04, bin_to_bcd(date));
	DS1307_WriteTime(0x05,bin_to_bcd(months));
	DS1307_WriteTime(0x06, bin_to_bcd((year - 2000)));
}

/*


/// Display Library // Display both Hours and minutes // should make this variable.
void Display_Time(int y, int color){
	struct DS1307_Properties *point_time = &Time;
	struct HT1632C_Properties *point_properties = &Current;
	point_properties->color= color;
	point_properties->yPosition= y;
	DS1307_ReadTime(0,color, DS_Minutes);
	HT1632_Write_Char(12,y, color, ':');
	DS1307_ReadTime(18,color, DS_Seconds);
}
 */


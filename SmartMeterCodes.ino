#include <LiquidCrystal.h>
#include <PZEM004Tv30.h>
int counter;
float coss;
#include <SPI.h> 
#include <SD.h>  
File SmartMeterFile;      
int DayCounter;
int PowerCounter;
float powerKWh;
float powerKWhTotal;
float DailyPowerKWhTotal;
float powerKWhProduced;
float ProducedPowerKWhTotal;
float DailyProducedPowerKWhTotal;
float Producedpower;

PZEM004Tv30 pzem(15, 16);

LiquidCrystal lcd( 8,  9,  4,  5,  6,  7);
#define PI 3.1415926535897932384626433832795;
void setup() {
 lcd.begin(16, 2);
 counter=1;
 DayCounter = 0;
  powerKWhTotal = 0;
  DailyPowerKWhTotal=0;
  ProducedPowerKWhTotal=0;
  
  DailyProducedPowerKWhTotal=0;
  if (!SD.begin(3))      
  {
    while (!SD.begin(3)); } 
    
}
void loop() {
  buttonlcd();
 SmartMeterFile = SD.open("data.txt", FILE_WRITE); 
 
    if (SmartMeterFile)
    {
      
      if(DayCounter==0)
      {
        SplitDayToTime(); 
        SmartMeterFile.println(" "); 
      }
      if(DayCounter!=0)
      {
        SmartMeterFile.print("Day ");
        SmartMeterFile.print(DayCounter);
        SmartMeterFile.print(":");
        SmartMeterFile.print(",");  
        SmartMeterFile.print("Usage:"); 
        SmartMeterFile.print(","); 
        for(int t=0;t<144;t++)
        {
          for(int pz=1;pz<=10;pz++)
          {
           float powerKWh = pzem.energy();
           powerKWhTotal=powerKWhTotal+powerKWh;
           if(pz==10){powerKWhTotal=powerKWhTotal/10;}
            
          }
         SmartMeterFile.print(powerKWhTotal);
         SmartMeterFile.print("KWh");
         SmartMeterFile.print(",");
            DailyPowerKWhTotal=DailyPowerKWhTotal+powerKWhTotal;
            powerKWhTotal=0;
        }
        SmartMeterFile.print(DailyPowerKWhTotal);
        SmartMeterFile.print("KWh,");
        SmartMeterFile.println(" ");
        DailyPowerKWhTotal=0;
         
      }
       if(DayCounter!=0)
      {
        SmartMeterFile.print("  ");
        
        SmartMeterFile.print(",");  
        SmartMeterFile.print("Produce:"); 
        SmartMeterFile.print(","); 
        for(int t=0;t<144;t++)
        {
          if(t>40&&t<120){
          for(int pzz=1;pzz<=10;pzz++)
          {
            float Producedpower = 0;
            ProducedPowerKWhTotal =ProducedPowerKWhTotal+Producedpower;
           if(pzz==10){ProducedPowerKWhTotal=ProducedPowerKWhTotal/10;}
           
          }
          }
          else
          {for(int pzz=1;pzz<=10;pzz++)
          {
            Producedpower = 0;
            ProducedPowerKWhTotal =ProducedPowerKWhTotal+Producedpower;
           if(pzz==10){ProducedPowerKWhTotal=ProducedPowerKWhTotal/10;}
           
          }
            }
         SmartMeterFile.print(ProducedPowerKWhTotal);
         SmartMeterFile.print("KWh");
         SmartMeterFile.print(",");
            DailyProducedPowerKWhTotal=DailyProducedPowerKWhTotal+ProducedPowerKWhTotal;
            ProducedPowerKWhTotal=0;
        }
        SmartMeterFile.print(" ");
        SmartMeterFile.print(",");
        SmartMeterFile.print(DailyProducedPowerKWhTotal);
        SmartMeterFile.println("KWh");
         ProducedPowerKWhTotal=0;
         DailyProducedPowerKWhTotal=0;
      }
    }
    SmartMeterFile.close(); 
                  
    DayCounter=((DayCounter+1)%31);                          


 
} 



void buttonlcd() {
 float v = pzem.voltage();
 float a = pzem.current();
 float e = pzem.energy();
 float w = pzem.power();
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print ("V:");
   lcd.setCursor(2,0);
   lcd.print (v);
   
   lcd.setCursor(9,0);
   lcd.print ("A:");
   lcd.setCursor(11,0);
   lcd.print (a);
  
   lcd.setCursor(0,1);
   lcd.print ("KWh:");
   lcd.setCursor(4,1);
   lcd.print (e);

delay(1000);

}



 
  

void SplitDayToTime()
{   int usercounter=0;
   char HoursOfDay[24][4] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
   char MinutesOfHours[6][4] = {"00","10","20","30","40","50"};
      if(usercounter==0)
      {
      SmartMeterFile.println("User No:0000,");
      SmartMeterFile.println("Intelligence Node Smart Meter,");
      usercounter=1;}
      SmartMeterFile.print("Days / Hours ,");
      SmartMeterFile.print(" ");
      SmartMeterFile.print(",");
      
   int counter=0;
      for (int i=0;i<24;i++)
      {
        for(int j=0;j<6;j++)
        { 
          
          SmartMeterFile.print(HoursOfDay[i]);
          SmartMeterFile.print(":");
          SmartMeterFile.print(MinutesOfHours[j]);
          SmartMeterFile.print(",");
           
           
          counter=(counter+1)%144;
        }
      }
       SmartMeterFile.print("Total Usage");
       SmartMeterFile.print(",");
       SmartMeterFile.print("Total Produce");
      
}


#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RFID.h>
#include "Car4W.h"
#include "TCRT5000.h"
#include "MotorCD.h"

////////////////////////////
//  LCD object instance  //
////////////////////////////

LiquidCrystal_I2C lcd(0x27, 20, 4);

////////////////////////////
//  RFID object instance  //
////////////////////////////

/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO   9
#define RESET_DIO 8

RFID RC522(SDA_DIO, RESET_DIO); 

//////////////////////////////////
//  TCRT5000 objects instances  //
//////////////////////////////////

TCRT5000 Sensor1(31);
TCRT5000 Sensor2(29);
TCRT5000 Sensor3(27);
TCRT5000 Sensor4(25);
TCRT5000 Sensor5(23);               

//////////////////////////////////////
//  Car4WWarehouse object instance  //
//////////////////////////////////////

Car4W Car4WWarehouse( 26 , 24 , 22 ,       // Motor Front Left
                      30 , 28 , 32 ,       // Motor Front Right
                       4 ,  3 ,  2 ,       // Motor Rear Left 
                       6 ,  5 ,  7 );      // Motor Rear Right

////////////////////////////////
//  Car4WWarehouse Varibles   //
////////////////////////////////

int SpeedMotorFrontLeft  = 0;
int SpeedMotorRearLeft   = 0;
int SpeedMotorFrontRight = 0;
int SpeedMotorRearRight  = 0;

bool bSensor[5] = {false, false, false, false, false};

////////////////////////////////
//  Car4WWarehouse Constants  //
////////////////////////////////

const unsigned char Speed = 255;

//////////////////////
//  PID Variables   //
//////////////////////

double Error          = 0;
double LastError      = 0;
double IntegralError  = 0;
double DerivateError  = 0;

double P = 0;                  
double I = 0;                    
double D = 0;
double U = 0;                   

//////////////////////
//  PID Constants   //
//////////////////////

const unsigned char SamplingTime = 1;
const unsigned char Kp = 60.0;
const unsigned char Ki =  1.0;
const unsigned char Kd = 50.0;

//////////////////////////
//  Define RTOS Tasks   //
//////////////////////////

void TaskLineFollower(void *pvParameters);
void TaskRFIDReader(void *pvParameters);
void TaskRaspberryCommunication(void *pvParameters);

void setup() 
{  
  Serial.begin(9600);

  //////////////////
  //  LCD setup  //
  //////////////////
  
  lcd.begin();
  lcd.backlight();

  //////////////////
  //  RFID setup  //
  //////////////////

  SPI.begin(); 
  RC522.init();
  
  //////////////////////////////
  //  TCRT5000 objects setup  //
  //////////////////////////////
  
  Sensor1.Setup();
  Sensor2.Setup();
  Sensor3.Setup();
  Sensor4.Setup();
  Sensor5.Setup();

  ///////////////////////////////////
  //  Car4WWarehouse object setup  //
  ///////////////////////////////////

  Car4WWarehouse.Setup();

  //////////////////////////
  //  Setup RTOS Tasks    //
  //////////////////////////

  xTaskCreate (  TaskLineFollower
              ,  (const portCHAR *) "LineFollower"
              ,  128                                // Stack size
              ,  NULL
              ,  3                                  // Priority
              ,  NULL );

  xTaskCreate (  TaskRFIDReader
              ,  (const portCHAR *) "RFIDReader"
              ,  128                                // Stack size
              ,  NULL
              ,  2                                  // Priority
              ,  NULL );

  xTaskCreate (  TaskRaspberryCommunication
              ,  (const portCHAR *) "RaspberryComunication"
              ,  128                                // Stack size
              ,  NULL
              ,  1                                  // Priority
              ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void TaskLineFollower(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    //////////////////////////
    //  Get TCRT500 values  //
    //////////////////////////
    
    bSensor[0] = Sensor1.getDigitalValue();
    bSensor[1] = Sensor2.getDigitalValue();
    bSensor[2] = Sensor3.getDigitalValue();
    bSensor[3] = Sensor4.getDigitalValue();
    bSensor[4] = Sensor5.getDigitalValue();

    ////////////////////////////////
    //  Get line follower error   //
    ////////////////////////////////
    
    if     ( (bSensor[0] == 0) && (bSensor[1] == 0) && (bSensor[2] == 0) && (bSensor[3] == 0) && (bSensor[4] == 1) )    Error =  4;
    else if( (bSensor[0] == 0) && (bSensor[1] == 0) && (bSensor[2] == 0) && (bSensor[3] == 1) && (bSensor[4] == 1) )    Error =  3;
    else if( (bSensor[0] == 0) && (bSensor[1] == 0) && (bSensor[2] == 0) && (bSensor[3] == 1) && (bSensor[4] == 0) )    Error =  2;
    else if( (bSensor[0] == 0) && (bSensor[1] == 0) && (bSensor[2] == 1) && (bSensor[3] == 1) && (bSensor[4] == 0) )    Error =  1;
    else if( (bSensor[0] == 0) && (bSensor[1] == 0) && (bSensor[2] == 1) && (bSensor[3] == 0) && (bSensor[4] == 0) )    Error =  0;
    else if( (bSensor[0] == 0) && (bSensor[1] == 1) && (bSensor[2] == 1) && (bSensor[3] == 0) && (bSensor[4] == 0) )    Error = -1;
    else if( (bSensor[0] == 0) && (bSensor[1] == 1) && (bSensor[2] == 0) && (bSensor[3] == 0) && (bSensor[4] == 0) )    Error = -2;
    else if( (bSensor[0] == 1) && (bSensor[1] == 1) && (bSensor[2] == 0) && (bSensor[3] == 0) && (bSensor[4] == 0) )    Error = -3;
    else if( (bSensor[0] == 1) && (bSensor[1] == 0) && (bSensor[2] == 0) && (bSensor[3] == 0) && (bSensor[4] == 0) )    Error = -4;

    //////////////////////////
    //  PID implementation  //
    //////////////////////////
  
    IntegralError += (Error * SamplingTime);                  
    DerivateError  = (Error - LastError) / SamplingTime;      
    
    LastError = Error;                                        
        
    P = Kp * Error;                                           
    I = Ki * IntegralError;                                   
    D = Kd * DerivateError;                                   
        
    U = P + I + D;                                           

    //////////////////////////////////////
    //  Get PWM signal for each motor   //
    //////////////////////////////////////
    
    SpeedMotorFrontLeft  = constrain(Speed - U , -255, 255);
    SpeedMotorRearLeft   = constrain(Speed - U , -255, 255);
    SpeedMotorFrontRight = constrain(Speed + U , -255, 255);
    SpeedMotorRearRight  = constrain(Speed + U , -255, 255);

    //////////////////////////////////////
    //  Set PWM signal for each motor   //
    //////////////////////////////////////

    //Car4WWarehouse.Forward(SpeedMotorFrontLeft, SpeedMotorFrontRight, SpeedMotorRearLeft, SpeedMotorRearRight);

    ///////////////////////////////
    //  Print values for debug   //
    ///////////////////////////////
/*
    Serial.print(bSensor[0]);
    Serial.print(bSensor[1]);
    Serial.print(bSensor[2]);
    Serial.print(bSensor[3]);
    Serial.print(bSensor[4]);
    Serial.print("  EP:");
    Serial.print(IntegralError);
    Serial.print("  ED:");
    Serial.print(DerivateError);
    Serial.print("  EA:");
    Serial.print(LastError);
    Serial.print("  P:");
    Serial.print(P);
    Serial.print("  I:");
    Serial.print(I);
    Serial.print("  D:");
    Serial.print(D);
    Serial.print("  U:");
    Serial.print(U);
    Serial.print("  MFL:");
    Serial.print(SpeedMotorFrontLeft);  
    Serial.print("  MRL:");
    Serial.print(SpeedMotorRearLeft);
    Serial.print("  MFR:");
    Serial.print(SpeedMotorFrontRight);
    Serial.print("  MRR:");
    Serial.println(SpeedMotorRearRight);
*/    
    vTaskDelay(2);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskRFIDReader(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    if (RC522.isCard())
    {
      RC522.readCardSerial();

      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("RFID Tag detected: ");

      lcd.setCursor(0, 1); 
      
      for(int i=0;i<5;i++)
      {
        
        lcd.print(RC522.serNum[i],HEX);
        lcd.print(" ");
        
        //Serial.print(RC522.serNum[i],DEC);
        //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format   
        //Serial.print(" "); 
      }
    }
    
    vTaskDelay(5);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskRaspberryCommunication(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    int x = 0;
    char DatoSerial;
    char DatoString[10] = {"         "};  

    while(Serial.available())
    {
      DatoSerial = Serial.read(); 
      DatoString[x] = DatoSerial;
      x++;
    }
    
    if      (!strcmp(DatoString,"Forward.."))  Car4WWarehouse.Forward  (200, 200, 200, 200);
    else if (!strcmp(DatoString,"Backward."))  Car4WWarehouse.Backward (200, 200, 200, 200);
    else if (!strcmp(DatoString,"Left....."))  Car4WWarehouse.Left     (200, 200, 200, 200);
    else if (!strcmp(DatoString,"Right...."))  Car4WWarehouse.Right    (200, 200, 200, 200);
    else if (!strcmp(DatoString,"Stop....."))  Car4WWarehouse.Stop();

    vTaskDelay(5);
  }
}


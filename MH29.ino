#include <math.h>
#include <SPI.h>

#include <LiquidCrystal.h>

#include "Wire.h"
extern TwoWire Wire1;

#include <wiinunchucki2c2.h>
#include <I2Cdev.h>
//#include <L3G4200D.h>
//#include <HMC5883L.h>
//#include <ADXL345.h>
//#include <Sensor.h>
//#include <MP101.h>
#include <SmartHead.h>

//Treshhold
const int joy_th=25;
int varx, vary = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 17, 16, 15, 14);

SmartHead HD;

void setup() {
  Serial.begin(115200);  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Powered up!");

  nunchuk_init();
  
  Serial.print ("Finished setup\n");
  delay(100);
  
  lcd.clear();
  lcd.print("Joystick calibration!");
  // Joystick calibration
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {
    joy_x=nunchuk_joy_x();
    joy_y=nunchuk_joy_y();
    Serial.print("JoyX:");
    Serial.print(joy_x);
    Serial.print("\t| ");
    Serial.print("JoyY:");
    Serial.print(joy_y);
    Serial.print("\t| ");
    
    nunchuk_calibrate_joy();
    Serial.print("CalX:");
    Serial.print(joy_zerox);
    Serial.print("\t| ");
    Serial.print("CalY:");
    Serial.print(joy_zeroy);
    Serial.print("\t| ");
  }  
  

  // initialize sensors
  Wire.begin();
  Wire.setClock(400000);
  Serial.println("Initializing I2C sensors...");
  HD.sns.Init();
  
  // verify connection
  lcd.clear();
  lcd.print("Testing sensors:");
  delay(2000); // 2 sec. to read status
  lcd.setCursor(0,1);
  if (HD.sns.testConnect())lcd.print("successful");
  else   lcd.print("failed");
  delay(2000); // 2 sec. to read status
     
  
  lcd.clear();
  lcd.print("Gyro calibration!");
  HD.sns.GyroCailbr();
  lcd.clear();
  lcd.print("Ready!!!");
  delay(1500); //1.5 sec. to read status
}


void loop() {
  delay(1);
  if (nunchuk_get_data()) {
    // 100 is the distance from 0 on the joystick but safe to take 90 as max value
    varx = nunchuk_cjoy_x(); // nunchuk.analogX is the value of the x-axis
    vary = nunchuk_cjoy_y(); // nunchuk.analogY is the value of the y-axis
//    Serial.print("JoyX:");Serial.print(varx);Serial.print("\t| ");
//    Serial.print("JoyY:");Serial.print(vary);Serial.print("\t| ");

    // Activate return mode - return mode automatically canceled when 0 delata angle is reached 
    if (nunchuk_zbutton()) HD.ReturnToPos();

//    Serial.print("Pot: ");
//    Serial.print(mapx);
//    Serial.print(", ");  
//    Serial.print(mapy);
     
//    if (nunchuk_cbutton())
//      Serial.print("C");
//    else
//      Serial.print("-");
//    if (nunchuk_zbutton())
//      Serial.print("Z");
//    else
//      Serial.print("-");
//    Serial.println();
  }
  else {
    Serial.println("Cannot communicate to wiichuck");
    lcd.clear();
    lcd.print("Cannot communicate to wiichuck");
  } 
  
  
  

  //moving head
  if (HD.amove) HD.move();
  else if (abs(varx)>joy_th ||  abs(vary)>joy_th) HD.move(varx, vary);
  else HD.move_flag=0;

  if (HD.move_flag) 
  { 
      float rh,rv;
      HD.GetAngle(&rh, &rv); 
      Serial.print("Horizontal angle:");
      Serial.println(rh);
      Serial.print("Vertical angle:");
      Serial.println(rv);      
  };
  
}

#include "MP101.h"
#include <SPI.h>
// Default constructor
MP101::MP101() {
    pinMode (CS_H, OUTPUT);
    pinMode (CS_V, OUTPUT);
    SPI.begin();

    pinMode(upPin, OUTPUT);
    pinMode(downPin, OUTPUT);
    pinMode(leftPin, OUTPUT);
    pinMode(rightPin, OUTPUT);
}

// Move head command
void MP101::move(int hspeed,int vspeed)
{
// direction switch 
    if (vspeed > l_speed_th) {
        upState = HIGH; 
    }
    else {
        if (vspeed < -l_speed_th) {
          downState = HIGH; 
        }
        else {
           upState = LOW;
           downState = LOW;
        };
    };
    if (hspeed > l_speed_th) {
        rightState = HIGH;
    }
    else {
        if (hspeed < -l_speed_th) {
          leftState = HIGH;
        }
        else {
           rightState = LOW;
           leftState = LOW;
        };
    }
    
    move_flag=upState||downState||rightState||leftState;

// set pot for speed 
    // The values used for speed 0-99 to map on 256 grades of pot
    if (leftState || rightState) {
        if (abs(hspeed)>h_speed_th) {mapx=255;}
        else {mapx = map(abs(hspeed), 0, h_speed_th, 0, 255);};
    }
    else {
        mapx=0;
    }; 
    if (upState || downState) {
        if (abs(vspeed)>h_speed_th) {mapx=255;}
        else {mapy = map(abs(vspeed), 0, h_speed_th, 0, 255);};
    }
    else {
        mapy=0;
    };         
    digitalPotWrite(CS_H,mapx);
    digitalPotWrite(CS_V,mapy);

// set pin for movement
    digitalWrite(upPin, upState);
    digitalWrite(downPin, downState);
    digitalWrite(leftPin, leftState);
    digitalWrite(rightPin, rightState);

    // if (move_flag) {
        // Serial.print("Horizontal: (rstate,lstate,angle,speed)");
        // Serial.print(rightState);
        // Serial.print(":");
        // Serial.print(leftState);
        // Serial.print(":");
        // Serial.print(rh);
        // Serial.print(":");  
        // Serial.print(abs(rh+ch));
        // Serial.print(":");
        // Serial.println(mapx);

        // Serial.print("Vertical: (ustate,dstate,angle,speed)");
        // Serial.print(upState);
        // Serial.print(":");
        // Serial.print(downState);
        // Serial.print(":");
        // Serial.print(rv);
        // Serial.print(":");
        // Serial.print(abs(rv+cv));
        // Serial.print(":");
        // Serial.println(mapy);
    // }  
};

// press shooter
void MP101::shootDown()
{
    shooterState = HIGH;
    digitalWrite(shooterPin, shooterState);
};

// unpress shooter
void MP101::shootUp()
{
    shooterState = LOW;
    digitalWrite(shooterPin, shooterState);
};

// unpress shooter
void MP101::shoot()
{
    shootDown()
    delay(10);
    shootUp();
};

// digital pot controlled via SPI
int MP101::digitalPotWrite(int CS, int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(mcp41100);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}
#ifndef L3G4200D_h
#define L3G4200D_h

#include <Wire.h>
#include "Arduino.h"


#define L3G4200D_DEVICE 105
#define L3G4200D_BYTES_READ 6

#define L3G4200D_CTRL_REG1 0x20
#define L3G4200D_CTRL_REG2 0x21
#define L3G4200D_CTRL_REG3 0x22
#define L3G4200D_CTRL_REG4 0x23
#define L3G4200D_CTRL_REG5 0x24

#define L3G4200D_OUT_X_L 0x28
#define L3G4200D_OUT_X_H 0x29
#define L3G4200D_OUT_Y_L 0x2A
#define L3G4200D_OUT_Y_H 0x2B
#define L3G4200D_OUT_Z_L 0x2C
#define L3G4200D_OUT_Z_H 0x2D

#define ALPHA_G 0.3

struct GyroRaw
{
  int x;
  int y;
  int z;
};

struct GyroDPS
{
  double x;
  double y;
  double z;
};

class L3G4200D
{
  public:
    L3G4200D();
    void init(double xoffset=0, double yoffset=0, double zoffset=0, int scale=250);
    void writeTo(byte address, byte val);
    void readFrom(byte address, int num, byte _buff[]);
    GyroRaw readGyro();
    GyroDPS readGyroDPS();
    void printAllRegister();
    void print_byte(byte val);
    void printCalibrationValues(int samples);
//    bool checkOverrun();
    
  private:
     byte _buff[6];
     
     double xg;
     double yg;
     double zg;
     
     double _xoffset;
     double _yoffset;
     double _zoffset;

    
};

#endif

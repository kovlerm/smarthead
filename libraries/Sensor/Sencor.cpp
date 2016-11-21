#include "Sensor.h"
// Default constructor, uses default I2C address.
Sensor::Sensor() {
    Reset();
}

// Device initialization
void Sensor::Init() {
    gyro.initialize();
    mag.initialize();
    accel.initialize();
    
    // Accelerometer scale
    accel.setRange(0x0); // 0x0 +/-2g 
    a_gain=4.0/1024.0;
    // for self test  
    //    Serial.println("SelfTestResult:");
    //    gyro.setBandwidthCutOffMode(L3G4200D_BW_MED_HIGH);
    //    gyro.setOutputDataRate(200);
    //    gyro.setBlockDataUpdateEnabled(1);
    //    gyro.setSelfTestMode(L3G4200D_SELF_TEST_NORMAL);
    //    gyro.setSelfTestMode(L3G4200D_SELF_TEST_1);
    //    gyro.setFullScale(2000);
    gyro.setFullScale(250); //250dps = 8.75 mdps/digit
    g_gain=0.00875; 
    
};


// Test connection
//  testing only for gyro since all 3 sensors have common wiring
bool Sensor::testConnect() { 
    return (gyro.testConnection());
};    

// Gyro callibration
//  Makes 100 reading on not moving head and calculate offsets and noize
void Sensor::GyroCailbr()
{
    delay(1500); //wait for the sensor to be ready 
    int dc_offset_x, dc_offset_y, dc_offset_z, noise_x, noise_y, noise_z;
    dc_offset_x=dc_offset_y=dc_offset_z=dc_offset_x_norm=dc_offset_y_norm=dc_offset_z_norm=noise_x=noise_y=noise_z=0;
    for (int n=1; n<101; n++) {
      delay(10);
      // read raw angular velocity measurements from device
      gyro.getAngularVelocity(&avx, &avy, &avz);
      Serial.print(avx); Serial.print("\t");
      Serial.print(avy); Serial.print("\t");
      Serial.print(avz);
  
      //Calculate initial DC offset and noise level of gyro
      dc_offset_x+=avx;
      dc_offset_x_norm=dc_offset_x/n;
      if (abs(avx-dc_offset_x_norm) > abs(noise_x)) {
        noise_x=avx-dc_offset_x_norm;
      }
      dc_offset_y+=avy;
      dc_offset_y_norm=dc_offset_y/n;
      if (abs(avy-dc_offset_y_norm) > abs(noise_y)) {
        noise_y=(int)avy-dc_offset_y_norm;
      }
      dc_offset_z+=avz;
      dc_offset_z_norm=dc_offset_z/n;
      if (abs(avz-dc_offset_y_norm) > abs(noise_z)) {
        noise_z=avz-dc_offset_z_norm;
      }
      //print dc offset and noise level
      Serial.print("DC Offset: ");
      Serial.print(dc_offset_x_norm);
      Serial.print(" | ");
      Serial.print(dc_offset_y_norm);
      Serial.print(" | ");
      Serial.print(dc_offset_z_norm);
      Serial.print(" | ");
    
      Serial.print("\tNoise Level: ");
      Serial.print(noise_x);
      Serial.print(" | ");
      Serial.print(noise_y);
      Serial.print(" | ");
      Serial.print(noise_z);
      Serial.println(" | ");
   };   
};

// Read delta angle
void Sensor::Read()
{
    // --------------------------
    // Gyro reading 
    // --------------------------

    // read raw angular velocity measurements from device
    gyro.getAngularVelocity(&avx, &avy, &avz);
    // Serial.print("raw angular velocity(X,Y,Z):");
    // Serial.print(avx);
    // Serial.print(":");
    // Serial.print(avy);
    // Serial.print(":");
    // Serial.println(avz);  
    
    // Add calibration offset
    avx=avx-dc_offset_x_norm;
    avy=avy-dc_offset_y_norm;
    avz=avz-dc_offset_z_norm;

    // digit time gain to dps then use Low Pass FIlter   
    g_fXg = (double) avx * g_gain * alpha_g + (g_fXg * (1.0 - alpha_g));
    g_fYg = (double) avy * g_gain * alpha_g + (g_fYg * (1.0 - alpha_g));
    g_fZg = (double) avz * g_gain * alpha_g + (g_fZg * (1.0 - alpha_g));
    
    // Serial.print("Adjusted angular velocity:\t");
    // Serial.print(g_fXg); Serial.print("\t");
    // Serial.print(g_fYg); Serial.print("\t");
    // Serial.print(g_fZg);
    
    // read time interval for calculating delta angle 
    unsigned long cT = micros();
    unsigned long dT = cT - pT;
    pT = cT;
    
    // Add delta angle to angle 
    // Serial.print("|XI|");
    xi = xi + g_fXg*(dT/1000000.0);
    // Serial.print(xi);
    yi = yi + g_fYg*(dT/1000000.0);
    // Serial.print("|YI|");
    // Serial.print(yi);
    zi = zi + g_fZg*(dT/1000000.0);
    // Serial.print("|ZI|");
    // Serial.print(zi);
    
    // --------------------------
    // Accelerometer reading - assuming only gravitation acceleration. 
    // So best reading in stationary position. Depends on position of sensor. 
    // --------------------------
    // read raw accel measurements from device
    accel.getAcceleration(&ax, &ay, &az);
    // digit time gain to dps then use Low Pass FIlter   
    fXg = (double)ax*a_gain * alpha + (fXg * (1.0 - alpha));
    fYg = (double)ay*a_gain * alpha + (fYg * (1.0 - alpha));
    fZg = (double)az*a_gain * alpha + (fZg * (1.0 - alpha));

    // display tab-separated accel x/y/z values
    // Serial.print("\taccel:\t");
    // Serial.print(fXg); Serial.print(",");
    // Serial.print(fYg); Serial.print(",");
    // Serial.println(fZg);
 
    //Roll & Pitch Equations
    roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
    pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
 
    // Serial.print(pitch);
    // Serial.print(":");
    // Serial.println(roll);  

    // --------------------------
    // Magnetometer reading 
    // --------------------------
    // read raw heading measurements from device
    mag.getHeading(&mx, &my, &mz);
    // display tab-separated gyro x/y/z values
    // Serial.print("\tmag:");
    // Serial.print(mx); Serial.print(",");
    // Serial.print(my); Serial.print(",");
    // Serial.println(mz); Serial.print(",");
    
    // To calculate heading in degrees. 0 degree indicates North
    heading = atan2(my, mx);
    if(heading < 0)
       heading += 2 * M_PI;
    // Serial.print("\theading:");
    // Serial.println(heading * 180/M_PI);
};

// Start reading angle 
void Sensor::FromAngle()
{
    // reset sensor
    Reset();
    // read sensor 
    Read();
    d_heading=heading;
    d_pitch=pitch;
    d_roll=roll;
    //Gyro
    xi=yi=zi=0; 
};

// Start reading angle 
void Sensor::Reset()
{
    //Gyro
    xi=yi=zi=0; 
    g_fXg = g_fYg = g_fZg = 0;
    //Accel 
    fXg = fYg = fZg = 0;
    roll=pitch=0;
};

// Return horisontal and vertical angle
void Sensor::getAngle(float* ch, float* cv)
{
    Read();            
    float cva=d_roll-roll;                          // Return vertical angle accel
    float chm=chm=(d_heading-heading) * 180/M_PI;   // Return horisonatl angle mag

    *ch=(float) (zi*0.99+d_heading*0.01);                      // Return horisontal angle (do not trust magnetometer)
    *cv=(float) (xi*0.7+cva*0.3);                              // Return vertical angle (more trust gyro)
    // Serial.print(ch);
    // Serial.print(":");
    // Serial.println(cv);
};      
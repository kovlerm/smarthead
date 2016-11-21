// Sensor Class combines 
// ADXL345 accelerometer
// L3G4200D gyro
// HMC5883L magnitometer
// Main input is taken from Gyro as integral of angular velocity
// Accelerometer is used as "horizon level" to get vertical angle
// Magnetometer is used as "compass" to get horizontal angle
// The class is based on 3 "device" classes 
// I2Cdev and L3G4200D must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "..\I2Cdev\I2Cdev.h"
#include "..\L3G4200D\L3G4200D.h"
#include "..\HMC5883L\HMC5883L.h"
#include "..\ADXL345\ADXL345.h"

class Sensor {
    public:
        //Accelerometer
        double pitch, roll;
        //Gyro
        double xi,yi,zi;       // current angle
        //Mag
        float heading=0;
    
        // Constructor define variables
        Sensor();
        //Initialize sensors check connections 
        void Init(); 
        // Test connection
        bool testConnect(); 
        // Gyro Calibration
        void GyroCailbr();
        // Read angles
        void Read();
        
        // Start reading angle 
        void FromAngle();
        
        // Reset internal variables 
        void Reset();
        
        // Get vertical and horizontal head angle from starting position
        void getAngle(float* ch, float* cv);

    private:
        // default address is 105
        // specific I2C address may be passed here
        L3G4200D gyro;
        // class default I2C address is 0x1E
        // specific I2C addresses may be passed as a parameter here
        // this device only supports one I2C address (0x1E)
        HMC5883L mag;
        // class default I2C address is 0x53
        // specific I2C addresses may be passed as a parameter here
        // ALT low = 0x53 (default for SparkFun 6DOF board)
        // ALT high = 0x1D
        ADXL345 accel;
    
        //Gyro
        int16_t avx, avy, avz;                                      // current raw reading
        int dc_offset_x_norm, dc_offset_y_norm, dc_offset_z_norm;   // calibration offsets (calculated by GyroCalibration)
        double g_gain;                                              // gain for scale setting
        
        double g_fXg = 0;                                           // Last time X Ang. velocity for LOW PATH filter
        double g_fYg = 0;                                           // Last time Y Ang. velocity for LOW PATH filter
        double g_fZg = 0;                                           // Last time Z Ang. velocity for LOW PATH filter
        const double alpha_g = 0.3;                                 // alpha for LOW PATH filter
        
        unsigned long pT;                                           // Time of last reading for delta time calculation

        //Accelerometer
        int16_t ax, ay, az;                                         // current raw reading
        double a_gain; // gain for scale setting
        
        double Xg = 0;                                              
        double Yg = 0;  
        double Zg = 0;
        double fXg = 0;                                             // Last time X acceleration for LOW PATH filter                          
        double fYg = 0;                                             // Last time Y acceleration for LOW PATH filter
        double fZg = 0;                                             // Last time Z acceleration for LOW PATH filter
        const double alpha = 0.3;                                   // alpha for LOW PATH filter
        
        float d_pitch=0;                                            // start rotation angle
        float d_roll=0;                                             // start rotation angle

        //Mag
        int16_t mx, my, mz;  
        float d_heading=0;                                          // start rotation angle
};

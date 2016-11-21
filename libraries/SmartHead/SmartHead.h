#include "..\MP101\MP101.h"
#include "..\Sensor\Sensor.h"
// Smart Head is based on MP101 class to control Motor head Bescor MP-101 Motorized Pan & Tilt Head and Sensor 
class SmartHead{
    public:
        MP101 mp101;
        Sensor sns; 
        int amove=0; // automatic moving
        int move_flag=0; // current state    
        // Constructor define variables
        SmartHead();
        // Remember current position (to measure angles from it)
        void SetPosition();
        // Return to set position
        void ReturnToPos();
        // Define rotation angles
        void SetRotation(float hAng, float vAng);
        // get remaining angle
        void GetRotation(float *hAng, float *vAng);        
        // get rotated angle (from set position)
        void GetAngle(float *hAng, float *vAng);
        // get last rotated angle (from set beginning of movement)
        void GetLastAngle(float *hAng, float *vAng);
        // Move head - override function of MP101 adding sensor call 
        // hspeed + right - left 0-99
        // vspeed + up    - down 0-99
        void move(int ,int);
        // move to define position (set by SetRotation) 
        void move();   

        // Programming Head
        // stop head for milliseconds
        void AddProgram(int delay);
        // move head to angle
        void AddProgram(float hAng, float vAng);
        // press shooter for time
        void AddProgramShoot(int delay);
        // Reset Program
        void ResetProgram();

        // start program
        void StartProgram(int delay);

    private:
          
        // current angle from beginning of movement 
        float c_vang, c_hang;
        // current angle from SetPosition 
        float p_vang, p_hang;
        //remaining angle from SetRotation 
        float r_vang, r_hang;     
        
        // Program structure 
        struct ProgramStep
        {
               int type; // type 0-pending, 1-shooting, 2-moving
               int time; // time milliseconds
               float va; // vertical angle
               float ha; // horizontal angle
        };
        
        ProgramStep Prog[20]; // max 20 step
        int pstep=0; // program steps
        int cstep=0; // current step
        
        
};

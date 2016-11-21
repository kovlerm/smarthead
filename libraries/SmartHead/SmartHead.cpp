#include "SmartHead.h"
// Programming Head
// int type; // type 0-pending, 1-shooting, 2-moving
// int time; // time milliseconds
// float va; // vertical angle
// float ha; // horizontal angle
        
// stop head for milliseconds
void SmartHead::AddProgram(int delay)
{
  Prog[pstep]={0,delay,0,0};
  pstep++;  
};
// move head to angle
void SmartHead::AddProgram(float hAng, float vAng)
{
  Prog[pstep]={2,0,hAng,vAng};
  pstep++;  
};
// press shooter for time
void SmartHead::AddProgramShoot(int delay)
{
  Prog[pstep]={1,delay,0,0};
  pstep++;   
};

// press shooter for time
void SmartHead::ResetProgram()
{
  pstep=0;  
};

// Remember current position (to measure angles from it)
void SmartHead::SetPosition(){
   p_vang=p_hang=0; sns.FromAngle(); 
};

// Define rotation angles
void SmartHead::SetRotation(float hAng, float vAng){
   r_vang=vAng;
   r_hang=hAng; 
   amove=1; 
};

// Define rotation angles
void SmartHead::ReturnToPos(){
   GetAngle(&r_hang, &r_vang); 
   r_vang=-r_vang;
   r_hang=-r_hang;  
   amove=1;
};

// get rotated angle (from set position)
void SmartHead::GetAngle(float *hAng, float *vAng){
    *hAng=p_hang+c_hang;
    *vAng=p_vang+c_vang;
};

// get last rotated angle (from set beginning of movement)
void SmartHead::GetLastAngle(float *hAng, float *vAng){
    *hAng=c_hang;
    *vAng=c_vang;
};

// move to Set Rotation position 
void SmartHead::move(int hspeed,int vspeed) { 
    if (!move_flag) {sns.FromAngle(); p_hang=c_hang; p_vang=c_vang; c_hang=c_vang=0;} // from stationary position
    else {sns.getAngle(&c_hang,&c_vang);};
    mp101.move(hspeed,vspeed);
    move_flag=1;
};    

// move to define position 
void SmartHead::move() { 
    int v_sp, h_sp;
    float dt_h=r_hang-c_hang;
    float dt_v=r_vang-c_vang;
    if (dt_h>=1 ) h_sp=100; // point is far more then 1 degree
    else if (dt_h<1 && dt_h>=0.5) h_sp=30; // point is close
    else if (dt_h>-1 && dt_h<=-0.5) h_sp=-30; // point is close
    else if (dt_h<=-1) h_sp=-100; // point is close
    else { // stop 
      Serial.print("Stop Horizontal");
      h_sp=0;
      r_hang=c_hang=0;
    };
    if (dt_v>=1 ) v_sp=100; // point is far more then 1 degree
    else if (dt_v<1 && dt_v>=0.5) v_sp=30; // point is close
    else if (dt_v>-1 && dt_v<=-0.5) v_sp=-30; // point is close
    else if (dt_v<=-1) v_sp=-100; // point is close
    else { // stop 
      Serial.print("Stop Vertical");
      v_sp=0;
      r_vang=c_vang=0;
    };
    if (v_sp!=0 || h_sp!=0) move(h_sp,v_sp);
    else amove=0;
};



    
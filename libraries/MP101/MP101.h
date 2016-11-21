// Motor head Bescor MP-101 Motorized Pan & Tilt Head
// Controlled by 4 optocouplers 4N35 (right,left, up, down) and 2 Digital Pot MCP41100 up-down, left-right speed. 
// 1 optocoupler 4N35 for shooter
#include <Arduino.h>
class MP101 {
    public:
        // state of the head 0-stationary 1-moving
        bool move_flag=0;
        // Constructor define variables
        MP101();
        // Move head 
        // hspeed + right - left 0-99
        // vspeed + up    - down 0-99
        void move(int hspeed,int vspeed);
        // press shooter
        void shootDown(); 
        // unpress shooter
        void shootUp(); 
        // momentary shooter press
        void shoot();

    private:
        
		// Digital Pins for Optocouplers 4N35
		const int upPin   =  6;      
		int upState       = LOW;
		const int downPin =  7;      
		int downState     = LOW;

		const int leftPin   =  3;      
		int leftState       = LOW;
		const int rightPin =  5;      
		int rightState     = LOW;
        
        const int shooterPin   =  2;      
		int shooterState       = LOW;
		
        // Digital Pins for Digital pots
		byte mcp41100=0x11; // Address for MCP41100
		int CS_H= 4; // Left-Right resistence 
		int CS_V=10; // Up-Down resistence
		
        // speed threshold (0-99)  
        const int l_speed_th=25; // low threshold - do not move if less then threshold to avoid tremors
        const int h_speed_th=90; // high threshold - move max speed
        
        int digitalPotWrite(int CS, int value);
};


#include <kipr/wombat>  // Include KIPR Wombat library
#include <iostream>     // Include standard input/output library
#include <cstdlib>      // For system() function

using namespace std;

//Parameters
bool shut_down_timer_enabled = false;    //Will the robot automqtically stop at the time limit
bool wait_for_light_trigger = false;     //Will the robot use the standard wait_for_light trigger at tournaments
int light_sensor_port = 0;               //What port is the light sensor connected to
int left_motor_port = 0;                 //Left drive motor port
int right_motor_port = 1;                //Right left motor port


//Prototypes
void load_parameters();                   //Loads runtime parameters
void os_system(const string& command;     //Allows running commands on the host system
void drive_straight();                    //Abstract drive straight function for the Link. It still veers off-course, but considerably less.

void main_code(){
  os_system("echo e");
}

int main() {
    load_parameters();
    main_code();
    return 0;
}
void load_parameters(){
  if ( shut_down_timer_enabled ) {
    shut_down_in(119);
  }
  if ( wait_for_light_trigger ) {
    wait_for_light
  }
}
void os_system(const string& command) {
    // Execute the system command
    int ret_code = system(command.c_str());
    if (ret_code != 0) {
        cerr << "Command execution failed with return code: " << ret_code << endl;
    }
}
void drive_straight() {
    // Calibrate the gyro's zero point
    short int GZ = gyro_z();

    // Integral term for correction
    int integral = 0;

    while (1) {
        // Read gyro's z-axis value
        short int g = gyro_z();

        // Calculate error
        int error = g - GZ;

        // Accumulate the integral term
        integral += error;

        // Prevent integral windup
        if (integral > INTEGRAL_LIMIT) integral = INTEGRAL_LIMIT;
        if (integral < -INTEGRAL_LIMIT) integral = -INTEGRAL_LIMIT;

        // Calculate corrections
        int correction = error * CORRECTION_FACTOR;    // Proportional correction
        correction += (int)(integral * 0.01);         // Integral correction

        // Adjust motor speeds
        mav(left_motor_port, BASE_SPEED - correction);
        mav(right_motor_port, BASE_SPEED + correction);
    }
}

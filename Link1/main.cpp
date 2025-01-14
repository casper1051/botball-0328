#include <kipr/wombat>  // Include KIPR Wombat library
#include <iostream>     // Include standard input/output library
#include <cstdlib>      // For system() function

using namespace std;

//Parameters
bool shut_down_timer_enabled = false;    //Will the robot automqtically stop at the time limit
bool wait_for_light_trigger = false;     //Will the robot use the standard wait_for_light trigger at tournaments
int light_sensor_port = 0;               //What port is the light sensor connected to
int left_motor_port = 0;                 //Left drive motor port
int right_motor_port = 0;                //Right left motor port


//Prototypes
void log(String buffer);                  //Logs output to the console
void load_parameters();                   //Loads runtime parameters
void os_system(const string& command;     //Allows running commands on the host system

void main_code(){
  os_system("echo e");
}

int main() {
    load_parameters();
    main_code();
    return 0;
}

void log(){
  cout << buffer << endl;
  
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

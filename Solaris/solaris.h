/*
Solaris Link Control
by Rocky R.
*/
#include <kipr/wombat.h>
#ifndef solaris
#define solaris
#include <string>

// Parameters
bool shut_down_timer_enabled = false;    // Will the robot automatically stop at the time limit
bool wait_for_light_trigger = false;     // Will the robot use the standard wait_for_light trigger at tournaments
int light_sensor_port = 0;               // Port for the light sensor
int left_motor_port = 1;                 // Left drive motor port
int right_motor_port = 0;                // Right drive motor port

// Constants
const int BASE_SPEED = 500;              // Base motor speed for turning (higher for better speed control)
const int CORRECTION_FACTOR = 2;         // Proportional correction factor
const int INTEGRAL_LIMIT = 10;           // Limit for integral windup
const float DEGREE_PER_COUNT = 250.0 / 512.0; // Degrees per gyroscope count

// Function Prototypes
void load_parameters();                 // Loads runtime parameters
void os_system(const std::string& command); // Runs system commands on the host
void drive_straight();                  // Abstract drive straight function for the robot
void main_code();                       // Main code execution logic
void turn_degrees(float degrees);       // Turns the robot by a specified number of degrees

// Function Definitions

inline void load_parameters(){
    if (shut_down_timer_enabled) {
        shut_down_in(119);
    }
    if (wait_for_light_trigger) {
        wait_for_light(light_sensor_port);
    }
}

inline void os_system(const std::string& command) {
    // Execute the system command
    int ret_code = system(command.c_str());
    if (ret_code != 0) {
        printf("An error has occurred on os_system: %d", ret_code);
    }
}

inline void drive_straight() {
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
inline void turn_degrees(float degrees) {
    //float initial_angle = 0.0;   // Start angle (in degrees)
    float current_angle = 0.0;   // Current angle (in degrees)
    float target_angle = degrees; // Target angle to reach
    float angular_velocity = 0.0; // Angular velocity in degrees per second

    // Start by reading the initial gyro_z (angular velocity)
    //short int initial_GZ = gyro_z();
    float last_time = seconds(); // Track the time

    // Loop until the target angle is reached
    while (1) {
        // Read the current gyro_z (angular velocity)
        short int current_GZ_raw = gyro_z();
        angular_velocity = (float)current_GZ_raw * DEGREE_PER_COUNT;

        // Calculate the time elapsed
        float current_time = seconds();
        float delta_time = current_time - last_time;

        // Integrate the angular velocity to get the angle
        current_angle += angular_velocity * delta_time;
        last_time = current_time; // Update last time for next iteration

        // Check if the target angle has been reached
        if (fabs(current_angle) >= fabs(target_angle)) {
            break;  // Exit the loop if the target angle is reached
        }

        // Adjust motor speeds based on the turn direction
        if (target_angle > 0) { // Turning right
            mav(left_motor_port, BASE_SPEED);
            mav(right_motor_port, -BASE_SPEED);
        } else { // Turning left
            mav(left_motor_port, -BASE_SPEED);
            mav(right_motor_port, BASE_SPEED);
        }
    }

    // Stop motors after reaching the target angle
    mav(left_motor_port, 0);
    mav(right_motor_port, 0);
}
// Debugging function for gyro z-axis

inline void debug_gyro() {
    // Save the initial gyro z value
    short int initial_GZ = gyro_z();

    printf("Initial gyro z-axis value: %d\n", initial_GZ);

    // Continuously print current gyro z value until user stops
    while (!side_button()) {}
    short int current_GZ = gyro_z();
    printf("Current gyro z-axis value: %d\n", current_GZ);
    msleep(500); // Sleep for 500 ms (adjust as needed)
}
#endif // Solaris

/*
	Solaris is a library, utility, and operating system packed into one img file. It can be used for various things. 
    This code is link_control_2.h of Solaris 1.2.1
	Older versions of Solaris are no longer supported
*/

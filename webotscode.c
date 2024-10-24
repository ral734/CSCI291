#include <webots/robot.h>
#include <webots/light_sensor.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <stdio.h>

#define TIME_STEP 64  // Time step for simulation in ms
#define MAX_SPEED 6.28  // Max speed of e-puck motors
#define NUM_DEAD_ENDS 9  // Number of dead-ends

// Structure to store information about dead-ends
typedef struct {
  double light_intensity;
  int index;
} DeadEnd;

// Function declarations
void move_forward();
void turn_left();
void turn_right();
void stop_robot();
void navigate_to_dead_end(int index);
void find_brightest_dead_end();

// Global variables
WbDeviceTag left_motor, right_motor, light_sensor;
WbDeviceTag ps[8];  // e-puck has 8 distance sensors
DeadEnd dead_ends[NUM_DEAD_ENDS];
int current_dead_end = 0;

void initialize() {
  wb_robot_init();

  // Set up motors
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Set up distance sensors
  char ps_names[8][4] = {"ps0", "ps1", "ps2", "ps3", "ps4", "ps5", "ps6", "ps7"};
  for (int i = 0; i < 8; i++) {
    ps[i] = wb_robot_get_device(ps_names[i]);
    wb_distance_sensor_enable(ps[i], TIME_STEP);
  }

  // Set up light sensor
  light_sensor = wb_robot_get_device("lts");  // Use the correct name of your light sensor
  wb_light_sensor_enable(light_sensor, TIME_STEP);

  // Initialize dead-ends data
  for (int i = 0; i < NUM_DEAD_ENDS; i++) {
    dead_ends[i].light_intensity = 0.0;
    dead_ends[i].index = i;
  }
}

int main(int argc, char **argv) {
  // Initialize the Webots API, sensors, and motors
  initialize();

  // Main loop
  while (wb_robot_step(TIME_STEP) != -1 && current_dead_end < NUM_DEAD_ENDS) {
    // Navigate to the current dead-end
    navigate_to_dead_end(current_dead_end);

    // Read the light sensor value
    double light_value = wb_light_sensor_get_value(light_sensor);
    printf("Dead End %d, Light Sensor Value: %f\n", current_dead_end, light_value);

    // Store the light value for the current dead-end
    dead_ends[current_dead_end].light_intensity = light_value;

    // Move to the next dead-end
    current_dead_end++;
  }

  // Find the brightest dead-end after visiting all dead-ends
  find_brightest_dead_end();

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}

// Function to navigate to a specified dead-end (placeholder)
void navigate_to_dead_end(int index) {
  // Placeholder function to simulate navigation to each dead-end
  // You need to implement a maze-solving algorithm here
  printf("Navigating to Dead End %d\n", index);
  move_forward();
  wb_robot_step(1000);  // Simulate time taken to navigate
  stop_robot();
}

// Function to find and navigate to the brightest dead-end
void find_brightest_dead_end() {
  int brightest_index = 0;
  double max_light_intensity = dead_ends[0].light_intensity;

  // Find the dead-end with the highest light intensity
  for (int i = 1; i < NUM_DEAD_ENDS; i++) {
    if (dead_ends[i].light_intensity > max_light_intensity) {
      max_light_intensity = dead_ends[i].light_intensity;
      brightest_index = i;
    }
  }

  // Navigate to the brightest dead-end
  printf("Brightest Dead End is %d with Light Intensity: %f\n", brightest_index, max_light_intensity);
  navigate_to_dead_end(brightest_index);
}

// Function to move the robot forward
void move_forward() {
  wb_motor_set_velocity(left_motor, MAX_SPEED * 0.5);
  wb_motor_set_velocity(right_motor, MAX_SPEED * 0.5);
}

// Function to turn the robot left
void turn_left() {
  wb_motor_set_velocity(left_motor, -MAX_SPEED * 0.5);
  wb_motor_set_velocity(right_motor, MAX_SPEED * 0.5);
}

// Function to turn the robot right
void turn_right() {
  wb_motor_set_velocity(left_motor, MAX_SPEED * 0.5);
  wb_motor_set_velocity(right_motor, -MAX_SPEED * 0.5);
}

// Function to stop the robot
void stop_robot() {
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
}

#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>
#include <stdio.h>
#include <stdbool.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28

// Function to check if a dead end is reached
bool is_dead_end(bool front_wall, bool left_wall, bool right_wall) {
  return front_wall && left_wall && right_wall;
}

int main(int argc, char **argv) {
  wb_robot_init();

  // Initialize motors
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  if (left_motor == 0 || right_motor == 0) {
    fprintf(stderr, "Error: Motor devices not found. Check device names.\n");
    return 1;
  }

  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Initialize distance sensors
  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  for (int ind = 0; ind < 8; ++ind) {
    sprintf(prox_sensor_name, "ps%d", ind);
    prox_sensors[ind] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[ind], TIME_STEP);
  }

  // Initialize light sensor
  WbDeviceTag ls1 = wb_robot_get_device("ls1");
  wb_light_sensor_enable(ls1, TIME_STEP);

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;

  bool dead_end_visited[9] = {false};
  int dead_end_count = 0;
  double dead_end_5_intensity = 0.0;

  double start_time = wb_robot_get_time();

  while (wb_robot_step(TIME_STEP) != -1) {
    double current_time = wb_robot_get_time();

    // Stop robot at exactly 6 minutes and 44 seconds
    if (current_time >= 404.0) {
      left_speed = 0.0;
      right_speed = 0.0;
      wb_motor_set_velocity(left_motor, left_speed);
      wb_motor_set_velocity(right_motor, right_speed);
      printf("Robot Stopped at the Brightest point with light intensity %f\n", dead_end_5_intensity);
      break;
    }

    // Read sensor values
    bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
    bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
    bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;
    bool right_wall = wb_distance_sensor_get_value(prox_sensors[2]) > 80;

    // Dead end detection
    if (is_dead_end(front_wall, left_wall, right_wall) && !dead_end_visited[dead_end_count]) {
      dead_end_visited[dead_end_count] = true;

      // Get light intensity value at the dead end
      double light_value = wb_light_sensor_get_value(ls1);
      printf("Dead end %d reached. Light intensity: %f\n", dead_end_count + 1, light_value);

      if (dead_end_count == 4) {
        dead_end_5_intensity = light_value;  // Store the light intensity for dead end 5
      }

      dead_end_count++;
    }

    // Left Wall Following Algorithm
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
    } else if (!left_wall) {
      left_speed = MAX_SPEED / 8;
      right_speed = MAX_SPEED;
    } else if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8;
    } else {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  wb_robot_cleanup();

  return 0;
}

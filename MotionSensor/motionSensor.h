#ifndef MOTION_SENSOR_H_
#define MOTION_SENSOR_H_

/**
 * Controls the motion sensor component of security camera. There is built in
 * hardware debouncing that pushes 3.3V for about 2 seconds, so this module does software
 * debouncing to get the correct state.
 * 
 * Note: Motion detector uses GPIO 31
*/

typedef enum {
    PIR_DETECT,
    PIR_NONE,
} PIRState;


// initializes the module
void MotionSensor_init(void);


// gets the state of the motion sensor
// if something is detected will return PIR_DETECT otherwise PIR_NONE
PIRState MotionSensor_getState(void);


// turns the motion sensor on or off. Returns true if it is on after toggling
bool MotionSensor_toggle(void);
bool MotionSensor_isEnabled(void);

// cleanup resources
void MotionSensor_cleanup(void);


#endif
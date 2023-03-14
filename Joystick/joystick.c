#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>

#include "joystick.h"

#include "../Utilities/utilities.h"
#include "../Timer/timer.h"

#define JOYSTICK_UP_PATH                    "/sys/class/gpio/gpio26"
#define JOYSTICK_DOWN_PATH                  "/sys/class/gpio/gpio46"
#define JOYSTICK_LEFT_PATH                  "/sys/class/gpio/gpio65"
#define JOYSTICK_RIGHT_PATH                 "/sys/class/gpio/gpio47"
#define JOYSTICK_CENTER_PATH                "/sys/class/gpio/gpio27"

#define JOYSTICK_UP_DIRECTION_PATH          "/sys/class/gpio/gpio26/direction"
#define JOYSTICK_DOWN_DIRECTION_PATH        "/sys/class/gpio/gpio46/direction"
#define JOYSTICK_LEFT_DIRECTION_PATH        "/sys/class/gpio/gpio65/direction"
#define JOYSTICK_RIGHT_DIRECTION_PATH       "/sys/class/gpio/gpio47/direction"
#define JOYSTICK_CENTER_DIRECTION_PATH      "/sys/class/gpio/gpio27/direction"

#define JOYSTICK_UP_VALUE_PATH              "/sys/class/gpio/gpio26/value"
#define JOYSTICK_DOWN_VALUE_PATH            "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT_VALUE_PATH            "/sys/class/gpio/gpio65/value"
#define JOYSTICK_RIGHT_VALUE_PATH           "/sys/class/gpio/gpio47/value"
#define JOYSTICK_CENTER_VALUE_PATH          "/sys/class/gpio/gpio27/value"

#define JSUP_GPIO                           26
#define JSRT_GPIO                           47
#define JSDN_GPIO                           46
#define JSLFT_GPIO                          65
#define JSCENTER_GPIO                       27

#define ACTIVE_LOW                          0

#define DEBOUNCE_DELAY_MS                   100

// ------------------------- PRIVATE ------------------------- //
static bool inDetectState = true;
static Timer timer;


static void configureJoystickPinToInput(const char *joystickDirectionPath) 
{
    Utilities_writeStringValueToFile("in", joystickDirectionPath);
}

static void exportPin(int gpioNumber, const char *pFilePath)
{
    Utilities_exportGpioPin(pFilePath, gpioNumber);
}

static void configureJoystickPinsToGPIO() 
{
    Utilities_runCommand("config-pin p8.14 gpio");
    Utilities_runCommand("config-pin p8.15 gpio");
    Utilities_runCommand("config-pin p8.16 gpio");
    Utilities_runCommand("config-pin p8.17 gpio");
    Utilities_runCommand("config-pin p8.18 gpio");
}

static void exportAllJoystickGPIO()
{
    exportPin(JSUP_GPIO, JOYSTICK_UP_PATH);
    exportPin(JSDN_GPIO, JOYSTICK_DOWN_PATH);
    exportPin(JSRT_GPIO, JOYSTICK_RIGHT_PATH);
    exportPin(JSLFT_GPIO, JOYSTICK_LEFT_PATH);
    exportPin(JSCENTER_GPIO, JOYSTICK_CENTER_PATH);
}

void configureAllJoystickPinsToInput() 
{
    configureJoystickPinToInput(JOYSTICK_UP_DIRECTION_PATH);
    configureJoystickPinToInput(JOYSTICK_DOWN_DIRECTION_PATH);
    configureJoystickPinToInput(JOYSTICK_LEFT_DIRECTION_PATH);
    configureJoystickPinToInput(JOYSTICK_RIGHT_DIRECTION_PATH);
}

JoystickInput getPressed(void)
{
    if (Utilities_readGpioValue(JOYSTICK_UP_VALUE_PATH) == ACTIVE_LOW) {
        return JOYSTICK_UP;
    }

    if (Utilities_readGpioValue(JOYSTICK_DOWN_VALUE_PATH) == ACTIVE_LOW) {
        return JOYSTICK_DOWN;
    }

    if (Utilities_readGpioValue(JOYSTICK_RIGHT_VALUE_PATH) == ACTIVE_LOW) {
        return JOYSTICK_RIGHT;
    }

    if (Utilities_readGpioValue(JOYSTICK_LEFT_VALUE_PATH) == ACTIVE_LOW) {
        return JOYSTICK_LEFT;
    }

    if (Utilities_readGpioValue(JOYSTICK_CENTER_VALUE_PATH) == ACTIVE_LOW) {
        return JOYSTICK_CENTER;
    }

    return JOYSTICK_NONE;
}


// ------------------------- PUBLIC ------------------------- //

JoystickInput Joystick_getPressed(void)
{
    // handle debouncing using hysteresis and timer
    JoystickInput pressed = getPressed();
    if (inDetectState) {
        if (pressed != JOYSTICK_NONE) {
            inDetectState = false;
            Timer_start(DEBOUNCE_DELAY_MS, &timer);
            return pressed;
        }
    } else {
        if (Timer_isExpired(&timer) && pressed == JOYSTICK_NONE) {
            inDetectState = true;
        }
    }
    return JOYSTICK_NONE;
}

void Joystick_init() 
{
    configureJoystickPinsToGPIO();
    exportAllJoystickGPIO();
    configureAllJoystickPinsToInput();
}

void Joystick_cleanup() 
{
    // nothing to cleanup
}
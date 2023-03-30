#ifndef STREAM_CONTROLLER_H_
#define STREAM_CONTROLLER_H_

/**
 * Controller for web cam streaming
*/

typedef enum {
    STREAM_OFF,
    STREAM_ON,
    STREAM_TRIGGER,
} StreamingOption;


// start the controller thread
void Stream_Controller_start(void);


// sets the trigger options for streaming
// void Stream_Controller_setTriggerAction(void (*callBack)(void));

void Stream_Controller_setStreamingOption(StreamingOption opt);
StreamingOption Stream_Controller_getStreamingOption(void);

// stop the controller thread and cleanup any resources
void Stream_Controller_stop(void);

#endif

/**
 * @file observer.h
 * @brief Observer pattern implementation in C
 * @author Marcos Yonamine
 * 
 * This file defines the interface for the observer pattern implementation in C.
 * 
 * All implementation was done with static memory allocation, focusing on memory safety for embedded systems.
 * 
 * Example of usage:
 * 
 * Suppose you have a temperature sensor and a humidity sensor and want to send updates
 * to multiple peripherals (e.g., display, serial, etc). You can create a subject for the temperature 
 * sensor and register peripherals callbacks to receive updates.
 * When the temperature changes, you can notify all registered observers with the new temperature value.
 * 
 * First, include the observer.h header in your C file:
 * #include "observer.h"
 * 
 * Then, inside main() function, initialize the observer module and create subjects for your sensors:
 * obs_module_init();
 * obs_subject_t * temp_subject = obs_subject_new();
 * obs_subject_t * humidity_subject = obs_subject_new();
 * 
 * Next, register your peripherals callbacks (e.g., display, serial, logger) to receive updates from the subjects:
 * obs_attatch(temp_subject, display_update_temperature);
 * obs_attatch(temp_subject, serial_update_temperature);
 * obs_attatch(temp_subject, logger_update_temperature);
 * obs_attatch(humidity_subject, display_update_humidity);
 * obs_attatch(humidity_subject, logger_update_humidity);
 * 
 * Suppose the variable 'temperature' holds the current temperature value and 'humidity' holds the current humidity value.
 * uint8_t temperature = 25;
 * uint8_t humidity = 50;
 * 
 * When the sensor values change, notify all registered peripherals with the new values:
 * obs_notify(temp_subject, (void *)&temperature);
 * obs_notify(humidity_subject, (void *)&humidity);
 * 
 * If you want to stop sending updates to a specific peripheral, you can detach it from the subject:
 * obs_detatch(temp_subject, serial_update_temperature);
 * 
 * Finally, when you are done with the subjects, make sure to destroy them:
 * obs_destroy(temp_subject);
 * obs_destroy(humidity_subject);
 * 
 * Declare the callback functions for your peripherals to handle the updates. For example:
 * void display_update_temperature(void *data) 
 * {
 *    uint8_t temp = *(uint8_t *)(data);
 *    printf("[DISPLAY] Temperature updated: %d\n", temp);
 * }
 * 
 * For more examples of callback functions, please refer to the observer_main.c file in the Examples folder.
 * 
 * Enjoy using the observer pattern in your C projects!
 */

#ifndef OBSERVER_H
#define OBSERVER_H

#if defined(__cplusplus)
extern "C" {
#endif

#define MAX_OBSERVERS 10
#define MAX_SUBJECTS 10

typedef struct obs_subject_t obs_subject_t;
typedef void (*obs_callback_t)(void *data);

void obs_module_init(void);

obs_subject_t * obs_subject_new(void);
void obs_attatch(obs_subject_t *subject, obs_callback_t callback);
void obs_detatch(obs_subject_t *subject, obs_callback_t callback);
void obs_notify(obs_subject_t *subject, void *data);
void obs_destroy(obs_subject_t *subject);

#if defined(__cplusplus)
}
#endif

#endif /* OBSERVER_H */
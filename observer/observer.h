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
 * to multiple peripherals (e.g., display, serial, etc). You can create a subject for each sensor 
 * and register peripherals callbacks to receive updates.
 * When the sensor value changes, you can notify all registered observers with the new sensor value.
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

/**
 * @brief Opaque type for the subject.
 * 
 */
typedef struct obs_subject_t obs_subject_t;

/**
 * @brief Type definition for the observer callback function.
 * The callback function takes a void pointer as an argument, which can be used as an Abstract Data Type.
 * We strongly recommend using a struct as the data type for the callback argument to ensure type safety and clarity in your code.
 * 
 */
typedef void (*obs_callback_t)(void *data);

/**
 * @brief Status codes for the observer module functions.
 * 
 */
typedef enum {
    OBS_SUCCESS = 0,
    OBS_ERROR_FULL = -1,
    OBS_ERROR_NOT_FOUND = -2,
    OBS_ERROR_INVALID = -3
} obs_status_t;

/**
 * @brief Initializes the observer module. This function must be called before using any other functions in the module.
 * @return obs_status_t Status of the initialization.
 * 
 */
obs_status_t obs_module_init(void);

/**
 * @brief Creates a new subject and returns a pointer to it. The subject is used to manage the observers and notify them of updates.
 * @return obs_subject_t* Pointer to the newly created subject, or NULL if the maximum number of subjects has been reached.
 * 
 */
obs_subject_t * obs_subject_new(void);

/**
 * @brief Attaches an observer callback to a subject. The callback will be called when the subject is notified of an update.
 * @param subject Pointer to the subject to which the observer will be attached.
 * @param callback Pointer to the callback function to be attached.
 * @return obs_status_t Status of the attachment.
 * 
 */
obs_status_t obs_attatch(obs_subject_t *subject, obs_callback_t callback);

/**
 * @brief Detaches an observer callback from a subject.
 * @param subject Pointer to the subject from which the observer will be detached.
 * @param callback Pointer to the callback function to be detached.
 * @return obs_status_t Status of the detachment.
 * 
 */
obs_status_t obs_detatch(obs_subject_t *subject, obs_callback_t callback);

/**
 * @brief Notifies all observers of a subject about an update.
 * @param subject Pointer to the subject that will notify its observers.
 * @param data Pointer to the data to be passed to the observers.
 * @return obs_status_t Status of the notification.
 * 
 */
obs_status_t obs_notify(obs_subject_t *subject, void *data);

/**
 * @brief Destroys a subject and releases its resources.
 * @param subject Pointer to the subject to be destroyed.
 * @return obs_status_t Status of the destruction.
 * 
 */
obs_status_t obs_destroy(obs_subject_t *subject);

#if defined(__cplusplus)
}
#endif

#endif /* OBSERVER_H */
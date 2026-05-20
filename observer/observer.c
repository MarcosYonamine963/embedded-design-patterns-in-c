#include "observer.h"
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @brief Maximum number of subjects.
 * 
 */
#define MAX_SUBJECTS 10

/**
 * @brief Maximum number of observers (callbacks) that can be attached to a subject.
 * 
 */
#define MAX_OBSERVERS 10

/**
 * @brief Internal structure for the observer callback. This structure is used to manage the callbacks for each subject.
 * It contains the callback function pointer and a flag to indicate if the slot is assigned or not.
 * 
 */
struct callback_t {
    obs_callback_t callback;
    uint8_t assigned;
};

/**
 * @brief Internal structure for the subject. This structure is used to manage the observers and notify them of updates.
 * It contains an array of callback_t structures, each representing a subscriber (observer), and a
 * flag to indicate if the subject slot is assigned or not.
 * An opaque type is defined in the header file.
 */
struct obs_subject_t 
{
    // array of callback structs, each representing a subscriber (observer). Static allocation to avoid malloc usage.
    struct callback_t callbacks[MAX_OBSERVERS];
    // flag to indicate if the subject slot is assigned or not.
    uint8_t assigned;
};

// static memory allocation of subjects
static struct obs_subject_t subjects[MAX_SUBJECTS];
// counter to keep track of the number of assigned subjects
static uint8_t subject_counter = 0;
// flag to indicate if the module has been initialized or not
static uint8_t module_initialized = 0;

obs_status_t obs_module_init(void)
{
    if(module_initialized) 
    {
        return OBS_ERROR_INVALID; // Module already initialized
    }

    // Initialize the subjects array, marking all slots as unassigned and setting callbacks to NULL
    for (uint8_t i = 0; i < MAX_SUBJECTS; i++) 
    {
        subjects[i].assigned = 0;
        for (uint8_t j = 0; j < MAX_OBSERVERS; j++) 
        {
            subjects[i].callbacks[j].assigned = 0;
            subjects[i].callbacks[j].callback = NULL;
        }
    }

    module_initialized = 1; // Mark the module as initialized

    return OBS_SUCCESS;
}// end obs_module_init

obs_subject_t * obs_subject_new(void) 
{
    if(subject_counter >= MAX_SUBJECTS) 
    {
        return NULL; // No more space for new subjects
    }

    uint8_t i = 0;

    // search for the first available slot in the subjects array
    for (i = 0; i < MAX_SUBJECTS; i++) 
    {
        if (subjects[i].assigned == 0) // Check if the subject slot is available
        { 
            subjects[i].assigned = 1;
            subject_counter++;
            break;
        }
    }

    // check if we found an available slot for the new subject
    if (subject_counter == 0 || i == MAX_SUBJECTS) 
    {
        return NULL; // No available subject slot found
    }

    // Initialize the callbacks array for the new subject
    for (uint8_t j = 0; j < MAX_OBSERVERS; j++) 
    {
        subjects[i].callbacks[j].assigned = 0;
        subjects[i].callbacks[j].callback = NULL;
    }

    // Create a new subject pointer and point to the newly assigned subject slot
    obs_subject_t * new_subject_ptr = &subjects[i];

    return new_subject_ptr; // Return the pointer to the newly created subject
}// end obs_subject_new

obs_status_t obs_attatch(obs_subject_t *subject, obs_callback_t callback) 
{
    if (subject == NULL || (subject)->assigned == 0) 
    {
        return OBS_ERROR_INVALID; // Subject is unassigned or null
    }

    if (callback == NULL)
    {
        return OBS_ERROR_INVALID; // Callback function is null
    }

    uint8_t i = 0;

    // verify if the callback is already attached to the subject
    for (i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 1 && (subject)->callbacks[i].callback == callback) 
        {
            return OBS_ERROR_INVALID; // Callback function is already attached to the subject
        }
    }

    // verify if there is an available slot for the new callback
    for (i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 0) // Check if the callback slot is available
        { 
            (subject)->callbacks[i].callback = callback; // Set the callback function
            (subject)->callbacks[i].assigned = 1; // Mark as assigned
            break;
        }
    }

    if (i == MAX_OBSERVERS) 
    {
        return OBS_ERROR_FULL; // No available slot for the new callback
    }

    return OBS_SUCCESS;
}// end obs_attatch

obs_status_t obs_detatch(obs_subject_t *subject, obs_callback_t callback) 
{
    if (subject == NULL || (subject)->assigned == 0) 
    {
        return OBS_ERROR_INVALID; // Subject is unassigned or null
    }
    
    if (callback == NULL) {
        return OBS_ERROR_INVALID; // Callback function is null
    }

    uint8_t i = 0;
    for (i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 1 && (subject)->callbacks[i].callback == callback) 
        {
            (subject)->callbacks[i].assigned = 0; // Mark as unassigned
            (subject)->callbacks[i].callback = NULL; // Set the callback to NULL;
            break;
        }
    }

    if (i == MAX_OBSERVERS)
    {
        return OBS_ERROR_NOT_FOUND; // Callback function not found
    }
    
    return OBS_SUCCESS;
}// end obs_detatch

obs_status_t obs_notify(obs_subject_t *subject, void *data) 
{
    if (subject == NULL || (subject)->assigned == 0) 
    {
        return OBS_ERROR_INVALID; // Subject is unassigned or null
    }    
    if (data == NULL) 
    {
        return OBS_ERROR_INVALID; // Data is null
    }    

    for (uint8_t i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 1) 
        {
            (subject)->callbacks[i].callback(data);
        }
    }

    return OBS_SUCCESS;
}// end obs_notify

obs_status_t obs_destroy(obs_subject_t *subject) 
{
    if (subject == NULL || (subject)->assigned == 0) 
    {
        return OBS_ERROR_INVALID; // Subject is already unassigned or null
    }

    uint8_t i = 0;
    for (i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 1) 
        {
            (subject)->callbacks[i].assigned = 0; // Mark as unassigned
            (subject)->callbacks[i].callback = NULL; // Set the callback to NULL
            break;
        }
    }

    (subject)->assigned = 0; // Mark the subject slot as unassigned
    subject_counter--;
    
    if(i == MAX_OBSERVERS)
    {
        return OBS_ERROR_NOT_FOUND; // Callback function not found
    }

    return OBS_SUCCESS;
}// end obs_destroy

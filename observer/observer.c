#include "observer.h"
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

struct callback_t {
    obs_callback_t callback;
    uint8_t assigned;
};

struct obs_subject_t 
{
    // array of callback structs, each representing a subscriber (observer). Static allocation to avoid malloc usage.
    struct callback_t callbacks[MAX_OBSERVERS];
    // flag to indicate if the subject slot is assigned or not.
    uint8_t assigned;
};

// static memory allocation of subjects
static struct obs_subject_t subjects[MAX_SUBJECTS];

static uint8_t subject_counter = 0;
static uint8_t module_initialized = 0;

void obs_module_init(void)
{
    if(module_initialized) {
        return; // Module already initialized
    }

    for (int i = 0; i < MAX_SUBJECTS; i++) 
    {
        subjects[i].assigned = 0;
        for (int j = 0; j < MAX_OBSERVERS; j++) 
        {
            subjects[i].callbacks[j].assigned = 0;
            subjects[i].callbacks[j].callback = NULL;
        }
    }

    module_initialized = 1; // Mark the module as initialized
}


obs_subject_t * obs_subject_new(void) 
{
    if(subject_counter >= MAX_SUBJECTS) {
        return NULL; // No more space for new subjects
    }

    // search for the first available slot in the subjects array
    int i = 0;
    for (i = 0; i < MAX_SUBJECTS; i++) {
        if (subjects[i].assigned == 0) { // Check if the subject slot is available
            subjects[i].assigned = 1;
            subject_counter++;
            break;
        }
    }

    if (subject_counter == 0 || i == MAX_SUBJECTS) 
    {
        return NULL; // No available subject slot found
    }

    obs_subject_t * new_subject_ptr = &subjects[i]; // Point to the newly assigned subject slot

    return new_subject_ptr; // Return the pointer to the newly created subject
}

void obs_attatch(obs_subject_t *subject, obs_callback_t callback) 
{
    if (subject == NULL || (subject)->assigned == 0) {
        return; // Subject is unassigned or null
    }

    if (callback == NULL) {
        return; // Callback function is null
    }

    // search for the first available slot in the callbacks array of the observer
    for (int i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 0) 
        { // Check if the callback slot is available
            (subject)->callbacks[i].callback = callback; // Set the callback function
            (subject)->callbacks[i].assigned = 1; // Mark as assigned
            break;
        }
    }
}

void obs_detatch(obs_subject_t *subject, obs_callback_t callback) 
{
    if (subject == NULL || (subject)->assigned == 0) {
        return; // Subject is unassigned or null
    }  

    for (int i = 0; i < MAX_OBSERVERS; i++) {
        if ((subject)->callbacks[i].assigned == 1 && (subject)->callbacks[i].callback == callback) {
            (subject)->callbacks[i].assigned = 0; // Mark as unassigned
            (subject)->callbacks[i].callback = NULL; // Set the callback to NULL;
            break;
        }
    }
}

void obs_notify(obs_subject_t *subject, void *data) 
{
    if (subject == NULL || (subject)->assigned == 0) {
        return; // Subject is unassigned or null
    }    

    for (int i = 0; i < MAX_OBSERVERS; i++) {
        if ((subject)->callbacks[i].assigned == 1) {
            (subject)->callbacks[i].callback(data);
        }
    }
}

void obs_destroy(obs_subject_t *subject) 
{
    if (subject == NULL || (subject)->assigned == 0) {
        return; // Subject is already unassigned or null
    }

    for (int i = 0; i < MAX_OBSERVERS; i++) 
    {
        if ((subject)->callbacks[i].assigned == 1) 
        {
            (subject)->callbacks[i].assigned = 0; // Mark as unassigned
            (subject)->callbacks[i].callback = NULL; // Set the callback to NULL
        }
    }
    (subject)->assigned = 0; // Mark the subject slot as unassigned
    subject_counter--;
}

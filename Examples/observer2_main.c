/**
 * @file observer2_main.c
 * @author Marcos Yonamine
 * 
 * This file is a simple example of how to implement the observer pattern in C.
 * 
 * Suppose you have a temperature sensor and a humidity sensor and want to send updates 
 * to multiple peripherals (e.g., display, serial, etc). 
 * You can create a subject for each sensor and register peripherals callbacks to receive updates. 
 * When the sensor values change, you can notify all registered observers with the new values.
 * 
 * Compile on windows with the following command:
 * gcc observer2_main.c ../observer/observer.c -o main.exe
 * 
 */

// ######## Include Headers ########
#include "../observer/observer.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// #####  Notification structure to hold the data to be sent to the observers ######
typedef struct
{
    char type[20]; // type of the notification (e.g., "temperature", "humidity", etc)
    float value; // value of the notification (e.g., temperature value, humidity value, etc)
}notification_t;


// #####  Callback Functions for the peripherals to handle updates ######

void display_update(void *data)
{
    notification_t *notif = (notification_t *)(data);
    printf("[DISPLAY] %s updated: %.1f\n", notif->type, notif->value);
}

void serial_update(void *data)
{
    notification_t *notif = (notification_t *)(data);
    printf("[SERIAL] %s updated: %.1f\n", notif->type, notif->value);
}

void logger_update(void *data)
{
    notification_t *notif = (notification_t *)(data);
    printf("[LOGGER] %s updated: %.1f\n", notif->type, notif->value);
}

// #######  Main function ########

int main(void)
{
    printf("Observer Pattern Example 2\n");

    // init the observer module
    obs_status_t status = obs_module_init();
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to initialize observer module. Error code: %d\n", status);
        return -1;
    }

    // create a new subject for the temperature sensor
    obs_subject_t * temp_subject = obs_subject_new();
    if(temp_subject == NULL) 
    {
        printf("Failed to create temperature subject. Maximum number of subjects reached.\n");
        return -1;
    }

    // create a new subject for the humidity sensor
    obs_subject_t * humidity_subject = obs_subject_new();
    if(humidity_subject == NULL) 
    {
        printf("Failed to create humidity subject. Maximum number of subjects reached.\n");
        return -1;
    }

    // register peripherals to receive temperature updates
    status = obs_attatch(temp_subject, display_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to attach display observer to temperature subject.\n");
        return -1;
    }
    status = obs_attatch(temp_subject, serial_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to attach serial observer to temperature subject.\n");
        return -1;
    }
    status = obs_attatch(temp_subject, logger_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to attach logger observer to temperature subject.\n");
        return -1;
    }

    // for humidiy, we only want to send updates to the display and logger, not serial
    status = obs_attatch(humidity_subject, display_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to attach display observer to humidity subject.\n");
        return -1;
    }
    status = obs_attatch(humidity_subject, logger_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to attach logger observer to humidity subject.\n");
        return -1;
    }

    // simulate temperature and humidity updates
    float temperature = 25.8; // initial temperature value
    float humidity = 50.4; // initial humidity value
    for (int i = 0; i < 5; i++) 
    {
        temperature += i * 1.5; // simulate a temperature change
        humidity +=  i * 0.7; // simulate a humidity change
        printf("\r\nUpdating temperature to %.1f and humidity to %.1f...\n", temperature, humidity);
        notification_t temp_notif = {"Temperature", temperature};
        status = obs_notify(temp_subject, (void *)&temp_notif); // notify all registered peripherals of the temperature update
        if (status != OBS_SUCCESS) 
        {
            printf("Failed to notify temperature updates.\n");
            return -1;
        }
        notification_t humidity_notif = {"Humidity", humidity};
        status = obs_notify(humidity_subject, (void *)&humidity_notif); // notify all registered peripherals of the humidity update
        if (status != OBS_SUCCESS) 
        {
            printf("Failed to notify humidity updates.\n");
            return -1;
        }

    }

    // detach the serial observer from the temperature updates
    // (let's say we don't want to send temperature updates to serial anymore)
    status = obs_detatch(temp_subject, serial_update);
    if (status != OBS_SUCCESS) 
    {
        printf("Failed to detach serial observer from temperature subject.\n");
        return -1;
    }
    printf("\r\nDetached serial observer from temperature updates.\n");

    // simulate more temperature updates to see that serial is no longer receiving them
    for (int i = 0; i < 5; i++) 
    {
        temperature += i * 1.2; // simulate a temperature change
        printf("\r\nUpdating temperature to %.1f...\n", temperature);
        notification_t temp_notif = {"Temperature", temperature};
        status = obs_notify(temp_subject, (void *)&temp_notif); // notify all registered peripherals of the temperature update
        if (status != OBS_SUCCESS) 
        {
            printf("Failed to notify temperature updates.\n");
            return -1;
        }
        // sleep(1); // wait a bit before the next update
    }

    return 0;
}

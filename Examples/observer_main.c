/**
 * @file observer_main.c
 * @author Marcos Yonamine
 * 
 * This file is a simple example of how to implement the observer pattern in C.
 * 
 * Suppose you have a temperature sensor and a humidity sensor and want to send updates 
 * to multiple peripherals (e.g., display, serial, etc). 
 * You can create a subject for the temperature sensor and register peripherals callbacks to receive updates. 
 * When the temperature changes, you can notify all registered observers with the new temperature value.
 * 
 * Compile on windows with the following command:
 * gcc observer_main.c ../observer/observer.c -o main.exe
 * 
 */

#include "../observer/observer.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//################################################################
// Example of usage of the observer pattern in C

// ###############  Callback Functions for the peripherals to handle updates ###############

void display_update_temperature(void *data)
{
    uint8_t temp = *(uint8_t *)(data);
    printf("[DISPLAY] Temperature updated: %d\n", temp);
}

void serial_update_temperature(void *data)
{
    uint8_t temp = *(uint8_t *)(data);
    printf("[SERIAL] Temperature updated: %d\n", temp);
}

void logger_update_temperature(void *data)
{
    uint8_t temp = *(uint8_t *)(data);
    printf("[LOGGER] Temperature updated: %d\n", temp);
}


void display_update_humidity(void *data)
{
    uint8_t humidity = *(uint8_t *)(data);
    printf("[DISPLAY] Humidity updated: %d\n", humidity);
}

void serial_update_humidity(void *data)
{
    uint8_t humidity = *(uint8_t *)(data);
    printf("[SERIAL] Humidity updated: %d\n", humidity);
}

void logger_update_humidity(void *data)
{
    uint8_t humidity = *(uint8_t *)(data);
    printf("[LOGGER] Humidity updated: %d\n", humidity);
}

//########################################################################################
// ###############  Main function demonstrating the observer pattern usage ###############

int main(void)
{
    printf("Observer Pattern Example\n");

    // init the observer module
    obs_module_init();

    // create a new subject for the temperature sensor
    obs_subject_t * temp_subject = obs_subject_new();

    // create a new subject for the humidity sensor
    obs_subject_t * humidity_subject = obs_subject_new();

    // register peripherals to receive temperature updates
    obs_attatch(temp_subject, display_update_temperature);
    obs_attatch(temp_subject, serial_update_temperature);
    obs_attatch(temp_subject, logger_update_temperature);

    // for humidiy, we only want to send updates to the display and logger, not serial
    obs_attatch(humidity_subject, display_update_humidity);
    obs_attatch(humidity_subject, logger_update_humidity);

    // simulate temperature and humidity updates
    uint8_t temperature = 25; // initial temperature value
    uint8_t humidity = 50; // initial humidity value
    for (int i = 0; i < 5; i++) {
        temperature = 25 + i; // simulate a temperature change
        humidity = 50 + i * 2; // simulate a humidity change
        printf("Updating temperature to %d and humidity to %d...\n", temperature, humidity);
        obs_notify(temp_subject, (void *)&temperature); // notify all registered peripherals of the temperature update
        obs_notify(humidity_subject, (void *)&humidity); // notify all registered peripherals of the humidity update
        // sleep(1); // wait a bit before the next update
    }

    // detach the serial observer from the temperature updates
    // (let's say we don't want to send temperature updates to serial anymore)
    obs_detatch(temp_subject, serial_update_temperature);
    printf("Detached serial observer from temperature updates.\n");

    // simulate more temperature updates to see that serial is no longer receiving them
    for (int i = 0; i < 5; i++) {
        temperature = 30 + i; // simulate a temperature change
        printf("Updating temperature to %d...\n", temperature);
        obs_notify(temp_subject, (void *)&temperature); // notify all registered peripherals of the temperature update
        // sleep(1); // wait a bit before the next update
    }

    return 0;
}

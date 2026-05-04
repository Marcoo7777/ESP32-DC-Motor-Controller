#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define ENPIN 4
#define PHASE 5
#define BUTTON 6

void pwm_init(){
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 20000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel_conf = {
        .gpio_num = ENPIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_conf);

    gpio_set_direction(PHASE, GPIO_MODE_OUTPUT);
}
void motor_set(int speed, int direction)
{

    gpio_set_level(PHASE, direction);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void app_main(void)
{

    pwm_init();
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);

    int increments = 0;

    int last_button_state = 1;

    int speed = 0;
    int direction = 1;
    
    while (1) {

        int current_button_state = gpio_get_level(BUTTON);

        if (last_button_state == 1 && current_button_state == 0) {
            increments++;

            vTaskDelay(pdMS_TO_TICKS(30));
        }

        last_button_state = current_button_state;

        if (current_button_state == 0) { 

            if (direction == 1) {
                speed += 5;
                if (speed >= 255) {
                    speed = 255;
                    direction = 0;
                }
            } else {
                speed -= 5;
                if (speed <= 0) {
                    speed = 0;
                    direction = 1;
                }
        }
    }

        motor_set(speed, increments % 2);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
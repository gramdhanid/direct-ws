/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#include "device_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// void change_switch_state(int switch_state)
// {
//     if (switch_state == SWITCH_OFF) {
//         gpio_set_level(GPIO_OUTPUT_RELAY, RELAY_GPIO_OFF);
//     } else {
//         gpio_set_level(GPIO_OUTPUT_RELAY, RELAY_GPIO_ON);
//     }
// }
void change_switch_state2(int switch_state2)
{
    if (switch_state2 == SWITCH_OFF) {
        gpio_set_level(GPIO_OUTPUT2_RELAY1, RELAY_GPIO_OFF);
        gpio_set_level(GPIO_OUTPUT2_RELAY2, RELAY_GPIO_OFF);
    } else {
        gpio_set_level(GPIO_OUTPUT2_RELAY1, RELAY_GPIO_ON);
        gpio_set_level(GPIO_OUTPUT2_RELAY2, RELAY_GPIO_ON);
    }
}

// void change_switch_state3(int switch_state3)
// {
//     if (switch_state3 == SWITCH_OFF) {
//         gpio_set_level(GPIO_OUTPUT3_RELAY1, RELAY_GPIO_OFF);
//         gpio_set_level(GPIO_OUTPUT3_RELAY2, RELAY_GPIO_OFF);
//         gpio_set_level(GPIO_OUTPUT3_RELAY3, RELAY_GPIO_OFF);
//     } else {
//         gpio_set_level(GPIO_OUTPUT3_RELAY1, RELAY_GPIO_ON);
//         gpio_set_level(GPIO_OUTPUT3_RELAY2, RELAY_GPIO_ON);
//         gpio_set_level(GPIO_OUTPUT3_RELAY3, RELAY_GPIO_ON);
//     }
// }
       

void change_led_state(int led_state)
{
    if (led_state == LED_OFF) {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_OFF);
    } else {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_ON);
    }
}

int get_button_event(int* button_event_type, int* button_event_count)
{
    // static uint32_t button_count1 = 0;
    static uint32_t button_count2 = 0;
    // static uint32_t button_count3 = 0;
    // static uint32_t button_last_state1 = BUTTON_GPIO_RELEASED;
    static uint32_t button_last_state2 = BUTTON_GPIO_RELEASED;
    // static uint32_t button_last_state3 = BUTTON_GPIO_RELEASED;
    static TimeOut_t button_timeout;
    static TickType_t long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
    static TickType_t button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);

    // uint32_t gpio_level1 = 0;
    uint32_t gpio_level2 = 0;
    // uint32_t gpio_level3 = 0;

    // gpio_level1 = gpio_get_level(GPIO_INPUT_BUTTON);

    gpio_level2 = gpio_get_level(GPIO_INPUT2_BUTTON1);
    gpio_level2 = gpio_get_level(GPIO_INPUT2_BUTTON2);

    // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON1);
    // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON2);
    // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON3);

    // if (button_last_state1 != gpio_level1 || button_last_state2 != gpio_level2 || button_last_state3 != gpio_level3)
    if (button_last_state2 != gpio_level2)
    // if (button_last_state1 != gpio_level1)
    {
        /* wait debounce time to ignore small ripple of currunt */
        vTaskDelay( pdMS_TO_TICKS(BUTTON_DEBOUNCE_TIME_MS) );
        // gpio_level1 = gpio_get_level(GPIO_INPUT_BUTTON);

        gpio_level2 = gpio_get_level(GPIO_INPUT2_BUTTON1);
        gpio_level2 = gpio_get_level(GPIO_INPUT2_BUTTON2);

        // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON1);
        // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON2);
        // gpio_level3 = gpio_get_level(GPIO_INPUT3_BUTTON3);

        // if (button_last_state1 != gpio_level1) 
        // {
        //     printf("Button event, val: %d, tick: %u\n", gpio_level1, (uint32_t)xTaskGetTickCount());
        //     button_last_state1 = gpio_level1;
        //     if (gpio_level1 == BUTTON_GPIO_PRESSED1) {
        //         button_count1++;
        //     }
        //     vTaskSetTimeOutState(&button_timeout);
        //     button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
        //     long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
        // }
        if (button_last_state2 != gpio_level2)
        {
            printf("Button event2, val: %d, tick: %u\n", gpio_level2, (uint32_t)xTaskGetTickCount());
            button_last_state2 = gpio_level2;
            if (gpio_level2 == BUTTON_GPIO_PRESSED) 
            {
                button_count2++;
            }
            vTaskSetTimeOutState(&button_timeout);
            button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
            long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
        }
        // else if (button_last_state3 != gpio_level3)
        // {
        //     printf("Button event3, val: %d, tick: %u\n", gpio_level3, (uint32_t)xTaskGetTickCount());
        //     button_last_state3 = gpio_level3;
        //     if (gpio_level3 == BUTTON_GPIO_PRESSED) {
        //         button_count3++;
        //     }
        //     vTaskSetTimeOutState(&button_timeout);
        //     button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
        //     long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
        // }

    } 
    // else if (button_count1 > 0) 
    // {
    //     if ((gpio_level1 == BUTTON_GPIO_PRESSED)
    //             && (xTaskCheckForTimeOut(&button_timeout, &long_press_tick ) != pdFALSE)) 
    //             {
    //         *button_event_type = BUTTON_LONG_PRESS;
    //         *button_event_count = 1;
    //         button_count1 = 0;
    //         return true;
    //     } else if ((gpio_level1 == BUTTON_GPIO_RELEASED)
    //             && (xTaskCheckForTimeOut(&button_timeout, &button_delay_tick ) != pdFALSE)) 
    //             {
    //         *button_event_type = BUTTON_SHORT_PRESS;
    //         *button_event_count = button_count1;
    //         button_count1 = 0;
    //         return true;
    //     }
    // }
    else if (button_count2 > 0) 
    {
        if ((gpio_level2 == BUTTON_GPIO_PRESSED)
                && (xTaskCheckForTimeOut(&button_timeout, &long_press_tick ) != pdFALSE)) 
                {
            *button_event_type = BUTTON_LONG_PRESS;
            *button_event_count = 1;
            button_count2 = 0;
            return true;
        } else if ((gpio_level2 == BUTTON_GPIO_RELEASED)
                && (xTaskCheckForTimeOut(&button_timeout, &button_delay_tick ) != pdFALSE)) 
                {
            *button_event_type = BUTTON_SHORT_PRESS;
            *button_event_count = button_count2;
            button_count2 = 0;
            return true;
        }
    }
    // else if (button_count3 > 0) 
    // {
    //     if ((gpio_level3 == BUTTON_GPIO_PRESSED1)
    //             && (xTaskCheckForTimeOut(&button_timeout, &long_press_tick ) != pdFALSE)) 
    //             {
    //         *button_event_type = BUTTON_LONG_PRESS;
    //         *button_event_count = 1;
    //         button_count3 = 0;
    //         return true;
    //     } else if ((gpio_level3 == BUTTON_GPIO_RELEASED1)
    //             && (xTaskCheckForTimeOut(&button_timeout, &button_delay_tick ) != pdFALSE)) 
    //             {
    //         *button_event_type = BUTTON_SHORT_PRESS;
    //         *button_event_count = button_count3;
    //         button_count3 = 0;
    //         return true;
    //     }
    // }



    return false;
}

void led_blink(int switch_state, int delay, int count)
{
    for (int i = 0; i < count; i++) {
        vTaskDelay(delay / portTICK_PERIOD_MS);
        change_led_state(1 - switch_state);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        change_led_state(switch_state);
    }
}

void change_led_mode(int noti_led_mode)
{
    static TimeOut_t led_timeout;
    static TickType_t led_tick = -1;
    static int last_led_mode = -1;
    static int led_state = SWITCH_OFF;

    if (last_led_mode != noti_led_mode) {
        last_led_mode = noti_led_mode;
        vTaskSetTimeOutState(&led_timeout);
        led_tick = 0;
    }

    switch (noti_led_mode)
    {
        case LED_ANIMATION_MODE_IDLE:
            break;
        case LED_ANIMATION_MODE_SLOW:
            if (xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE) {
                led_state = 1 - led_state;
                change_led_state(led_state);
                vTaskSetTimeOutState(&led_timeout);
                if (led_state == SWITCH_ON) {
                    led_tick = pdMS_TO_TICKS(200);
                } else {
                    led_tick = pdMS_TO_TICKS(800);
                }
            }
            break;
        case LED_ANIMATION_MODE_FAST:
            if (xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE) {
                led_state = 1 - led_state;
                change_led_state(led_state);
                vTaskSetTimeOutState(&led_timeout);
                led_tick = pdMS_TO_TICKS(100);
            }
            break;
        default:
            break;
    }
}

void iot_gpio_init(void)
{
	gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
	io_conf.mode = GPIO_MODE_INPUT;
    // io_conf.pin_bit_mask = (1ULL << GPIO_INPUT_BUTTON);
	io_conf.pin_bit_mask = (1ULL << GPIO_INPUT2_BUTTON1);
    io_conf.pin_bit_mask = (1ULL << GPIO_INPUT2_BUTTON2);

    // io_conf.pin_bit_mask = (1ULL << GPIO_INPUT3_BUTTON1);
    // io_conf.pin_bit_mask = (1ULL << GPIO_INPUT3_BUTTON2);
    // io_conf.pin_bit_mask = (1ULL << GPIO_INPUT3_BUTTON3);

	io_conf.pull_down_en = 0x1;
	io_conf.pull_up_en = 0x0;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_MAINLED;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_MAINLED_0;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
    // io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_RELAY;
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT2_RELAY1;
    io_conf.pin_bit_mask = 1 << GPIO_OUTPUT2_RELAY2;

    // io_conf.pin_bit_mask = 1 << GPIO_OUTPUT3_RELAY1;
    // io_conf.pin_bit_mask = 1 << GPIO_OUTPUT3_RELAY2;
    // io_conf.pin_bit_mask = 1 << GPIO_OUTPUT3_RELAY3;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_NOUSE2;
	gpio_config(&io_conf);

	// gpio_set_intr_type(GPIO_INPUT_BUTTON2, GPIO_INTR_ANYEDGE);
    // gpio_set_intr_type(GPIO_INPUT_BUTTON3, GPIO_INTR_ANYEDGE);
    // gpio_set_intr_type(GPIO_INPUT_BUTTON, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_INPUT2_BUTTON1, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_INPUT2_BUTTON2, GPIO_INTR_ANYEDGE);
    // gpio_set_intr_type(GPIO_INPUT3_BUTTON1, GPIO_INTR_ANYEDGE);
    // gpio_set_intr_type(GPIO_INPUT3_BUTTON2, GPIO_INTR_ANYEDGE);
    // gpio_set_intr_type(GPIO_INPUT3_BUTTON3, GPIO_INTR_ANYEDGE);

	gpio_install_isr_service(0);

	gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_ON);
	gpio_set_level(GPIO_OUTPUT_MAINLED_0, 0);

}




#include "config.h"

#define OFF 0
#define ON 1

int led_status[] = {0, 0, 0};

void led_init() {
    led_clear(BLUE);
    led_clear(GREEN);
    led_clear(ORANGE);
    UART_PRINT("[!] LEDs initialized\r\n");
}

void led_set(enum led_enum led) {
    switch (led) {
        case BLUE:
            led_status[BLUE] = ON;
            MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_7, GPIO_PIN_7);
            break;
        case GREEN:
            led_status[GREEN] = ON;
            MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_0, GPIO_PIN_0);
            break;
        case ORANGE:
            led_status[ORANGE] = ON;
            MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_6, GPIO_PIN_6);
            break;
        default:
            break;
    }
}

void led_clear(enum led_enum led) {
    switch (led) {
        case BLUE:
            led_status[BLUE] = OFF;
            MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_7, 0);
            break;
        case GREEN:
            led_status[GREEN] = OFF;
            MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_0, 0);
            break;
        case ORANGE:
            led_status[ORANGE] = OFF;
            MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_6, 0);
            break;
        default:
            break;
    }
}

void led_toggle(enum led_enum led) {
    if (led_status[led])
        led_clear(led);
    else
        led_set(led);
}

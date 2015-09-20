#ifndef __LED_H__
#define __LED_H__

enum led_enum {BLUE, GREEN, ORANGE};

void led_init();

void led_set(enum led_enum);
void led_clear(enum led_enum);
void led_toggle(enum led_enum);

#endif

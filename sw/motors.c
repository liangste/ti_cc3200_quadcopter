#include "config.h"
#include "stdint.h"

void _update_duty_cycle(
    unsigned long ulBase,
    unsigned long ulTimer,
    uint8_t ucLevel)
{
    //
    // Match value is updated to reflect the new dutycycle settings
    //
    MAP_TimerMatchSet(ulBase,ulTimer,(ucLevel*DUTYCYCLE_GRANULARITY));
}

void _setup_timer_pwm_mode(
    unsigned long ulBase,
    unsigned long ulTimer,
    unsigned long ulConfig,
    unsigned char ucInvert)
{
    //
    // Set GPT - Configured Timer in PWM mode.
    //
    MAP_TimerConfigure(ulBase, ulConfig);
    MAP_TimerPrescaleSet(ulBase, ulTimer, 0);

    //
    // Inverting the timer output if required
    //
    MAP_TimerControlLevel(ulBase, ulTimer, ucInvert);

    //
    // Load value set to ~0.5 ms time period
    //
    MAP_TimerLoadSet(ulBase,ulTimer,TIMER_INTERVAL_RELOAD);

    //
    // Match value set so as to output level 0
    //
    MAP_TimerMatchSet(ulBase,ulTimer,TIMER_INTERVAL_RELOAD);

}

void motors_init() {
    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK); // m3
    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK); // m4
    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK); // m1 & m2

    _setup_timer_pwm_mode(TIMERA0_BASE, TIMER_A,
            (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM), 1);

    _setup_timer_pwm_mode(TIMERA2_BASE, TIMER_B,
            (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM), 1);

    _setup_timer_pwm_mode(TIMERA3_BASE, TIMER_A,
            (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM), 1);

    _setup_timer_pwm_mode(TIMERA3_BASE, TIMER_B,
            (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM), 1);

    MAP_TimerEnable(TIMERA0_BASE,TIMER_A);
    MAP_TimerEnable(TIMERA2_BASE,TIMER_B);
    MAP_TimerEnable(TIMERA3_BASE,TIMER_A);
    MAP_TimerEnable(TIMERA3_BASE,TIMER_B);

    MAP_GPIOPinWrite(GPIOA3_BASE, GPIO_PIN_4, GPIO_PIN_4); // enable AND-gate bypass

    motors_set_m1(0);
    motors_set_m2(0);
    motors_set_m3(0);
    motors_set_m4(0);
}

uint8_t motors_correct_throttle(int32_t throttle)
{
    if (throttle > MAX_THROTTLE_VALUE) {
        return MAX_THROTTLE_VALUE;
    } else if (throttle < MIN_THROTTLE_VALUE) {
        return MIN_THROTTLE_VALUE;
    }
    return (uint8_t) throttle;
}

void motors_set_m1(uint8_t val) {
    _update_duty_cycle(TIMERA3_BASE, TIMER_A, val);
}

void motors_set_m2(uint8_t val) {
    _update_duty_cycle(TIMERA0_BASE, TIMER_A, val);
}

void motors_set_m3(uint8_t val) {
    _update_duty_cycle(TIMERA3_BASE, TIMER_B, val);
}

void motors_set_m4(uint8_t val) {
    _update_duty_cycle(TIMERA2_BASE, TIMER_B, val);
}

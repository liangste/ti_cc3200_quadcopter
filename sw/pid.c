#include "config.h"
#include "pid.h"
#include "motors.h"

void pid_init(pid_data_t* p, float k_p, float k_i, float k_d) {
    p->k_p = k_p;
    p->k_i = k_i;
    p->k_d = k_d;
    p->error_sum = 0.0f;
    p->value = 0;
}

void pid_update(pid_data_t* p, double desired, double measured, double rate) {
    double error = desired - measured;
    p->error_sum += error;

    double tmp = p->k_p * error + p->k_i * p->error_sum + p->k_d * rate;
    p->value = (int32_t)(tmp);
}

// get correction value
int32_t pid_get_value(pid_data_t* p) {
    return p->value;
}

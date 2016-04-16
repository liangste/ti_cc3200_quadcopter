#include "kf.h"

// reset the kalman filter
void kalman_reset(kalman_filter_t* kf)
{
    kf->angle = 0.0f;
    kf->bias = 0.0f;

    kf->P[0][0] = 0.0f;
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 0.0f;
}

// set variances to kalman filter
void kalman_set_variances(
    kalman_filter_t* kf,
    float q_angle,
    float q_bias,
    float r_measure) {

    kf->q_angle = q_angle;
    kf->q_bias = q_bias;
    kf->r_measure = r_measure;
}

// update filter structure with new data
void kalman_update(
    kalman_filter_t* kf,
    float new_angle,
    float new_rate,
    float dt) {

    kf->rate = new_rate - kf->bias;
    kf->angle += dt * kf->rate;

    kf->P[0][0] += dt * (dt * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->q_angle);
    kf->P[0][1] -= dt * kf->P[1][1];
    kf->P[1][0] -= dt * kf->P[1][1];
    kf->P[1][1] += dt * kf->q_bias;

    float S = kf->P[0][0] + kf->r_measure;

    float K[2];
    K[0] = kf->P[0][0] / S;
    K[1] = kf->P[1][0] / S;

    float y = new_angle - kf->angle;

    kf->angle += K[0] * y;
    kf->bias += K[1] * y;

    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];

    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;
}

// get estimated angle from filter
float kalman_get_angle(kalman_filter_t* kf) {
    return kf->angle;
}

#ifndef __AHRS_H__
#define __AHRS_H__

extern volatile float beta;				// algorithm gain
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame

void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void ahrs_update(float, float, float, float, float, float);
void ahrs_get_rpy(float*, float*, float*);

#endif

#ifndef __MATH2_H
#define __MATH2_H
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "arm_math.h"

float Quick_Sqrt(float num);
void Ramp_Calc(float *output, float input, float min_value, float max_value);
void Steep_Calc(float *output, float *output_tmp, float input, float min_value, float max_value);
void Int16_Constrain(int16_t* data, int16_t min_value, int16_t max_value);
int16_t Int16_Limit(int16_t data, int16_t min_value, int16_t max_value);
void Float_Constrain(float* data, float min_value, float max_value);
float Hex4_To_Float1(unsigned char *array);
uint16_t uint8_t_Array_To_uint16_t(uint8_t *Array);
float String_To_Float(char *array);
void Handle_Angle8191_PID_Over_Zero(float *tar, float *cur);
void Handle_Angle360_PID_Over_Zero(float *tar, float *cur);
float GM6020_YAW_Angle_To_360(uint16_t gm6020_angle);
void Pitch_Angle_Limit(float* angle, float down_angle, float up_angle);

void Bubble_Sort(uint8_t array[], const uint8_t num);
#endif

/*
 * main.h
 *
 *  Created on: Jun 21, 2016
 *      Author: DiLuka
 */

#ifndef MAIN_H_
#define MAIN_H_


typedef struct command_s
{
    char op;
    uint8_t code;
    uint8_t have_x;
    float x_axis;
    uint8_t have_y;
    float y_axis;
    uint8_t have_z;
    float z_axis;
} command_t;

#endif /* MAIN_H_ */

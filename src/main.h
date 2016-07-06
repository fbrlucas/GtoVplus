/*
 * main.h
 *
 *  Created on: Jun 21, 2016
 *      Author: DiLuka
 */

#ifndef MAIN_H_
#define MAIN_H_

#define FAST_SPEED "60"
#define SLOW_SPEED "40"

typedef struct command_s
{
    char op;
    uint8_t code;
    //uint8_t have_x;
    //float x_axis;
    //uint8_t have_y;
    //float y_axis;
    uint8_t axis[3]; //Precisa?????????
    float value[3];
} command_t;

#endif /* MAIN_H_ */

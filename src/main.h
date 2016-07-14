/*
 * main.h
 *
 *  Created on: Jun 21, 2016
 *      Author: DiLuka
 */

#ifndef MAIN_H_
#define MAIN_H_

#define FAST_SPEED "30"
#define SLOW_SPEED "20" //Not Used
#define Z_SAFE 0
#define Z_CUT 15

typedef struct command_s
{
    char op;
    uint8_t code;
    float value[3];
} command_t;

#endif /* MAIN_H_ */

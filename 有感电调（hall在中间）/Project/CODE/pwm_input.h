
#ifndef _pwm_input_h
#define _pwm_input_h



#include "common.h"


typedef struct
{
    uint16 frequency;   // ????
    uint16 period;      // ????
    uint16 high_value;  // ????????
    uint16 high_time;   // ??????? us
    uint16 throttle;    // ??????
}pwmin_struct;


extern pwmin_struct pwmin;

void pwm_input_init(void);
#endif

#include "headfile.h"
#include "common.h"
#include "board.h"
#define AL_IO P21
#define BL_IO P23
#define CL_IO P25
#define LEDA P32
#define LEDB P33
#define LEDC P50
#define Clockwise_direction 1
#define Counterclockwise_direction 0
void StepMotor(int direction);
void step_test();
void ESC_init();
void PWM_IO_SET0();
void StartMotor(void);
void StepMotor_Force(void);
void Speed_Init();
void step_test_ex();
int clip(int num,int min,int max);
typedef struct
{
  int16 aimspeed;    //目标速度
	int16 setspeed;    //设定速度
	uint16 setspeed_basic;
	int16 speed_now;

	uint16 Kp;
	uint16 Ki;
	uint16 Kd;
	int32 speed_pwm;

	int16 error;
	int16 last_error;
	int16 pre_error;
} Speed_Pram;
extern Speed_Pram speed;
int32 speed_increase_pid(Speed_Pram *p);
#ifndef NEXT_MOTION_H
	#define NEXT_MOTION_H
#include "Arduino.h"

class Wheel
{
  volatile int dir;
  volatile long micros_last=0;
  volatile long micros_cur=2000000;
  volatile float last_omega = 0;
  volatile float omega = 0;

  public:
  uint8_t int_pin;
  uint8_t sign_pin;
  int ENC_COUNT = 960;
  float r = 0.0318;
  volatile long pos=0;

  Wheel(uint8_t _int_pin,uint8_t _sign_pin);
  void encUpdate();
  float getOmega();
  void flush();
  void set_wheel_radius(float _r);
  void set_encoder_count(int _ENC_COUNT);
};

class Motor
{
  uint8_t EN;
  uint8_t PH;
  public:
  static const uint8_t FORWARD = 1;
  static const uint8_t BACKWARD = 0;

  Motor(uint8_t _EN, uint8_t _PH);
  void set_pins(uint8_t _EN, uint8_t _PH);
  void go(float pwm);
  void stop();
};

class Pid
{
  float val = 0;
  public:
  float kp = 0.2;
  float ki = 0.1;
  float ie = 0;
  float IE_LIMIT = 0.5;
  float RETURN_LIMIT = 255;

  float getVal(float error);
  void flush();
};


class Next_bot_motion
{
	const uint8_t W_L_A = 2;
	const uint8_t W_L_B = 16;
	const uint8_t W_R_A = 3;
	const uint8_t W_R_B = 17;
	const uint8_t M_L_EN = 6;
	const uint8_t M_L_PH = 7;
	const uint8_t M_R_EN = 5;
	const uint8_t M_R_PH = 4;
	
	const int MOVE_TO = 1;
	const int ROTATE_TO = 2;
	const int WHEEL_OMEGA = 3;
  
	int stability = 0;
	long posRefL=0;
	long posRefR=0;
	float omegaRefL=0;
	float omegaRefR=0;
	
	float pwmL=0;
	float pwmR=0;
  float tmp;
  float R = 0.135;
  public:
  const int STOP = 0;
  const int MOVE = 4;
  volatile uint8_t mode = 0;
	Pid pid[6];
	Wheel* wheel_l;
	Wheel* wheel_r;
	Motor* motor_l;
	Motor* motor_r;

	void begin();
  void stop();
	void flush_all();
	void move_to(float dis);
  void move();
	void rotate_to(float angle);
  void wheel_omega(float omega_l,float omega_r);
	uint8_t updt();
};

#endif

#include "next_motion.h"

Wheel::Wheel(uint8_t _int_pin,uint8_t _sign_pin)
{
	int_pin = _int_pin;
	sign_pin = _sign_pin;
	pinMode(sign_pin,INPUT);
    pinMode(int_pin, INPUT_PULLUP);
}

void Wheel::encUpdate()
{
    micros_last = micros_cur;
    micros_cur = micros();
    if(digitalRead(sign_pin))
    {
      pos++;
      dir = 1;
    }
    else
    {
      pos--;
      dir = -1;
    }
}

float Wheel::getOmega()
{
    if((micros()-micros_cur) > 100000)
      return 0;
    else
      return dir*((1000000*2*3.1415/ENC_COUNT)/((double)(micros_cur-micros_last)));
}

void Wheel::flush()
{
    pos = 0;
}

void Wheel::set_wheel_radius(float _r)
{
	r = _r;
}

void Wheel::set_encoder_count(int _ENC_COUNT)
{
	ENC_COUNT = _ENC_COUNT;
}
  
Motor::Motor(uint8_t _EN,uint8_t _PH)
{
	PH = _PH;
	EN = _EN;
	pinMode(EN,OUTPUT);
	pinMode(PH,OUTPUT);
}

void Motor::go(float pwm)
{
  uint8_t direction = 1;
	if(pwm<0)
  {
		direction = 0;
    pwm *= -1;
  }
	pwm = constrain(pwm,0,255);
	digitalWrite(PH,direction);
	analogWrite(EN,pwm);
}

void Motor::set_pins(uint8_t _EN,uint8_t _PH)
{
	PH = _PH;
	EN = _EN;
	pinMode(EN,OUTPUT);
	pinMode(PH,OUTPUT);
}
  
void Motor::stop()
{
    analogWrite(EN,0);
}

float Pid::getVal(float error)
{
    if(abs(error) <= 10000)  
      ie = constrain(ie + error,-IE_LIMIT,IE_LIMIT);
    if(error == 0)
      ie = 0;
    val = constrain(((kp*error) + (ki*ie)),-RETURN_LIMIT,RETURN_LIMIT);
    return val;
}

void Pid::flush()
{
    ie = 0;
}


void Next_bot_motion::begin()
{
	wheel_l = new Wheel(W_L_A,W_L_B);
	wheel_r = new Wheel(W_R_A,W_R_B);
	motor_l = new Motor(M_L_EN,M_L_PH);
	motor_r = new Motor(M_R_EN,M_R_PH);

  pid[4].kp = 200;
  pid[4].ki = 3;
  pid[4].IE_LIMIT = 30;
  pid[4].RETURN_LIMIT = 250;
  
  pid[3].kp = 200;
  pid[3].ki = 3;
  pid[3].IE_LIMIT = 30;
  pid[3].RETURN_LIMIT = 250;
  
  pid[0].kp = 0.02;
  pid[0].ki = 0.0004;
  pid[0].IE_LIMIT = 1000;
  pid[0].RETURN_LIMIT = 4;

  pid[1].kp = 0.02;
  pid[1].ki = 0.0004;
  pid[1].IE_LIMIT = 1000;
  pid[1].RETURN_LIMIT = 4;

  pid[2].kp = 0.05;
  pid[2].ki = 0.0005;
  pid[2].IE_LIMIT = 1000;
  pid[2].RETURN_LIMIT = 4;
}

void Next_bot_motion::stop()
{
  motor_l->stop();
  motor_r->stop();
}

void Next_bot_motion::flush_all()
{
    wheel_l->flush();
    wheel_r->flush();
    pid[0].flush();
    pid[1].flush();
    pid[2].flush();
    pid[3].flush();
    pid[4].flush();
    pid[5].flush();
}
  
void Next_bot_motion::move_to(float dis)
{
    posRefL = dis*(wheel_l->ENC_COUNT/(2*3.1415*wheel_l->r));
    posRefR = dis*(wheel_r->ENC_COUNT/(2*3.1415*wheel_r->r));
    mode = MOVE_TO;
    flush_all();
}

void Next_bot_motion::rotate_to(float angle)
{
    posRefL = angle*R*(wheel_l->ENC_COUNT/(2*3.1415*wheel_l->r));
    posRefR = -angle*R*(wheel_r->ENC_COUNT/(2*3.1415*wheel_r->r));
    mode = ROTATE_TO;
    flush_all();
}

void Next_bot_motion::wheel_omega(float omega_l, float omega_r)
{
  omegaRefL = omega_l;
  omegaRefR = omega_r;
  mode = WHEEL_OMEGA;
}

void Next_bot_motion::move()
{
    mode = MOVE;
    posRefL = 9000000;
    posRefR = 9000000;
    flush_all();
// motor_l->go(s);
// motor_r->go(s-(s <  0 ? -30 : 30));

}

  
uint8_t Next_bot_motion::updt()
{
    if(mode == MOVE)
    {
      tmp = pid[2].getVal(wheel_l->pos - wheel_r->pos);
      omegaRefL = pid[0].getVal(posRefL -(wheel_l->pos)) - tmp;
      omegaRefR = pid[1].getVal(posRefR -(wheel_r->pos)) + tmp;

      pwmL = pid[3].getVal(omegaRefL - wheel_l->getOmega());
      pwmR = pid[4].getVal(omegaRefR - wheel_r->getOmega());
      
      motor_l->go(pwmL);
      motor_r->go(pwmR);
      
      if(abs(pwmR) <= 70 && abs(pwmL) <= 70)
      {
        if(stability++ >= 1000)
        {
          mode = STOP;
        }
      }
      else
      {
        stability = 0;
      }
      return 0;
    }
    
    if(mode == MOVE_TO)
    {
      tmp = pid[2].getVal(wheel_l->pos - wheel_r->pos);
      omegaRefL = pid[0].getVal(posRefL -(wheel_l->pos)) - tmp;
      omegaRefR = pid[1].getVal(posRefR -(wheel_r->pos)) + tmp;

      pwmL = pid[3].getVal(omegaRefL - wheel_l->getOmega());
      pwmR = pid[4].getVal(omegaRefR - wheel_r->getOmega());
      
      motor_l->go(pwmL);
      motor_r->go(pwmR);
      
      if(abs(pwmR) <= 70 && abs(pwmL) <= 70)
      {
        if(stability++ >= 1000)
        {
          mode = STOP;
        }
      }
      else
      {
        stability = 0;
      }
      return 0;
    }

    else if(mode == ROTATE_TO)
    {

      tmp = pid[2].getVal((wheel_l->pos + wheel_r->pos)/2);
      omegaRefL = pid[0].getVal(posRefL -(wheel_l->pos)) - tmp;
      omegaRefR = pid[1].getVal(posRefR -(wheel_r->pos)) - tmp;

      pwmL = pid[3].getVal(omegaRefL - wheel_l->getOmega());
      pwmR = pid[4].getVal(omegaRefR - wheel_r->getOmega());
      
      motor_l->go(pwmL);
      motor_r->go(pwmR);
      
      if(abs(pwmR) <= 70 && abs(pwmL) <= 70)
      {
        if(stability++ >= 1000)
        {
          mode = STOP;
        }
      }
      else
      {
        stability = 0;
      }
      return 0;
    }

    else if(mode == WHEEL_OMEGA)
    {
      pwmL = pid[3].getVal(omegaRefL - wheel_l->getOmega());
      motor_l->go(pwmL);
      pwmR = pid[4].getVal(omegaRefR - wheel_r->getOmega());
      motor_r->go(pwmR);
    }
    
    else
    {
      motor_r->stop();
      motor_l->stop();
      return 1;
    }
}

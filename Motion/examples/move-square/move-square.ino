#include "Motion.h"
Motion motion;

void setup()
{
    motion.begin();
    attachInterrupt(digitalPinToInterrupt(motion.wheel_l->int_pin), isr_encoder1_process, RISING);
    attachInterrupt(digitalPinToInterrupt(motion.wheel_r->int_pin), isr_encoder2_process, RISING);
    Serial.begin(9600);

    delay(5000);
}

void loop()
{
  motion.move_to(0.1);
  while(!motion.updt());
  motion.rotate_to(1.5708);
  while(!motion.updt());
  motion.move_to(0.1);
  while(!motion.updt());
  motion.rotate_to(1.5708);
  while(!motion.updt());
  motion.move_to(0.1);
  while(!motion.updt());
  motion.rotate_to(1.5708);
  while(!motion.updt());
  motion.move_to(0.1);
  while(!motion.updt());
  motion.rotate_to(1.5708);
  while(!motion.updt());
  while(1)
  {
    
  }
}

void isr_encoder1_process(void)
{
  motion.wheel_l->encUpdate();
}

void isr_encoder2_process(void)
{
  motion.wheel_r->encUpdate();
}


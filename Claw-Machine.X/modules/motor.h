#ifndef MOTOR_H
#define	MOTOR_H

void MotorConfig();
void Tmr0Config();
void Int2Config();
void Grab();
int  isGrabbing();
void magnet_on();

void magnet_off();

void motor_turn(int delay);


#endif	/* MOTOR_H */


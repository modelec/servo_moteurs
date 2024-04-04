#include "servo_motor.h"

// Fonction pour initialiser le PCA9685
void initPCA9685(int handle)
{
    // Mettre le PCA9685 en mode de sommeil
    i2cWriteByteData(handle, MODE1_REG, 0x10);

    // Configurer la fréquence PWM (par exemple, 50 Hz)
    int prescale_val = 25000000 / (4096 * 50) - 1;
    i2cWriteByteData(handle, PRE_SCALE, prescale_val);

    // Mettre le PCA9685 en mode de travail normal
    i2cWriteByteData(handle, MODE1_REG, 0x00);
}

// Fonction pour définir la position du servo
void setServoPosition(int handle, int channel, int position)
{
    if (position < 0 || position > 180){
	return;
    }
    int on_time = SERVO_MIN + (SERVO_MAX - SERVO_MIN) * position / 180 - 1;//temps ou le servo est allumé par rapport à 4096
    int on_time_lsb = on_time & ~(0b1111 << 8);// 8 premiers bits de poids faible du temps ou le servo est allumé
    int on_time_msb = on_time >> 8;// 4 premiers bits poids fort de on_time
    i2cWriteWordData(handle, LED0_ON_L + 4 * channel, 0);
    i2cWriteByteData(handle, LED0_ON_H + 4* channel, 0);
    i2cWriteByteData(handle, LED0_OFF_L + 4 * channel, on_time_lsb);//0xCD = 0.05*2^12-1
    if (0 < (on_time >> 8) && (on_time >> 8) < 256){
    	i2cWriteByteData(handle, LED0_OFF_H + 4*channel, on_time_msb);
    }
    else {
    	i2cWriteByteData(handle, LED0_OFF_H +4*channel, 0);
    }
}

//Change l'angle du servo de maniere precise (jspr par pitié)
void changeServoPositionSynchro(int handle, int channel, int currentPosition, int targetPosition){
	if (targetPosition - currentPosition > 0){
		for (int i = currentPosition+1; i <= targetPosition; i++){
			setServoPosition(handle, channel, i);
        		gpioSleep(PI_TIME_RELATIVE, 0, 10000);
		}//Fo bouger les servo en meme temps aaah donc rajouter les position et channel et info des autres servo
	}
}

//Fonctions pour le bras sur le canal 1
void baisser_bras(int handle){
    int angle = 107;
    for (int i = 1; i <= 107;i++){
        gpioSleep(PI_TIME_RELATIVE, 0, 10000);
        setServoPosition(handle, 4, i);
        setServoPosition(handle, 5, 107-i);
    }
}

void lever_bras(int handle){
    int angle = 107;
    for (int i = 1; i <= 107;i++){
        gpioSleep(PI_TIME_RELATIVE, 0, 10000);
        setServoPosition(handle, 4, 107-i);
        setServoPosition(handle, 5, i);
    }
}


//Fonction pour les pinces
void fermer_pince(int handle, int pince){
    int angle;
    if (pince < 0 || pince > 2){
    	return;
    }
    switch(pince){
	case 0:
	    angle = 142;
	    break;
	case 1:
	    angle = 42;
	    break;
	case 2:
	    angle = 152;
	    break;
    }
    setServoPosition(handle, pince, angle);
//10° de + pour fermer completement
}

void ouvrir_pince(int handle, int pince){
    int angle;
    if (pince < 0 || pince > 2){
    	return;
    }
    switch(pince){
	case 0:
		angle = 115;
		break;
	case 1:
		angle = 22;
		break;
	case 2:
		angle = 125;
		break;
    }
    setServoPosition(handle, pince, angle);
}

void check_panneau(int handle, int quelBras){//bras gauche = 7 bras droit = 6
        setServoPosition(handle, quelBras, 30);
}

void uncheck_panneau(int handle, int quelBras){
        setServoPosition(handle, quelBras, 0);
}
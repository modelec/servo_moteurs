#include <stdio.h>
#include <pigpio.h>
#include <time.h>

//Define registers address
#define PCA9685_ADDR 0x40
#define MODE1_REG    0x00
#define PRE_SCALE    0xFE

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

//Define duty cycle (in a range of 12 bits = 4096) for PWM (min 2%, max 12%)
#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096

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
    int angle = 100;
    for (int i = 1; i <= angle;i++){
        gpioSleep(PI_TIME_RELATIVE, 0, 10000);
        setServoPosition(handle, 4, i);
        setServoPosition(handle, 5, angle-i);
    }
}

void lever_bras(int handle){
    int angle = 107;
    for (int i = 1; i <= angle;i++){
        gpioSleep(PI_TIME_RELATIVE, 0, 10000);
        setServoPosition(handle, 4, angle-i);
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

int main()
{
    int handle;

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "Impossible d'initialiser pigpio\n");
        return 1;
    }

    handle = i2cOpen(1, PCA9685_ADDR, 0);

    if (handle < 0)
    {
        fprintf(stderr, "Impossible d'ouvrir la connexion I2C\n");
        gpioTerminate();
        return 1;
    }

    initPCA9685(handle);
    /*lever_bras(handle);
    gpioSleep(PI_TIME_RELATIVE, 1, 0);
    ouvrir_pince(handle, 0);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    ouvrir_pince(handle, 1);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    ouvrir_pince(handle, 2);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    fermer_pince(handle, 0);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    fermer_pince(handle, 1);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    fermer_pince(handle, 2);
    gpioSleep(PI_TIME_RELATIVE, 1, 0);
    baisser_bras(handle);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    */
    check_panneau(handle, 6);
    gpioSleep(PI_TIME_RELATIVE, 1, 0); // Pause de 2 secondes
    uncheck_panneau(handle, 6);
    i2cClose(handle);

    // Terminaison de pigpio
    gpioTerminate();

    return 0;
}

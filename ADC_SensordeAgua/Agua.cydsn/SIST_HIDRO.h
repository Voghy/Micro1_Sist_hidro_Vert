/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

// Librerias
#include "project.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define CHANNEL_0_SENSOR_A_2    0    
#define CHANNEL_1_SENSOR_A_1    1
#define CHANNEL_2_SENSOR_PH     2

#define SENSOR_1_AGUA   2       //  Voltaje que cuando es mayor hay agua y cuando es menor no hay
#define SENSOR_2_AGUA   2.5     //  Voltaje que cuando es menor hay agua y cuando es mayor no hay

#define PH_ACIDO        6.9     //  Valor del PH que sale con un liquido neutro, faltan soluciones de PH para calibrar el sensor
#define PH_BASICO       7.3
#define offset_PH       22.47    //  El valor del voltaje calibrado con el potenciometro del sensor de pH

//  Banderas para que las interrupciones vayan a una accion
#define COMANDO 1
#define CONFIGURACION_PH 2
#define GUARDAR_EEPROM 3
#define PH_TIMER 4

// Valores por default del pH
typedef struct {
    float minPH; // Valor mínimo de pH
    float maxPH; // Valor máximo de pH
} ConfigPH;

#define MIN_PULSE 199     // Ancho de pulso para 0° (en microsegundos)
#define MAX_PULSE 399    // Ancho de pulso para 180° (en microsegundos)

#define ABIERTO 0
#define CERRADO 1
#define SERVO_PH_ELEVADOR_ABIERTO 0
#define SERVO_PH_REDUCTOR_ABIERTO 1
#define SERVOS_CERRADOS 2

#define PH_BAJO 0
#define PH_ALTO 1
#define PH_RANGO 2

#define Max_Buffer       100
char rxBuffer[Max_Buffer];      //Cadena de caracter
uint8_t rxIndex = 0;            //Variable auxiliar para conteo de nro de caracteres

volatile int interrup = 0;

char cMain = 0;

#define Max_Buffer1      32
char buffer[Max_Buffer1];

#define MAX_BUFFER_ESP   32
char bufferESP[MAX_BUFFER_ESP];

uint8_t lee_ESP();
void enviar_Respuesta(char c);

void init_ADC();
uint8_t sensor_de_agua();
uint8_t sensor_de_agua_1();
void sensor_PH();
void servoPosition(uint8_t nServo);

/* [] END OF FILE */

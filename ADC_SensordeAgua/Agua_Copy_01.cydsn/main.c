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
#include "project.h"
#include "SIST_HIDRO.h"

ConfigPH configPH;

CY_ISR(ver_pH){
    interrup = PH_TIMER;
}

void inicializarConfigPH() {
    configPH.minPH = PH_ACIDO; 
    configPH.maxPH = PH_BASICO; 
}

void actualizarConfigPH(float nuevoMin, float nuevoMax) {
    if (nuevoMin < nuevoMax && nuevoMin >= 0 && nuevoMax <= 14) {
        configPH.minPH = nuevoMin;
        configPH.maxPH = nuevoMax;
    } else {
        UART_PSOC_PutString("Error: Rango de pH invalido.\r\n");
    }
}

void conv_pH (char *buffer){
    char* pEnd;
    static float f1, f2;
    f1 = strtof (buffer, &pEnd);
    f2 = strtof (pEnd, NULL);
    snprintf (rxBuffer, Max_Buffer, "Valores recibidos de pH: %.2f, %.2f.\r\n", f1, f2);
    UART_PSOC_PutString(rxBuffer);
    actualizarConfigPH(f1, f2);
    
}


void servoPosition(uint8_t nServo){
    resetTimerpH_Write(1);
    switch(nServo){
        case SERVO_PH_ELEVADOR_ABIERTO:
            PWM_WriteCompare1(MIN_PULSE);
            PWM_WriteCompare2(MAX_PULSE);
            resetTimerpH_Write(0); // resetar el timer
            timerpH_Enable();
            break;
        case SERVO_PH_REDUCTOR_ABIERTO:
            PWM_WriteCompare1(MAX_PULSE);
            PWM_WriteCompare2(MIN_PULSE);
            resetTimerpH_Write(0); // resetar el timer
            timerpH_Enable();
            break;
        default:
            PWM_WriteCompare1(MAX_PULSE);
            PWM_WriteCompare2(MAX_PULSE);
            break;
            
    }
}

void init_ADC(){
    ADC_SA1_StartConvert();
}


uint8_t sensor_de_agua(){
    uint16_t result = ADC_SA1_GetResult16(CHANNEL_1_SENSOR_A_1);
    float voltaje = ADC_SA1_CountsTo_Volts(result); 

    snprintf(buffer, 100, "Sensor 1 de Agua, Canal %d: %f\r\n", CHANNEL_1_SENSOR_A_1, voltaje);
    UART_PSOC_PutString(buffer);
    
    if(voltaje >= SENSOR_1_AGUA){
        Pin_LED_Write(1);  // Enciende LED si hay agua
        return 1;
    }else{
        Pin_LED_Write(0);  // Apaga LED si no hay agua
        return 0;
    }
}

void sensor_PH(){  //  Lee el adc, el pin del pH
    uint16_t result = ADC_SA1_GetResult16(CHANNEL_2_SENSOR_PH);
    float voltaje = ADC_SA1_CountsTo_Volts(result); 
    float valor_PH = offset_PH - (5.5 * voltaje);     
    
    Pin_LED_R_Write(0);
    Pin_LED_Write(0);
    Pin_LED_V_Write(0);
    servoPosition(SERVOS_CERRADOS);
    
    if(valor_PH < configPH.minPH){
        Pin_LED_R_Write(1);  
        sniprintf(buffer, Max_Buffer1, "El valor del pH es: %.2f, acida\r\n", valor_PH);
        
        UART_ESP_PutString(buffer);
        UART_PSOC_PutString(buffer);
        servoPosition(SERVO_PH_ELEVADOR_ABIERTO);
        //return valor_PH;
    }else if(valor_PH > configPH.maxPH){
        Pin_LED_Write(1);  
        sniprintf(buffer, Max_Buffer1, "El valor del pH es: %.2f, basica\r\n", valor_PH);
        UART_ESP_PutString(buffer);
        UART_PSOC_PutString(buffer);
    
        servoPosition(SERVO_PH_REDUCTOR_ABIERTO);
        //return valor_PH;
    }else{
        Pin_LED_V_Write(1);  
        sniprintf(buffer, Max_Buffer1, "El valor del pH es: %.2f, en rango\r\n", valor_PH);
        UART_ESP_PutString(buffer);
        UART_PSOC_PutString(buffer);
    
        servoPosition(SERVOS_CERRADOS);
        //return valor_PH;
    }
}


void enviar_Respuesta(char c){
    
    UART_PSOC_PutString("Esperando datos del ESP8266\r\n");
    //UART_PSOC_PutString(rxBuffer);
    //CyDelay(1000);
    
    if (c == 'S') { // Verificar si el mensaje recibido por el ESP8266 es de sincronización, S
        UART_ESP_PutString("Datos sincronizados con ESP8266\r\n");
        UART_PSOC_PutString("Datos sincronizados con ESP8266\r\n");
        
    }else if(c == 'T'){  //  Verifica si el mensaje recibido por el ESP8266 es del sensor, T
        int ret = sensor_de_agua(); //  Entra en la funcion donde se verifica los niveles de agua (o los voltajes) y guarda lo retornado
        if(ret == 1){   //  Si lo retornado es igual a 1, hay agua
            UART_ESP_PutString("Hay agua en el tanque\r\n");
            UART_PSOC_PutString("Hay agua en el tanque\r\n");
            Pin_Bomba_Write(1);
        }else{  //  Si no, no hay agua
            UART_ESP_PutString("No hay agua en el tanque\r\n");
            UART_PSOC_PutString("No hay agua en el tanque\r\n");
            Pin_Bomba_Write(0);
        }
    }else if(c == 'P'){
        sensor_PH();
    }else if(c == 'A'){
        Pin_Bomba_Write(0);
        UART_ESP_PutString("Bomba Apagada\r\n");
        UART_PSOC_PutString("Bomba Apagada\r\n");
    }else if(c == 'B'){
        Pin_Bomba_Write(1);
        UART_ESP_PutString("Bomba Encendida\r\n");
        UART_PSOC_PutString("Bomba Encendida\r\n");
    } 
}
void leeLinea (char *buf, int tamBuf, uint8_t (*getcharl)(), void (*putcharl)(uint8_t)) {
    int i;
    uint8 c;
   
    for (i = 0; i < tamBuf -1; ) {
        if ((c = getcharl()) != 0){
            putcharl(c);
            if (c == '\r') break;   // salimos si recibimos un enter
            buf[i++] = c;
        }
    }
    buf[i] = '\0';
    putcharl('\n');
}
CY_ISR(leerESP) {
    char c;
    static uint8_t eRecibido = 0;
    
    if (eRecibido == 0 && UART_ESP_GetChar() == 'e') {
        eRecibido = 1;
        UART_ESP_PutChar('l');
        return;
    } else if (eRecibido == 1) {
        eRecibido = 0;
        c = UART_ESP_GetChar();
        UART_ESP_PutChar(c);
        UART_PSOC_PutChar(c);
        UART_PSOC_PutChar('\n');
        UART_PSOC_PutChar('\r');
        
        
        if (c == 'C') {
            interrup = CONFIGURACION_PH;
            cMain = c;
            leeLinea(buffer,Max_Buffer1,UART_ESP_GetChar,UART_ESP_PutChar);
            
            
        } else if (bufferESP[0] == 'G') {
            interrup = GUARDAR_EEPROM;  // Interrupción para guardar en EEPROM
            cMain = c;  // Guardar el valor 'G'
        } else if (c == 'S' || c == 'T' || c == 'P' || c == 'A' || c == 'B') {
            interrup = COMANDO;  // Interrupción para comando
            cMain = c;  // Guardar el comando
        }
    }

}

/*
CY_ISR(leerESP) {
   int i;
   char c;
   if((c = UART_ESP_GetChar()) != 'e')
    return;
   c = UART_ESP_GetChar();
   UART_ESP_PutChar('l');
    
   for (i = 0; i < MAX_BUFFER_ESP - 1; ) {
        c = UART_ESP_GetChar();
        if (c != 0) {
            if (c == '\r' || c == '\n') {
                break;
            }
            bufferESP[i++] = c;
        }
    }
    bufferESP[i] = '\0';
    
    UART_ESP_PutString(bufferESP);
    UART_PSOC_PutChar(bufferESP[0]);
    
    
    
    UART_ESP_PutChar(c);
    UART_PSOC_PutChar(c);
    UART_PSOC_PutChar('\n');
    UART_PSOC_PutChar('\r');
    if (c == 'C') {
        interrup = 2;   // Interrupcion de configuracion del pH
    } else if(c == 'G' ) {
        interrup = 3;
    } else if(c =='S' ||c == 'T'||c  == 'P'||c == 'A'|| c == 'B'){
        interrup = 1;
    }
}
*/


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_PSOC_Start();
    UART_ESP_Start();
    ADC_SA1_Start();
    init_ADC();
    PWM_Start();  
    timerpH_Start();
    
    resetTimerpH_Write(0);
    int_pH_StartEx(ver_pH);     // Interrupcion con timer para verificar el pH y cerrar o abrir valvulas de pH
    
    isr_rx2_StartEx(leerESP);
    
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        if (interrup == COMANDO) {
            UART_PSOC_PutString("Interrupcion 1 activada. Mensaje recibido y procesado correctamente.\r\n");
            enviar_Respuesta(cMain);
            interrup = 0;  // Reinicia el contador de interrupciones
        } else if(interrup == CONFIGURACION_PH){
            UART_PSOC_PutString("Interrupcion 2 activada. Mensaje recibido y procesado correctamente.\r\n");
            conv_pH(buffer);
            interrup = 0;
        }else if(interrup == GUARDAR_EEPROM){
            UART_PSOC_PutString("Interrupcion 3 activada. Mensaje recibido y procesado correctamente.\r\n");
            //guardar_eeprom();
            interrup = 0;
        }else if(interrup == PH_TIMER)
        {
            sensor_PH();
            interrup = 0;
        }
//        if(registro_TimerDelay == 1)
//        {
//            //hacer lo que sea que deba hacerse cada n segundos.    
//        }
    }
}

/* [] END OF FILE */

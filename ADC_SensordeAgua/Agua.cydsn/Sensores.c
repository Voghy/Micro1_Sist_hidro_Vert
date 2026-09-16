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
//#include "SIST_HIDRO.h"


//void init_ADC(){
//    ADC_SA1_StartConvert();
//}
//
//uint8_t sensor_de_agua(){
//    uint16_t result = ADC_SA1_GetResult16(CHANNEL_1_SENSOR_A_1);
//    float voltaje = ADC_SA1_CountsTo_Volts(result); 
//
//    snprintf(buffer, 100, "Sensor 1 de Agua, Canal %d: %f\r\n", CHANNEL_1_SENSOR_A_1, voltaje);
//    UART_PSOC_PutString(buffer);
//    
//    if(voltaje >= SENSOR_1_AGUA){
//        Pin_LED_Write(1);  // Enciende LED si hay agua
//        return 1;
//    }else{
//        Pin_LED_Write(0);  // Apaga LED si no hay agua
//        return 0;
//    }
//}
//
//uint8_t sensor_de_agua_1(){
//    uint16_t result = ADC_SA1_GetResult16(CHANNEL_0_SENSOR_A_2);
//    float voltaje = ADC_SA1_CountsTo_Volts(result); 
//
//    snprintf(buffer, 100, "Sensor 2 de Agua, Canal %d: %f\r\n", CHANNEL_0_SENSOR_A_2, voltaje);
//    UART_PSOC_PutString(buffer);
//    
//    if(voltaje < SENSOR_2_AGUA){
//        Pin_LED_Write(0);  // Enciende LED si hay agua
//        return 0;
//    }else{
//        Pin_LED_Write(1);  // Apaga LED si no hay agua
//        return 1;
//    }
//}
//
//float sensor_PH(){
//    uint16_t result = ADC_SA1_GetResult16(CHANNEL_2_SENSOR_PH);
//    float voltaje = ADC_SA1_CountsTo_Volts(result); 
//    
//    float valor_PH = 7 - (voltaje - offset_PH);
//    
//    snprintf(buffer, 100, "Sensor PH, Canal %d: %f,     %f\r\n", CHANNEL_2_SENSOR_PH, valor_PH, voltaje);
//    UART_PSOC_PutString(buffer);
//    
//    if(valor_PH < PH_ACIDO){
//        Pin_LED_R_Write(1);  
//        Pin_LED_Write(0);  
//        Pin_LED_V_Write(0);  
//        
//        return valor_PH;
//    }else if(valor_PH > PH_BASICO){
//        Pin_LED_R_Write(0);  
//        Pin_LED_Write(1);  
//        Pin_LED_V_Write(0);  
//        
//        return valor_PH;
//    }else{
//        Pin_LED_R_Write(0);  
//        Pin_LED_Write(0);  
//        Pin_LED_V_Write(1);  
//        
//        return valor_PH;
//    }
//}

/* [] END OF FILE */

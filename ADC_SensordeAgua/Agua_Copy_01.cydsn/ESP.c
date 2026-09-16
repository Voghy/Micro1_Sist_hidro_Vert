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
//
//
//uint8_t lee_ESP(){  //  Como la funcion lee linea, un poco modificada
//    uint8_t resultado = 0;  //  Retorna el valor para entrar en el if del loop del main
//    if (UART_ESP_GetRxBufferSize() > 0) {  // Verificar si se recibió algún dato 
//        char receivedChar = UART_ESP_GetChar();
//        if (receivedChar != '\n' && receivedChar != '\r') {
//                rxBuffer[rxIndex++] = receivedChar; // Almacenar en el buffer
//                
//        } else {
//            rxBuffer[rxIndex] = '\0'; // Agregar fin de cadena
//            rxIndex = 0; // Reiniciar índice
//            
//            resultado = 1;
//            
//        }
//    }
//    
//    return resultado;
//}
//
//void enviar_Respuesta(){
//    
//    UART_PSOC_PutString("Esperando datos del ESP8266\r\n");
//    UART_PSOC_PutString(rxBuffer);
//    // Verificar si el mensaje recibido por el ESP8266 es de sincronización
//    if (strcmp(rxBuffer, "S") == 0) {
//        UART_ESP_PutString("Datos sincronizados con ESP8266\r\n");
//        UART_PSOC_PutString("Datos sincronizados con ESP8266\r\n");
//        
//    }else if(strcmp(rxBuffer, "T") == 0){  //  Verifica si el mensaje recibido por el ESP8266 es del sensor
//        int ret = sensor_de_agua(); //  Entra en la funcion donde se verifica los niveles de agua (o los voltajes) y guarda lo retornado
//        if(ret == 1){   //  Si lo retornado es igual a 1, hay agua
//            UART_ESP_PutString("Hay agua en el tanque\r\n");
//            UART_PSOC_PutString("Hay agua en el tanque\r\n");
//        }else{  //  Si no, no hay agua
//            UART_ESP_PutString("No hay agua en el tanque\r\n");
//            UART_PSOC_PutString("No hay agua en el tanque\r\n");
//        }
//    }else if(strcmp(rxBuffer, "P") == 0){
//        float retornado2 = sensor_PH();
//        sniprintf(buffer, Max_Buffer1, "El valor del Ph es: %f\r\n", retornado2);
//        UART_ESP_PutString(buffer);
//        UART_PSOC_PutString(buffer);
//    }else if(strcmp(rxBuffer, "A") ==0){
//        Pin_Bomba_Write(0);
//        UART_ESP_PutString("Bomba Apagada\r\n");
//        UART_PSOC_PutString("Bomba Apagada\r\n");
//    }else if(strcmp(rxBuffer, "B") ==0){
//        Pin_Bomba_Write(1);
//        UART_ESP_PutString("Bomba Encendida\r\n");
//        UART_PSOC_PutString("Bomba Encendida\r\n");
//    } 
//}

/* [] END OF FILE */

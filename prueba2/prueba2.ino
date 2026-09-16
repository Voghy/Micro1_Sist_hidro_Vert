#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
SoftwareSerial espSerial(D7, D8);  // RX en D7, TX en D8

// Parámetros de conexión WiFi
#define CANT_WIFI 6
String wifi[CANT_WIFI] = {"+++", "++++", "+++", "+++", "+++", "+++"};
String wifiPassword[CANT_WIFI] = {"---", "---", "---!", "---", "---", "---"};
String wifiName = "";

// Configuración del bot de Telegram
#define BOT_TOKEN "$$$:***"
String chat_id_Voghy = "###";
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Variables globales
unsigned long Ultima_hora = 0;
const unsigned long intervalo = 300000;
unsigned long lastTimeBotRan;
const long botRequestDelay = 5000;
bool WiFiConnected = false;


struct ConfigPH {
  float minPH;  
  float maxPH;  
};

ConfigPH configPH = {5.5, 7.5};  // Valores iniciales por defecto

// ---------------------- Funciones del RTC ----------------------
void iniciarRTC() {
  Wire.begin(D2, D1);        
  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC perdió potencia, ajustando la hora...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void imprimirRTC() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

//  -------   ENVIAR ALERTAS PARA SABER EL ESTADO DEL TANQUE Y EL NIVEL DE pH   -------------
void enviarAlertaHora() {
  unsigned long act_Time = millis();
  String datosPSoC = "";
  if (act_Time - Ultima_hora >= intervalo) {
    imprimirRTC();
    bot.sendMessage(chat_id_Voghy, "Alerta: Ha pasado una hora", "");
    Ultima_hora = act_Time;
    if(enviarPSOC('P')){
      datosPSoC += espSerial.readStringUntil('\0');
      Serial.println("PSoC: " + datosPSoC);
      //bot.sendMessage(chat_id_Voghy, "PSoC: " + datosPSoC, "");
    }
    if(enviarPSOC('T')){
      datosPSoC += espSerial.readStringUntil('\0');
      Serial.println("PSoC: " + datosPSoC);
      bot.sendMessage(chat_id_Voghy, "PSoC: " + datosPSoC, "");
    }
  }
}

// ---------------------- Conexión WiFi ----------------------
void conectarWiFi() {
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i < CANT_WIFI; i++) {
      Serial.print("Intentando conectar a: ");
      Serial.println(wifi[i]);
      WiFi.begin(wifi[i].c_str(), wifiPassword[i].c_str());
      int attemptCount = 0;
      while (WiFi.status() != WL_CONNECTED && attemptCount < 20) {
        delay(500);
        attemptCount++;
        Serial.print(".");
      }
      if (WiFi.status() == WL_CONNECTED) {
        wifiName = wifi[i];
        WiFiConnected = true;
        break;
      }
    }
    if (WiFiConnected) {
      Serial.println("\nConectado a: " + wifiName);
    } else {
      Serial.println("\nNo se pudo conectar a ninguna red WiFi.");
      delay(5000);
    }
  }
}

// ---------------------- Comunicación con PSoC ----------------------

int enviarPSOC(char msg) {
  char aux = 0;
  unsigned long startTime;

  // Limpia el buffer serial antes de iniciar la comunicación
  while (espSerial.available()) espSerial.read();

  // Enviar prefijo 'e'
  espSerial.write('e');

  // Esperar respuesta 'l' con timeout
  startTime = millis();
  while (!(espSerial.available() > 0)) {
    if (millis() - startTime > 10000) { // Timeout de 10 segundo
      Serial.println("Error: Timeout esperando respuesta 'l'");
      return 0;
    }
  }
  if (espSerial.read() != 'l') {
    Serial.println("Error: Respuesta incorrecta (no 'l')");
    return 0;
  }

  // Enviar mensaje de comando
  espSerial.write(msg);

  // Esperar eco del mensaje con timeout
  startTime = millis();
  while (!(espSerial.available() > 0)) {
    if (millis() - startTime > 10000) { // Timeout de 10 segundo
      Serial.println("Error: Timeout esperando eco del comando");
      return 0;
    }
  }
  aux = espSerial.read();

  // Verificar si el eco recibido coincide con el mensaje enviado
  if (aux == msg) {
    Serial.println("Comando enviado correctamente");
    return 1;
  }

  Serial.println("Error: Comando no confirmado por el PSoC");
  return 0;
}

int enviarPSOC_1(String msg) {
  String aux = "";
  unsigned long startTime;

  // Limpia el buffer serial antes de iniciar la comunicación
  while (espSerial.available()) espSerial.read();

  // Enviar prefijo 'e'
  espSerial.write('e');

  // Esperar respuesta 'l' con timeout
  startTime = millis();
  while (!(espSerial.available() > 0)) {
    if (millis() - startTime > 10000) { // Timeout de 10 segundos
      Serial.println("Error: Timeout esperando respuesta 'l'");
      return 0;
    }
  }
  if (espSerial.read() != 'l') {
    Serial.println("Error: Respuesta incorrecta (no 'l')");
    return 0;
  }

  // Enviar el mensaje de comando
  espSerial.print(msg + '\n');  // Enviar cadena con terminador de línea

  // Esperar eco del mensaje con timeout
  startTime = millis();
  while (!(espSerial.available() > 0)) {
    if (millis() - startTime > 10000) { // Timeout de 10 segundos
      Serial.println("Error: Timeout esperando eco del comando");
      return 0;
    }
  }

  // Leer el eco completo desde el ESP
  aux = espSerial.readStringUntil('\n');  // Leer hasta encontrar un '\n'

  // Verificar si el eco recibido coincide con el mensaje enviado
  if (aux == msg) {
    Serial.println("Comando enviado correctamente");
    return 1;
  }

  Serial.println("Error: Comando no confirmado por el PSoC");
  return 0;
}


// ---------------------- Telegram Bot ----------------------
void enviarMenu(String chat_id) {
  String menuText = "Menú de comandos:\n";
  menuText += "/connection - Sincronizar con PSoC\n";
  menuText += "/stTanque - Estado del tanque\n";
  menuText += "/stPH - Valor del pH\n";
  menuText += "/apBom - Apagar bomba\n";
  menuText += "/enBom - Encender bomba\n";
  menuText += "/config - Configurar rangos de pH\n";
  menuText += "/guardar - Guardar configuración\n";
  String keyboardJson = "[[\"/connection\", \"/stTanque\"], [\"/stPH\", \"/apBom\"], [\"/enBom\", \"/config\"], [\"/guardar\"]]";
  bot.sendMessageWithReplyKeyboard(chat_id, menuText, "", keyboardJson);
}

//  -------------------- Configuracion de pH -----------------
void configurarPH(float nuevoMin, float nuevoMax) {
  String mensaje = "";
  if ((nuevoMin < nuevoMax) && (0 <= nuevoMin) && (nuevoMax <= 14)) {
    configPH.minPH = nuevoMin;
    configPH.maxPH = nuevoMax;
    Serial.println("Configuración actualizada:");
    Serial.print("Mínimo: ");
    Serial.println(configPH.minPH);
    Serial.print("Máximo: ");
    Serial.println(configPH.maxPH);
    mensaje = "C " + String(nuevoMin, 2) + "-" + String(nuevoMax, 2);
    
  } else {
    Serial.println("Error: El valor mínimo debe ser menor que el máximo.");
  }
}

//  ------------------  RESPONDER POR TELEGRAM  --------------------
void manejarNuevosMensajes(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String from_name = bot.messages[i].from_name;
    String text = bot.messages[i].text;
    String datosPSoC = "";

    Serial.println("Mensaje de: " + from_name + " - " + text);

    int comando = 0;
    if (text == "/start") {
      bot.sendMessage(chat_id, "Hola " + from_name + ", soy tu bot ESP8266!", "");
    } else if (text == "/connection") {
      comando = 1;
    } else if (text == "/stTanque") {
      comando = 2;
    } else if (text == "/stPH") {
      comando = 3;
    } else if (text == "/apBom") {
      comando = 4;
    } else if (text == "/enBom") {
      comando = 5;
    } else if (text.startsWith("/config")) {
      comando = 6;
    } else if (text == "/guardar") {
      comando = 7;
    } else if (text == "/help") {
      comando = 8;
    } 

    switch (comando) {
      case 1:
        bot.sendMessage(chat_id, "Sincronización con PSoC", "");
        if(enviarPSOC('S')){
         
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "Datos del PSoC: " + datosPSoC, "");
          
        }else{
          bot.sendMessage(chat_id, "ERROR...", "");
        }
        break;
      case 2:
        bot.sendMessage(chat_id, "Verificando el estado del tanque", "");
        if(enviarPSOC('T')){
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "PSoC: " + datosPSoC, "");
        }else bot.sendMessage(chat_id, "ERROR...", "");
        break;

      case 3:
        bot.sendMessage(chat_id, "Verificando valor del pH", "");
        if(enviarPSOC('P')){
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "PSoC: " + datosPSoC, "");
        }else bot.sendMessage(chat_id, "ERROR...", "");
        break;

      case 4:
        bot.sendMessage(chat_id, "Apagando Bomba", "");
        if(enviarPSOC('A')){
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "PSoC: " + datosPSoC, "");
        }else bot.sendMessage(chat_id, "ERROR...", "");
        break;

      case 5:
        bot.sendMessage(chat_id, "Prendiendo Bomba", "");
        if(enviarPSOC('B')){
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "PSoC: " + datosPSoC, "");
        }else bot.sendMessage(chat_id, "ERROR...", "");
        break;

      case 6: {
        String configString1 = ("C " + text.substring(7));
        String configString = (text.substring(7));  // Extraer configuración
        if(enviarPSOC_1(configString1)){
          Serial.println("Enviado correctamente al PSOC. \n");
        }else{
          Serial.println("Error: No se logro enviar al PSOC.\n");
        }
        int separatorIndex = configString.indexOf(',');
        if (separatorIndex > 0) {
          float nuevoMin = configString.substring(0, separatorIndex).toFloat();
          float nuevoMax = configString.substring(separatorIndex + 1).toFloat();
          configurarPH(nuevoMin, nuevoMax);
          bot.sendMessage(chat_id, "Nueva configuración aplicada:\nMin: " + String(configPH.minPH) + "\nMax: " + String(configPH.maxPH), "");
        } else {
          bot.sendMessage(chat_id, "Formato incorrecto. Usa: /config min,max", "");
        }
        break;
      }

      case 7:
        if(enviarPSOC('G')){
          datosPSoC += espSerial.readStringUntil('\0');
          Serial.println("Datos del PSoC: " + datosPSoC);
          bot.sendMessage(chat_id, "PSoC: " + datosPSoC, "");
        }else bot.sendMessage(chat_id, "ERROR...", "");
        break;

      case 8:
        Serial.println("Enviando menu...");
        enviarMenu(chat_id);
        break;

      default:
        bot.sendMessage(chat_id, "Comando no reconocido. Usa /Ayuda para obtener la lista de comandos.", "");        
        break;
    }
  }
}

// ---------------------- Setup y Loop ----------------------
void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  
  iniciarRTC();
  conectarWiFi();
  
  client.setInsecure(); 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - lastTimeBotRan > botRequestDelay) {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      while (numNewMessages) {
        manejarNuevosMensajes(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }else {
      enviarAlertaHora();
    }
  } else {
    conectarWiFi();
  }
}

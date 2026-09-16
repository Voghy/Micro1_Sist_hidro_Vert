# 🌱 Sistema Hidropónico Automatizado con PSoC ARM Cortex-M3

Trabajo Práctico de **Microprocesadores I** — Universidad Católica "Nuestra Señora de la Asunción"
Facultad de Ciencias y Tecnología — Ingeniería Electrónica

---

## 📖 ¿Qué es?

Diseño e implementación de un **sistema hidropónico semi-automatizado** basado en el microcontrolador **PSoC ARM Cortex-M3**, que integra sensores y actuadores para controlar y monitorear variables críticas del cultivo sin suelo: **pH del agua**, **nivel del tanque** y **circulación programable de agua**.

El sistema permite la **gestión remota vía WiFi (ESP8266)** y el almacenamiento de hasta **4 programas de riego** configurables por el usuario.

---

## 🎯 Objetivos

- Automatizar el riego mediante una bomba de agua programable.
- Monitorear y regular el pH del agua dentro de rangos configurables.
- Detectar el nivel de agua en el tanque para evitar funcionamiento en seco.
- Permitir la interacción remota del usuario mediante WiFi (ESP8266 + Telegram).
- Almacenar hasta 4 programas de riego seleccionables.

---

## ⚙️ ¿Qué se hace?

1. **Diseño del sistema hidropónico vertical:**
   - Estructura con tanque en la base y tubos perforados para los cultivos.
   - Caja de control con PSoC, ESP8266, RTC y módulos de sensores.

2. **Integración de hardware:**
   - **Bomba de agua** controlada por relé de 5V desde el PSoC.
   - **Sensor de nivel** (resistivo / YL-83) para detectar presencia de agua.
   - **Sensor de pH SEN0161** con calibración empírica.
   - **Servos** para dosificación de soluciones reguladoras de pH (válvulas impresas en 3D).
   - **RTC DS3231** para mantener la hora ante cortes de energía.

3. **Comunicación:**
   - Protocolo **UART semidúplex** entre PSoC y ESP8266.
   - **TCP/IP** para corrección de errores.
   - Conexión a **Telegram** para monitoreo y control remoto.

4. **Programación del PSoC:**
   - Monitoreo, control y comunicación integrados.
   - Medición automática de pH cada hora.

5. **Pruebas:**
   - Individuales (sensores, actuadores, comunicación).
   - De integración (sincronización del sistema completo).
   - Validación del protocolo PSoC ↔ ESP.

---

## 🧩 Componentes principales

| Componente | Descripción |
|---|---|
| **PSoC ARM Cortex-M3** | Microcontrolador principal |
| **ESP8266** | Módulo WiFi para conexión remota |
| **RTC DS3231** | Reloj de tiempo real con respaldo |
| **Bomba de agua + Relé 5V** | Circulación programable de agua |
| **Sensor de pH SEN0161** | Medición de acidez/alcalinidad |
| **Sensor de nivel (YL-83 / resistivo)** | Detección de agua en tanque |
| **Servos + válvulas 3D** | Dosificación de soluciones reguladoras |
| **Display / Telegram** | Interfaz de usuario |

---

## 📐 Ecuación de calibración del pH

Calibrado empíricamente con leche (pH 6.6, 2.8V) y agua con limón (pH 2.8, 3.57V):

```
pH = -5.51 · V + 22.47
```

---

## 📊 Resultados

- ✅ Monitoreo y regulación automática de agua y pH.
- ✅ Comunicación remota efectiva vía ESP8266.
- ✅ Cumplimiento de todos los objetivos planteados.
- ⚠️ Margen de mejora en velocidad de notificación y autonomía del tanque.

---

## ⚠️ Limitaciones

- Dependencia de conectividad para alertas en tiempo real.
- Alcance limitado a un solo tipo de cultivo.
- Recomendación: evitar proyectos que combinen agua y electrónica simultáneamente por riesgo de daños.

---

## 🚀 Mejoras futuras

- Integrar almacenamiento de datos en la nube.
- Automatizar el llenado del tanque de agua.
- Añadir sensores de temperatura y humedad.
- Implementar el circuito en una **PCB** dedicada.

---

## 📁 Contenido del repositorio

- Documentación completa del sistema
- Código del PSoC y del ESP8266
- Esquemáticos de conexión
- Fotos del prototipo y montaje
- Diseños 3D de las válvulas
- Datasheets de los componentes

---

## 👤 Autor

- **Tania Romero** — Y06343

**Profesor:** Vicente González
**Curso:** 4to — 2024
**Asunción, Paraguay**

---

## 📚 Bibliografía destacada

- Cropaia — Sistemas hidropónicos
- Datasheets: RTC DS3231, SEN0161, bomba de agua
- ROHDE & SCHWARZ — Entendiendo el UART
- Wikipedia — pH, TCP/IP

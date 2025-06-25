# 💧 Proyecto de Mezclador Inteligente con ESP32-S3

Este proyecto permite controlar la mezcla de dos líquidos usando un ESP32-S3. La proporción se ajusta mediante botones físicos o una interfaz web, y el proceso es visualizado en una pantalla OLED. El sistema puede activar bombas de agua en función del porcentaje establecido.

---

## ⚙️ Características

- Control de porcentaje de mezcla de Líquido A y B (0–100%)
- Interfaz web para ajustar y disparar la mezcla
- Botones físicos para aumentar o disminuir el porcentaje
- Pantalla OLED para mostrar el estado y proporciones
- Activación de bombas de 3–5 V mediante transistores
- Código adaptado fácilmente para LED o bombas reales

---

## 🧩 Hardware necesario

- ESP32-S3 DevKit
- 2 Microbombas 3V–5V (ej. RUNCCI-YUN)
- Pantalla OLED 128x64 (I2C)
- 2 Transistores MOSFET o NPN (ej. 2N2222 o IRLZ44N)
- 2 Diodos de rueda libre (ej. 1N4007)
- 2 Resistencias para base/gate (220–1kΩ)
- Fuente de alimentación externa (p. ej., 3 pilas AA o batería Li-ion 5 V)
- Cables y protoboard

---

## 🔌 Conexiones eléctricas

### ESP32 ↔ Pantalla OLED (I2C)
- SDA → GPIO 9  
- SCL → GPIO 10  
- VCC → 3.3V  
- GND → GND

### Botones
- BTN_INCR → GPIO 12  
- BTN_DECR → GPIO 13  
- BTN_MEZCLA → GPIO 18  
*(Usan `INPUT_PULLUP`)*

### Bombas (vía transistor)
Cada bomba se conecta así:
- **Fuente positiva de la bomba** → +V externo (3V–5V)
- **Cable negativo de la bomba** → Drenador (Drain) del MOSFET
- **Fuente (Source) del MOSFET** → GND común
- **Gate/base** del transistor → GPIO (LED_A: GPIO 4, LED_B: GPIO 5) a través de una resistencia (220Ω)
- **Diodo** en paralelo con bomba (ánodo al GND, cátodo al positivo)

---

## 🌐 Interfaz Web

Al conectarse al ESP32 vía WiFi, puedes acceder a la interfaz web:

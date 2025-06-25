#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pines físicos
#define BTN_INCR 12
#define BTN_DECR 13
#define LED_A 4
#define LED_B 5
#define BTN_MEZCLA 18

// Pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Nelfi";
const char* password = "12345678";

WebServer server(80);

int percentA = 50;
const int step = 10;
const int totalTime = 10000;

unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 200;

bool lastStateIncr = HIGH;
bool lastStateDecr = HIGH;

void mezclar();
void handleRoot();
void handleMezclar();

void mostrarOLEDporcentajes() {
  int percentB = 100 - percentA;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Texto porcentajes
  display.setCursor(0, 0);
  display.println("Porcentajes de mezcla:");
  display.setCursor(0, 12);
  display.printf("A: %d%%", percentA);
  display.setCursor(64, 12);
  display.printf("B: %d%%", percentB);

  // Barras gráficas
  int barWidth = 120;
  int barHeight = 10;
  int barStartY = 30;
  int widthA = map(percentA, 0, 100, 0, barWidth);
  int widthB = barWidth - widthA;

  // Marco
  display.drawRect(4, barStartY, barWidth, barHeight, SSD1306_WHITE);

  // Rellenar barra A
  if (widthA > 0) {
    display.fillRect(4, barStartY, widthA, barHeight, SSD1306_WHITE);
  }

  display.display();
}


void setup() {
  Serial.begin(115200);

  pinMode(BTN_INCR, INPUT_PULLUP);
  pinMode(BTN_DECR, INPUT_PULLUP);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BTN_MEZCLA, INPUT_PULLUP);
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);

  // Inicializar pantalla OLED
  Wire.begin(9, 10);  // SDA = 9, SCL = 10
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Error al iniciar OLED");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando sistema");
  display.setCursor(0, 16);
  display.println("Conectando WiFi...");
  display.display();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  String ip = WiFi.localIP().toString();
  Serial.println("\nWiFi conectada. IP: " + ip);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi conectado");
  display.setCursor(0, 16);
  display.print("IP: ");
  display.println(ip);
  display.display();
  delay(1500);
  mostrarOLEDporcentajes();

  server.on("/", handleRoot);
  server.on("/mezclar", handleMezclar);
  server.begin();
  Serial.println("Servidor HTTP iniciado");

  server.on("/set", []() {
    if (server.hasArg("porcentaje")) {
      percentA = constrain(server.arg("porcentaje").toInt(), 0, 100);
      mostrarOLEDporcentajes();
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Falta el parametro");
    }
  });
  
}

void loop() {
  server.handleClient();

  bool currStateIncr = digitalRead(BTN_INCR);
  bool currStateDecr = digitalRead(BTN_DECR);
  unsigned long now = millis();

  if (currStateIncr == LOW && lastStateIncr == HIGH && (now - lastDebounce) > debounceDelay) {
    percentA = min(100, percentA + step);
    Serial.printf("Porcentaje A: %d%%\n", percentA);
    mostrarOLEDporcentajes();
    lastDebounce = now;
  }

  if (currStateDecr == LOW && lastStateDecr == HIGH && (now - lastDebounce) > debounceDelay) {
    percentA = max(0, percentA - step);
    Serial.printf("Porcentaje A: %d%%\n", percentA);
    mostrarOLEDporcentajes();
    lastDebounce = now;
  }

  lastStateIncr = currStateIncr;
  lastStateDecr = currStateDecr;

  // Pulsador de mezcla (GPIO 18)
  static bool lastMezclaState = HIGH;
  bool currMezclaState = digitalRead(BTN_MEZCLA);
  if (currMezclaState == LOW && lastMezclaState == HIGH && (now - lastDebounce) > debounceDelay) {
    mezclar();
    lastDebounce = now;
  }
  lastMezclaState = currMezclaState;
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <title>Control de Bombas</title>
      <link href="https://fonts.googleapis.com/css2?family=Quicksand:wght@500&display=swap" rel="stylesheet">
      <style>
        body {
          margin: 0;
          font-family: 'Quicksand', sans-serif;
          background: linear-gradient(135deg, #e0f7fa, #ffffff);
          display: flex;
          justify-content: center;
          align-items: center;
          height: 100vh;
        }
        .container {
          background-color: #ffffffcc;
          border-radius: 12px;
          padding: 30px;
          box-shadow: 0 8px 16px rgba(0,0,0,0.2);
          text-align: center;
          max-width: 400px;
        }
        h2 {
          color: #333;
          margin-bottom: 20px;
        }
        input[type=range] {
          width: 100%;
        }
        .output {
          font-size: 18px;
          margin-top: 10px;
          color: #0077cc;
        }
        .button {
          margin-top: 20px;
          padding: 12px 25px;
          font-size: 16px;
          background-color: #0077cc;
          color: white;
          border: none;
          border-radius: 6px;
          cursor: pointer;
          transition: background-color 0.3s ease;
        }
        .button:hover {
          background-color: #005fa3;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h2>Ajusta el porcentaje de la Bebida A</h2>
        <div class="slider">
          <input type="range" name="porcentaje" min="0" max="100" value="50"
                 oninput="updateOutput(this.value)" onchange="sendPorcentaje(this.value)">
          <div class="output"><span id="value">50</span>%</div>
        </div>
        <form action="/mezclar" method="GET">
          <input id="hiddenValue" type="hidden" name="porcentaje" value="50">
          <input class="button" type="submit" value="Mezclar">
        </form>
      </div>

      <script>
        function updateOutput(val) {
          document.getElementById('value').innerText = val;
          document.getElementById('hiddenValue').value = val;
        }

        function sendPorcentaje(val) {
          fetch('/set?porcentaje=' + val)
            .then(response => response.text())
            .then(data => console.log("OLED actualizado:", data))
            .catch(err => console.error("Error:", err));
        }
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}


void handleMezclar() {
  if (server.hasArg("porcentaje")) {
    percentA = constrain(server.arg("porcentaje").toInt(), 0, 100);
    mostrarOLEDporcentajes();
    mezclar();
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Falta el parámetro 'porcentaje'.");
  }
}

void mezclar() {
  int percentB = 100 - percentA;
  int timeA = totalTime * percentA / 100;
  int timeB = totalTime * percentB / 100;

  String mensaje = "Mezclando: A=" + String(percentA) + "% B=" + String(percentB) + "%";
  Serial.println(mensaje);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Mezclando...");
  display.setCursor(0, 16);
  display.printf("A: %d%%", percentA);
  display.setCursor(0, 32);
  display.printf("B: %d%%", percentB);
  display.display();

  digitalWrite(LED_A, HIGH);
  digitalWrite(LED_B, HIGH);
  delay(min(timeA, timeB));

  if (timeA > timeB) {
    digitalWrite(LED_B, LOW);
    delay(timeA - timeB);
  } else if (timeB > timeA) {
    digitalWrite(LED_A, LOW);
    delay(timeB - timeA);
  }

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);

  Serial.println("Mezcla completada.");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Mezcla completada");
  display.display();
  delay(1500);
  mostrarOLEDporcentajes();
}

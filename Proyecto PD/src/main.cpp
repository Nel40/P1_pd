#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Pines físicos
#define BTN_INCR 12
#define BTN_DECR 13
#define LED_A 4
#define LED_B 5

const char* ssid = "Nelfi";
const char* password = "12345678";

WebServer server(80);

int percentA = 50;
const int step = 10;             // Paso de ajuste por pulsador
const int totalTime = 10000;     // Tiempo total de mezcla (ms)

unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 200;

bool lastStateIncr = HIGH;
bool lastStateDecr = HIGH;

// Declaraciones anticipadas
void mezclar();
void handleRoot();
void handleMezclar();

void setup() {
  Serial.begin(115200);

  pinMode(BTN_INCR, INPUT_PULLUP);
  pinMode(BTN_DECR, INPUT_PULLUP);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);

  Serial.println("Sistema listo. Usa los botones para ajustar y escribe 'm' para mezclar.");

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectada. IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/mezclar", handleMezclar);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Servidor web
  server.handleClient();

  // Lectura no bloqueante de botones
  bool currStateIncr = digitalRead(BTN_INCR);
  bool currStateDecr = digitalRead(BTN_DECR);
  unsigned long now = millis();

  if (currStateIncr == LOW && lastStateIncr == HIGH && (now - lastDebounce) > debounceDelay) {
    percentA = min(100, percentA + step);
    Serial.printf("Porcentaje A: %d%%\n", percentA);
    lastDebounce = now;
  }

  if (currStateDecr == LOW && lastStateDecr == HIGH && (now - lastDebounce) > debounceDelay) {
    percentA = max(0, percentA - step);
    Serial.printf("Porcentaje A: %d%%\n", percentA);
    lastDebounce = now;
  }

  lastStateIncr = currStateIncr;
  lastStateDecr = currStateDecr;

  // Activación por monitor serial
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'm') {
      mezclar();
    }
  }
}

void handleRoot() {
  String html = R"rawliteral(
    <html>
    <head><title>Control de Bombas</title></head>
    <body>
      <h2>Ajusta el porcentaje de la Bebida A</h2>
      <form action="/mezclar" method="GET">
        <input type="range" name="porcentaje" min="0" max="100" value="50"
               oninput="this.nextElementSibling.value = this.value">
        <output>50</output>%
        <br><br>
        <input type="submit" value="Mezclar">
      </form>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleMezclar() {
  if (server.hasArg("porcentaje")) {
    percentA = constrain(server.arg("porcentaje").toInt(), 0, 100);
    mezclar();
    server.sendHeader("Location", "/");
    server.send(303); // Redirigir a página principal
  } else {
    server.send(400, "text/plain", "Falta el parámetro 'porcentaje'.");
  }
}

void mezclar() {
  int percentB = 100 - percentA;
  int timeA = totalTime * percentA / 100;
  int timeB = totalTime * percentB / 100;

  Serial.printf("Mezclando con LEDs: A=%d%% (%d ms), B=%d%% (%d ms)\n", percentA, timeA, percentB, timeB);

  // Enciende ambos LEDs
  digitalWrite(LED_A, HIGH);
  digitalWrite(LED_B, HIGH);
  delay(min(timeA, timeB));

  // Apaga uno según corresponda
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
}

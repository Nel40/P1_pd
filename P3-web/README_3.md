
Informe de la Práctica 3: WiFi y Bluetooth

El objetivo de esta práctica es comprender el funcionamiento de WiFi y Bluetooth en la
ESP32-S3. Se desarrollará un servidor web alojado en la ESP32 que permita a los usuarios
jugar al "3 en Raya" a través de un navegador web. Además, se implementará la comunicación
serie mediante Bluetooth con una aplicación en el móvil.

WiFi
WiFi permite la conexión inalmábrica de dispositivos mediante los protocolos TCP/IP y UDP. La
ESP32 puede actuar como un servidor web al que se conectan otros dispositivos a través de la
red.
Desarrollo de la Práctica
Práctica A: Generación de una Página Web con ESP32
Se implementó un servidor web en la ESP32 que permite jugar al "3 en Raya". La página web
fue diseñada con HTML, CSS y JavaScript, y se almacena como una cadena de texto en el
código de la ESP32.
Código Implementado
```cpp
#include <WiFi.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "Nautilus";
const char* password = "20000Leguas";

WebServer server(80);

// HTML, CSS y JavaScript para el juego 3 en raya
String HTML = "<!DOCTYPE html>\
<html>\
<head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>3 en Raya con ESP32</title>\
    <style>\
        body { font-family: Arial, sans-serif; text-align: center; }\
        .board { display: grid; grid-template-columns: repeat(3, 100px);\
                 grid-template-rows: repeat(3, 100px); gap: 5px; justify-content: center; margin: 20px auto; }\
        .cell { width: 100px; height: 100px; font-size: 36px; text-align: center;\
                vertical-align: middle; border: 2px solid black; cursor: pointer; }\
        .cell.taken { cursor: not-allowed; }\
    </style>\
</head>\
<body>\
    <h1>3 en Raya con ESP32</h1>\
    <div class='board' id='board'></div>\
    <p id='status'>Turno: X</p>\
    <button onclick='resetGame()'>Reiniciar</button>\
    <script>\
        let board = ['', '', '', '', '', '', '', '', ''];\
        let currentPlayer = 'X';\
        let gameActive = true;\
        const boardElement = document.getElementById('board');\
        const statusElement = document.getElementById('status');\
        function createBoard() {\
            boardElement.innerHTML = '';\
            board.forEach((cell, index) => {\
                let cellElement = document.createElement('div');\
                cellElement.classList.add('cell');\
                cellElement.textContent = cell;\
                cellElement.onclick = () => makeMove(index);\
                boardElement.appendChild(cellElement);\
            });\
        }\
        function makeMove(index) {\
            if (board[index] === '' && gameActive) {\
                board[index] = currentPlayer;\
                currentPlayer = currentPlayer === 'X' ? 'O' : 'X';\
                checkWinner();\
                createBoard();\
                statusElement.textContent = 'Turno: ' + currentPlayer;\
            }\
        }\
        function checkWinner() {\
            const winPatterns = [\
                [0,1,2], [3,4,5], [6,7,8], [0,3,6], [1,4,7], [2,5,8], [0,4,8], [2,4,6]\
            ];\
            winPatterns.forEach(pattern => {\
                let [a, b, c] = pattern;\
                if (board[a] && board[a] === board[b] && board[a] === board[c]) {\
                    gameActive = false;\
                    statusElement.textContent = 'Ganador: ' + board[a];\
                }\
            });\
            if (!board.includes('') && gameActive) {\
                gameActive = false;\
                statusElement.textContent = 'Empate!';\
            }\
        }\
        function resetGame() {\
            board = ['', '', '', '', '', '', '', '', ''];\
            currentPlayer = 'X';\
            gameActive = true;\
            statusElement.textContent = 'Turno: ' + currentPlayer;\
            createBoard();\
        }\
        createBoard();\
    </script>\
</body>\
</html>";

// Manejo de la raíz
void handle_root() {
    server.send(200, "text/html", HTML);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado con IP: " + WiFi.localIP().toString());
    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}

void loop() {
    server.handleClient();
}
```
Funcionamiento
1. La ESP32 se conecta a la red WiFi utilizando las credenciales configuradas.
2. Se inicia un servidor web en el puerto 80.
3. Al acceder a la IP de la ESP32 desde un navegador web, se carga la página "3 en
Raya".
4. Los jugadores pueden interactuar con el juego, y el estado del tablero se gestiona en
JavaScript.
Unset
Salida en el Terminal
WiFi conectado con IP: 192.168.120
Servidor HTTP iniciado
Visualización en el Navegador
Al ingresar la dirección IP de la ESP32 en un navegador web, se despliega la interfaz del juego
"3 en Raya".
Conclusión
La práctica permitió comprender el funcionamiento de WiFi en la ESP32-S3. Se logró
implementar un servidor web para jugar "3 en Raya". Esto demuestra la versatilidad de la
ESP32 para aplicaciones de IoT y conectividad inalámbrica.

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

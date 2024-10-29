#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>

const char* ssid = "NOME_DO_WIFI";
const char* password = "SENHA_DO_WIFI";
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
}

void handleCSS() {
    File file = SPIFFS.open("/style.css", "r");
    server.streamFile(file, "text/css");
    file.close();
}

void handleJS() {
    File file = SPIFFS.open("/script.js", "r");
    server.streamFile(file, "application/javascript");
    file.close();
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    if (!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return;
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado!");

    server.on("/", handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);

    server.begin();
    webSocket.begin();

    webSocket.onEvent([](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        if (type == WStype_TEXT) {
            Serial.printf("Comando recebido: %s\n", payload);
            // Adicione o código para controlar o robô com base no comando
        }
    });
}

void loop() {
    server.handleClient();
    webSocket.loop();
}

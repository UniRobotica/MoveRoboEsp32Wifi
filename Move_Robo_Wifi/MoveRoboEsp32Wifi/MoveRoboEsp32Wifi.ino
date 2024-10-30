#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <MPU6050.h>

const char* ssid = "Robson5G";
const char* password = "12345678";

WebServer server(80);
MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

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
    Serial.println("Conectado! IP: " + WiFi.localIP().toString());

    server.on("/", handleRoot);
    server.begin();
    
    // Inicializa o MPU6050
    Wire.begin();
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("MPU6050 não conectado!");
    }
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
}

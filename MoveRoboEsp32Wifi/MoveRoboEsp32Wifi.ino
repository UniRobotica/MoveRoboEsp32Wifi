#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// Configuração da rede Wi-Fi
const char* ssid = "CLARO_2G2701F5";
const char* password = "752701F5";

// Configuração dos pinos dos motores
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

AsyncWebServer server(80);
AsyncWebSocket ws("/");

// Função para controlar o movimento do robô
void controlRobot(String direction) {
  if (direction == "forward") {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    analogWrite(motorLeftPWM, 100);

    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    analogWrite(motorRightPWM, 100);
  } else if (direction == "backward") {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    analogWrite(motorLeftPWM, 100);

    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    analogWrite(motorRightPWM, 100);
  } else if (direction == "left") {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    analogWrite(motorLeftPWM, 100);

    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    analogWrite(motorRightPWM, 100);
  } else if (direction == "right") {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    analogWrite(motorLeftPWM, 100);

    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    analogWrite(motorRightPWM, 100);
  } else {
    analogWrite(motorLeftPWM, 0);
    analogWrite(motorRightPWM, 0);
  }
}

// Callback do WebSocket para receber mensagens de direção
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                      void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    String message = String((char*)data).substring(0, len);
    controlRobot(message);
  }
}

void setup() {
  // Inicializa o Wi-Fi e o servidor
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  
  Serial.println("Conectado ao WiFi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());  // Exibe o IP do ESP32 no monitor serial

  // Configura os pinos dos motores
  pinMode(motorRightPWM, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Inicializa o WebSocket e o servidor
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  server.begin();
  Serial.println("Servidor WebSocket iniciado");
}

void loop() {
  ws.cleanupClients(); // Mantém o WebSocket ativo
}

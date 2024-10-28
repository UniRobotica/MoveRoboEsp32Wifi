#include <WiFi.h>
#include <WebServer.h>

// Configurações de Wi-Fi
const char* ssid = "Robson";
const char* password = "Rms12345";

// Configuração do servidor web na porta 80
WebServer server(80);

// Variáveis para controlar o movimento do robô
int tiltX = 0;
int tiltY = 0;

// Configuração dos pinos do motor
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Função para configurar o Wi-Fi
void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());
}

// Função de movimentação do robô
void controlRobot() {
  if (tiltY > 10) {
    moveForward();
  } else if (tiltY < -10) {
    moveBackward();
  } else if (tiltX > 10) {
    rotateRight();
  } else if (tiltX < -10) {
    rotateLeft();
  } else {
    stopMotors();
  }
}

// Funções de controle dos motores
void moveForward() { /* Lógica para mover o robô para frente */ }
void moveBackward() { /* Lógica para mover o robô para trás */ }
void rotateRight() { /* Lógica para rotacionar o robô para a direita */ }
void rotateLeft() { /* Lógica para rotacionar o robô para a esquerda */ }
void stopMotors() { /* Lógica para parar os motores */ }

// Função que recebe os dados do celular
void handleSensorData() {
  if (server.hasArg("tiltX") && server.hasArg("tiltY")) {
    tiltX = server.arg("tiltX").toInt();
    tiltY = server.arg("tiltY").toInt();
    controlRobot();
  }
  server.send(200, "text/plain", "Dados recebidos");
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  
  // Define o manipulador da rota para receber dados
  server.on("/sensor", handleSensorData);
  
  // Inicia o servidor
  server.begin();
  Serial.println("Servidor iniciado!");
}

void loop() {
  server.handleClient();
}

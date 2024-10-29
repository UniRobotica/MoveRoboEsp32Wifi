#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>

// Configurações do MPU6050
MPU6050 mpu;
int16_t ax, ay, az; // Aceleração
int16_t gx, gy, gz; // Giroscópio

// Defina os pinos do TB6612 para os motores
const int motorRightPWM = 13;  
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;

const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Variáveis para Wi-Fi
const char* ssid = "CLARO_2G2701F5"; // Substitua pelo seu SSID
const char* password = "752701F5"; // Substitua pela sua senha
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  // Inicialização do MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1);
  }

  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
}

void loop() {
  WiFiClient client = server.available(); // Espera por um cliente

  if (client) {
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Controle dos motores via requisições HTTP
    if (request.indexOf("GET /forward") != -1) {
      moveForward();
    } else if (request.indexOf("GET /backward") != -1) {
      moveBackward();
    } else if (request.indexOf("GET /stop") != -1) {
      stopMotors();
    }

    // Resposta HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>Controle do Robô</h1>");
    client.println("<button onclick=\"fetch('/forward')\">Avançar</button>");
    client.println("<button onclick=\"fetch('/backward')\">Retroceder</button>");
    client.println("<button onclick=\"fetch('/stop')\">Parar</button>");
    client.println("</html>");
    client.stop();
  }
}

// Função para mover o robô para frente
void moveForward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, 255);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, 255);
}

// Função para mover o robô para trás
void moveBackward() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, 255);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, 255);
}

// Função para parar os motores
void stopMotors() {
  analogWrite(motorLeftPWM, 0);
  analogWrite(motorRightPWM, 0);
}

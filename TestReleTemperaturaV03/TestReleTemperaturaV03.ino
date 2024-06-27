#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const int pinRele1 = 5;
const int pinRele2 = 18;

bool boolRele1 = LOW;
bool boolRele2 = LOW;

WebServer server(80);  // Servidor HTTP en el puerto 80
WebSocketsServer webSocket(81); // Servidor WebSocket en el puerto 81

Adafruit_BME280 bme; // I2C

unsigned long previousMillis = 0;
const long interval = 2000; // intervalo para actualizar los datos (2 segundos)

void setup() {
  Serial.begin(115200);
  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  digitalWrite(pinRele1, LOW);
  digitalWrite(pinRele2, LOW);

  if (!bme.begin(0x76)) {
    Serial.println("No se puede encontrar un sensor BME280 válido, verifica la conexión");
    while (1);
  }

  createAP();
  server.on("/", handleRoot);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  webSocket.loop();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendSensorData();
  }
}

void createAP() {
  // Configura el punto de acceso con SSID y contraseña
  bool apStarted = WiFi.softAP(ssid, password);
  
  if (apStarted) {
    Serial.println("Access Point started successfully");
  } else {
    Serial.println("Failed to start Access Point");
    while (true); // Detiene el programa si el AP no se inicia correctamente
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  // Generalmente será 192.168.4.1
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

String webpage() {
  float temp = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float humidity = bme.readHumidity();
  
  String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Control de Reles ESP32</title>
  <style>
    button { padding: 15px; font-size: 16px; }
    .releOn { background-color: green; color: white; }
    .releOff { background-color: red; color: white; }
  </style>
</head>
<body>
  <h1>Control de Reles ESP32</h1>
  <button id="rele1" class="releOff" onclick="toggleRelay('toggle1')">Rele 1: OFF</button>
  <button id="rele2" class="releOff" onclick="toggleRelay('toggle2')">Rele 2: OFF</button>
  <h2>Sensor BME280</h2>
  <p>Temperatura: <span id="temp">)" + String(temp) + R"rawliteral(</span> &deg;C</p>
  <p>Presion: <span id="pressure">)" + String(pressure) + R"rawliteral(</span> hPa</p>
  <p>Humedad: <span id="humidity">)" + String(humidity) + R"rawliteral(</span> %</p>
  <script>
    function toggleRelay(command) {
      var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      socket.onopen = function(event) {
        socket.send(command);
      };
      socket.onmessage = function(event) {
        if (command == 'toggle1') {
          var button = document.getElementById('rele1');
          button.className = (event.data == '1') ? 'releOn' : 'releOff';
          button.innerHTML = 'Rele 1: ' + ((event.data == '1') ? 'ON' : 'OFF');
        } else if (command == 'toggle2') {
          var button = document.getElementById('rele2');
          button.className = (event.data == '1') ? 'releOn' : 'releOff';
          button.innerHTML = 'Rele 2: ' + ((event.data == '1') ? 'ON' : 'OFF');
        }
        socket.close();
      };
    }

    function updateSensorData(temp, pressure, humidity) {
      document.getElementById('temp').innerText = temp;
      document.getElementById('pressure').innerText = pressure;
      document.getElementById('humidity').innerText = humidity;
    }

    setInterval(function() {
      var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      socket.onopen = function(event) {
        socket.send('getSensorData');
      };
      socket.onmessage = function(event) {
        var data = event.data.split(',');
        updateSensorData(data[0], data[1], data[2]);
        socket.close();
      };
    }, 2000);
  </script>
</body>
</html>
)rawliteral";
  return htmlPage;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String text = String((char *)payload);
    if (text == "toggle1") {
      boolRele1 = !boolRele1;
      digitalWrite(pinRele1, boolRele1);
      String msg = String(boolRele1);
      webSocket.broadcastTXT(msg);
      Serial.println("RELE 1 " + msg);
    } else if (text == "toggle2") {
      boolRele2 = !boolRele2;
      digitalWrite(pinRele2, boolRele2);
      String msg = String(boolRele2);
      webSocket.broadcastTXT(msg);
      Serial.println("RELE 2 " + msg);
    } else if (text == "getSensorData") {
      sendSensorData();
    }
  }
}

void sendSensorData() {
  float temp = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float humidity = bme.readHumidity();
  
  String sensorData = String(temp) + "," + String(pressure) + "," + String(humidity);
  webSocket.broadcastTXT(sensorData);
}


#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const int pinRele1 = 5;
const int pinRele2 = 18;

bool boolRele1 = HIGH;
bool boolRele2 = HIGH;
WebServer server(80);  // Servidor HTTP en el puerto 80
WebSocketsServer webSocket(81); // Servidor WebSocket en el puerto 81

void setup() {
  Serial.begin(115200);
  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  digitalWrite(pinRele1, HIGH);
  digitalWrite(pinRele2, HIGH);

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
}

void createAP() {
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  // Generalmente será 192.168.4.1
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

String webpage() {
  return R"rawliteral(
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
  </script>
</body>
</html>
)rawliteral";
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
    }
  }
}


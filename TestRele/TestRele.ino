#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "G7";
const char* password = "12345678";

const int pinRele1 = 5;
const int pinRele2 = 18;

WebServer server(80);  // Servidor HTTP en el puerto 80
WebSocketsServer webSocket(81); // Servidor WebSocket en el puerto 81

void setup() {
  Serial.begin(115200);
  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  digitalWrite(pinRele1, LOW);
  digitalWrite(pinRele2, LOW);

  connectWiFi();
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

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi. IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

String webpage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<body>
<h1>Control de Reles ESP32</h1>
<script>
document.onkeydown = function(evt) {
  evt = evt || window.event;
  var key = evt.key;
  if (key == '1') {
    toggleRelay('toggle1');
  } else if (key == '0') {
    toggleRelay('toggle2');
  }
};
function toggleRelay(command) {
  var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  socket.onopen = function(event) {
    socket.send(command);
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
      digitalWrite(pinRele1, !digitalRead(pinRele1));
    } else if (text == "toggle2") {
      digitalWrite(pinRele2, !digitalRead(pinRele2));
    }
  }
}


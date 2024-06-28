/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "MONIC";
const char* password = "1000603413";

// Relay pins
#define RLY1 17
#define RLY2 16
#define RLY3 14
#define RLY4 15

bool relayState1 = false;
bool relayState2 = false;
bool relayState3 = false;
bool relayState4 = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 17</h2>
      <p class="state">state: <span id="state1">%STATE1%</span></p>
      <p><button id="button1" class="button">Toggle</button></p>
    </div>
    <div class="card">
      <h2>Output - GPIO 16</h2>
      <p class="state">state: <span id="state2">%STATE2%</span></p>
      <p><button id="button2" class="button">Toggle</button></p>
    </div>
    <div class="card">
      <h2>Output - GPIO 14</h2>
      <p class="state">state: <span id="state3">%STATE3%</span></p>
      <p><button id="button3" class="button">Toggle</button></p>
    </div>
    <div class="card">
      <h2>Output - GPIO 15</h2>
      <p class="state">state: <span id="state4">%STATE4%</span></p>
      <p><button id="button4" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var states = event.data.split(",");
    document.getElementById('state1').innerHTML = states[0] == "1" ? "ON" : "OFF";
    document.getElementById('state2').innerHTML = states[1] == "1" ? "ON" : "OFF";
    document.getElementById('state3').innerHTML = states[2] == "1" ? "ON" : "OFF";
    document.getElementById('state4').innerHTML = states[3] == "1" ? "ON" : "OFF";
  }
  function onLoad(event) {
    initWebSocket();
    initButtons();
  }
  function initButtons() {
    document.getElementById('button1').addEventListener('click', () => { toggleRelay(1); });
    document.getElementById('button2').addEventListener('click', () => { toggleRelay(2); });
    document.getElementById('button3').addEventListener('click', () => { toggleRelay(3); });
    document.getElementById('button4').addEventListener('click', () => { toggleRelay(4); });
  }
  function toggleRelay(relay){
    websocket.send('toggle' + relay);
  }
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  String state = String(relayState1) + "," + String(relayState2) + "," + String(relayState3) + "," + String(relayState4);
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle1") == 0) {
      relayState1 = !relayState1;
      digitalWrite(RLY1, relayState1 ? LOW : HIGH);
    }
    if (strcmp((char*)data, "toggle2") == 0) {
      relayState2 = !relayState2;
      digitalWrite(RLY2, relayState2 ? LOW : HIGH);
    }
    if (strcmp((char*)data, "toggle3") == 0) {
      relayState3 = !relayState3;
      digitalWrite(RLY3, relayState3 ? LOW : HIGH);
    }
    if (strcmp((char*)data, "toggle4") == 0) {
      relayState4 = !relayState4;
      digitalWrite(RLY4, relayState4 ? LOW : HIGH);
    }
    notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE1"){
    return relayState1 ? "ON" : "OFF";
  }
  if(var == "STATE2"){
    return relayState2 ? "ON" : "OFF";
  }
  if(var == "STATE3"){
    return relayState3 ? "ON" : "OFF";
  }
  if(var == "STATE4"){
    return relayState4 ? "ON" : "OFF";
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);
  
  digitalWrite(RLY1, HIGH);
  digitalWrite(RLY2, HIGH);
  digitalWrite(RLY3, HIGH);
  digitalWrite(RLY4, HIGH);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
}

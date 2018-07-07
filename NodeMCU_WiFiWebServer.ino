const int Servo_gpio_pin = 2;
const int http_server_port = 80;

const char* ssid = "WifiName";
const char* password = "WifiPassword";

#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myservo;
WiFiServer server(http_server_port);

void setup() {
  USBInit();
  delay(10);
  ServoInit();
  delay(10);
  WifiInit();
  // prepare GPIO2
  //  pinMode(2, OUTPUT);
  //  digitalWrite(2, 0);

}

void loop() {
  LoopServer();
  //  testServo();
}

void USBInit() {
  Serial.begin(115200);
}

void ServoInit() {
  myservo.attach(Servo_gpio_pin);
}

void WifiInit() {

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void LoopServer() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  //  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  if (req.indexOf(".ico") != -1) {
    return;
  }
  Serial.println(req);
  client.flush();

  // Match the request


  // Set GPIO2 according to the request
  //  digitalWrite(2, val);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += ResponseListener(req);
  //  s += 1 ? "yes" : "no";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
//  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

String ResponseListener(String req) {
  if (req.indexOf("/control") != -1) {
    String response = "";
    response += "<a href=\"./servo_on\">Servo Turn On</a>";
    response += "<br>";
    response += "<a href=\"./servo_off\">Servo Turn Off</a>";
    response += "<br>";
    response += "<a href=\"./pwm\">testServo</a>";
    response += "<br>";
    response += "<a href=\"./read\">read</a>";
    response += "<br>";
    response += "<a href=\"./write\">write</a>";
    response += "<br>";
    response += "";
    return response;
  }
  if (req.indexOf("/pwm") != -1) {
    testServo();
    return "motor";
  }
  if (req.indexOf("/servo_on") != -1) {
    ServoTurnOn();
    return "Servo Turn On";
  }
  if (req.indexOf("/servo_off") != -1) {
    ServoTurnOff();
    return "Servo Turn Off";
  }
  if (req.indexOf("/read") != -1) {
    return "read";
  }
  if (req.indexOf("/write") != -1) {
    return "write";
  }
}

void testServo() {
  ServoTurnOn();
  delay(1000);
  ServoTurnOff();
  delay(1000);
}

void ServoTurnOn() {
  myservo.write(30);
}

void ServoTurnOff() {
  myservo.write(50);
}

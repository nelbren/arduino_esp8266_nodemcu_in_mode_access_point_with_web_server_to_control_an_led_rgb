/*
 Arduino_ESP8266_NodeMCU_Access_Point_Web_Server_Led_RGB.c 
 v0.0.1 - 2019-02-21 - nelbren.com
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "ESPWebServer";
const char* password = "12345678";

WiFiServer server(80);

int ledRed =   D0; // D0 = GPIO16
int ledGreen = D1; // D1 = GPIO05
int ledBlue =  D2; // D2 = GPIO04

int estado = 0;
char *msg;

void setup(void){
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  pinMode(ledGreen, OUTPUT);
  digitalWrite(ledGreen, HIGH);

  pinMode(ledBlue, OUTPUT);
  digitalWrite(ledBlue, HIGH);

  pinMode(ledRed, OUTPUT);
  digitalWrite(ledRed, HIGH);

  server.begin();
  Serial.println("HTTP server started");
}

void red(void) {
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  Serial.println("RED!");
}

void green(void) {
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);
  Serial.println("GREEN!");
}

void yellow(void) {
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledBlue, LOW);
  Serial.println("YELLOW!");
}

void blue(void) {
  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  Serial.println("BLUE!");
}

void white(void) {
  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, HIGH);  
  Serial.println("WHITE!");
}

void black(void) {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  Serial.println("BLACK!");
}

void demo(void) {
  Serial.println("DEMO:");
  for(int i = 0; i < 6; i++) {
    switch (i) {
      case 0: red(); break;
      case 1: green(); break;
      case 2: yellow(); break;
      case 3: blue(); break;
      case 4: white(); break;
      case 5: black(); break;
    }
    delay(1000);
  }
}

void loop(void){
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");

  while(client.connected() && !client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  String mode = "";
  
  if (request.indexOf("/BLACK") != -1) {
    black();
    mode = "BLACK";
  }

  if (request.indexOf("/RED") != -1) {
    red();
    mode = "RED";
  }
  
  if (request.indexOf("/GREEN") != -1) {
    green();
    mode = "GREEN";
  }
  
  if (request.indexOf("/YELLOW") != -1) {
    yellow();
    mode = "YELLOW";
  }
  
  if (request.indexOf("/BLUE") != -1) {
    blue();
    mode = "BLUE";
  }
  
  if (request.indexOf("/WHITE") != -1) {
    white();
    mode = "WHITE";
  }

  if (request.indexOf("/DEMO") != -1) {
    demo();
    mode = "DEMO";
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  //client.println("<head><meta http-equiv='refresh' content='1'></head>");

  if (mode == "") {
    client.println("<h1>MODE: Waiting for instructions from Cyberdyne Systems</h1>");
  } else {  
    client.println("<h1>MODE: ");
    client.println(mode);
    client.println("</h1>");
  }
  client.println("<hr>");
  client.println("<h2>");
  client.println("<a href=\"/RED\">RED</a><br>");
  client.println("<a href=\"/GREEN\">GREEN</a><br>");
  client.println("<a href=\"/YELLOW\">YELLOW</a><br>");
  client.println("<a href=\"/BLUE\">BLUE</a><br>");
  client.println("<a href=\"/WHITE\">WHITE</a><br>");
  client.println("<a href=\"/BLACK\">BLACK</a><br>");
  client.println("<br>");
  client.println("<a href=\"/DEMO\">DEMO</a><br>");
  client.println("</h2>");
  client.println("<hr>");
  client.println("<center>");
  client.println("<h3><a href='https://nelbren.com'>nelbren.com</a>&copy;2019</h3>");
  client.println("</center>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

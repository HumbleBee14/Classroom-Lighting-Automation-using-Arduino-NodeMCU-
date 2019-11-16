#include <ESP8266WebServer.h>


const int ir11 = D7; //Incoming
const int ir22 = D8; //Outgoing

int temperature, humidity, pressure, altitude;

/*Put your SSID & Password*/
const char* ssid = "Tenda_192EB0";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

ESP8266WebServer server(80);              
 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(ir11,INPUT); // For Infrared
  pinMode(ir22,INPUT); // For Infrared
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}   // Setup ENDS

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  temperature = digitalRead(ir11); // read input value
  humidity = (!digitalRead(ir11));
  pressure = digitalRead(ir22);
  altitude = (!digitalRead(ir22));
  server.send(200, "text/html", SendHTML(temperature,humidity,pressure,altitude)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

//meta http-equiv=\"refresh\" content=\"2\"  is used to AUTO REFRESH PAGE after 2 seconds

String SendHTML(int temperature,int humidity,int pressure,int altitude){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"refresh\" content=\"2\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Station</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 Smart Automatic Classroom Lighting</h1>\n";
  ptr +="<p>Temperature(IR1): ";
  ptr +=temperature;
  ptr +="&deg;C</p>";
  ptr +="<p>Humidity: ";
  ptr +=humidity;
  ptr +="%</p>";
  ptr +="<p>Pressure(IR2): ";
  ptr +=pressure;
  ptr +="hPa</p>";
  ptr +="<p>Altitude: ";
  ptr +=altitude;
  ptr +="m</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ESP8266WebServer.h>

int count=0;  //Setting Initial Count=0;
int ir1=1;
int ir2=1;

//const int relayVCC = D3;
const int relay = D6;
const int ir11 = D7; //Incoming
const int ir22 = D5; //Outgoing
int mscntON =0;
int mscntOFF =0;
String pwrs;
String in = "One Person Came In!";
String out= "One Person Came Out!";
String live = " ";

//float curses=0.0;
//const int sensorIn = A0; // Current Sensor Analog Pin

//int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module

//double Voltage = 0;
//double VRMS = 0;
//double AmpsRMS = 0;

int irsensor1, tcount, irsensor2;
//float current=0.0; 
float randNumber,anal;

/*Put your SSID & Password*/
const char* ssid = "Tenda_192EB0";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

ESP8266WebServer server(80);              
 
void setup() {                  // SETUP()
  Serial.begin(115200);
  
  //WiFi.begin(ssid, password);     //Connect to your WiFi router
  //Serial.println("_ _");
  //WiFi.disconnect();  //Prevent connecting to wifi based on previous configur
  
  randomSeed(100);
  pinMode(relay,OUTPUT);
  //pinMode(relayVCC,OUTPUT);
  //digitalWrite(relayVCC,HIGH); //Setting D3 as VCC for relay (bcoz of Les 3V Pins)
  
  digitalWrite(relay,HIGH);  //Active LOW
  pinMode(ir11,INPUT); // For Infrared
  pinMode(ir22,INPUT); // For Infrared
  Serial.println("Initiating The SYSTEM in few Seconds...");
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
  //WiFi.config(staticIP, subnet, gateway, dns);
  
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");

}

void loop() {
  
  randNumber = random(0,1)/1.2;
  ir1 = digitalRead(ir11); // read input value of IR Sensor 1
  ir2 = digitalRead(ir22);  // read input value of IR Sensor 2

  if (WiFi.status()!= WL_CONNECTED) 
  {
  WiFi.begin(ssid, password); 
  if (WiFi.status()== WL_CONNECTED) 
    {
      Serial.println("");
      Serial.println("WiFi connected..!");
      Serial.print("Got IP: ");  
      Serial.println(WiFi.localIP());
    }
  }

  // ***** Current Sensor**
/*  Voltage = getVPP();
  VRMS = (Voltage/2.0)*0.707; 
  AmpsRMS = (VRMS * 1000)/mVperAmp;
  curses = AmpsRMS; // Amperes Reading after Calibration
  if (AmpsRMS>=0.02)
  {
    Serial.print(AmpsRMS);
    Serial.println(" Amps RMS");
   }
  */
  // *****
  
   if (ir1 == LOW)
    { count=count+1; 
      Serial.println(in);
      randNumber = random(7,9)/1.2;
      live = in;
      Serial.print("Total Count: ");
      Serial.println(count);
      delay(300); // Delay between two Sensor Readings
    }
  if (ir2 == LOW)
    {
       Serial.println(out);
       randNumber = random(0,1)/1.2;
       live = out;
      if (count>0)
        {
          count=count-1; 
          Serial.print("Total Count: ");
          Serial.println(count);
          delay(300);
        }
       else
       { delay(300); }
    }

  if (count>0)  //Someone is inside
  {
    //digitalWrite(led,HIGH);
    //delay(400);
    //digitalWrite(led,LOW);
    randNumber = random(7,9)/1.2;
    digitalWrite(relay,LOW); //(Active LOW)  Switching ON relay
    delay(300);
    mscntOFF=0;  //Notification Message Count
  
  if (mscntON<1)   //Message Display Counter for ON Notification
    { 
      Serial.println("\n\nLights Turned ON\n\n");
      //delay(100);
      mscntON=mscntON+1;
    }
  }
  
  else
  {
    digitalWrite(relay,HIGH); // Switching OFF relay

    if (mscntOFF<1)   //Message Display Counter for OFF Notification
    { 
      Serial.println("\n\nLights Turned OFF\n\n");
      //delay(300);
      mscntOFF=mscntOFF+1;
      mscntON=0;  //Notification Message Count
    }
   }
  server.handleClient();     // VERY IMPORTANT CODE to Initiate Server & render page *********#########
  
}

// ******* Current Sensor Calculations
/*
float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           //record the maximum sensor value
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           //record the maximum sensor value
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
*/

// ****************** CURRENT SENSOR CALCULATION ->END

void handle_OnConnect() {
  
  irsensor1 = ir1;
  irsensor2 = ir2;
  tcount = count;
  anal = randNumber;
//  current = curses;

    if (tcount>0)
   { pwrs="ON";}
   else
   pwrs="OFF";
  
  server.send(200, "text/html", SendHTML(irsensor1,irsensor2,tcount,anal,pwrs,live)); //current 
  live=" ";
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

//meta http-equiv=\"refresh\" content=\"2\"  is used to AUTO REFRESH PAGE after 2 seconds

String SendHTML(int i1,int i2,int tc,float an, String pwr, String livest) //float cs
 {
  
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Smart Classroom Power Management</title>\n";
  ptr +="<style> html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #800000;margin: 50px auto 40px;}\n";
  ptr +="p {font-size: 24px;color: #4B0082;margin-bottom: 10px;}\n";
  ptr +="p1 {font-size: 24px;color: darkgreen;margin-bottom: 10px;}\n";
  ptr +="mark {background-color: white;color: black;}\n";
  ptr +="</style>\n";
  
  // AutoUpdateSTARTS
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,500);\n";    //AutoUpdateInterval
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.body.innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  // AutoUpdateEND
  
  ptr +="</head>\n";
  ptr +="<body style='background-color:#E6E6FA'>\n";  // Background Color
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 Smart Automatic Classroom Lighting</h1>\n";

  ptr +="<p><mark>POWER: ";
  ptr +=pwr;    //ON or OFF
  ptr +="</mark> </p>";

  ptr +="<p1>Total Count: ";
  ptr +=tc;
  ptr +=" Students</p1>";
  
  ptr +="<p>Current Sesnor(Reading): ";
  ptr +=an;   // REPLACE THIS WITH CURRENT SENSOR VARIABLE
  ptr +=" Ampere</p>";

  ptr +="<p> <mark>";
  ptr +=livest;
  ptr +=" </mark></p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

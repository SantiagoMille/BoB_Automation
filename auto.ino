// By: Luis Santiago Mille Fragoso 
// part of IGEM TEC CEM 2018

///////////////////////////////////////////////////////////////This is the Node MCU program////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

const char* ssid = "mille";
const char* password = "12345678";
char data[25] = {'t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e'};
uint addr = 0;
struct { char str[20] = "";} token;
String payload;
char tokenToSend[20]="1537430442.8130972";
long x = 1;
 
void setup () {
  //////////////////////////////////////Initial setup (Serial, eeprom, wifi and I2C)
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Wire.begin(D1, D2); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  EEPROM.begin(512);
  
  /////////////////////////////////Get, if exists, the previously generated token that is saved into memory
  EEPROM.get(addr,token);  

  /////////////////////////////////////////////////////Setup HTTP Client 
  HTTPClient http;  
  http.begin("http://igem-2018-tec.herokuapp.com/token");    
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  
  ////////////////////////////////////////////////////Build JSON for the POST request
  StaticJsonBuffer<300> JSONbuffer;   
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["token"] = String(token.str);
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  ///////////////////////////////////////////////////Send POST and get the token from server
  int httpCode = -1;
  while(httpCode<=0){
    httpCode = http.POST(JSONmessageBuffer);
  }

  ////////////////////////////////////////////////If Request code is higher that 0
  if (httpCode > 0) { 
  
    payload = http.getString();    ///////We get the response tokenToSend
	
	//////If no previously existing token then we save this token into the EEPROM 
	//////We also display this code into the serial monitor because it means this is first time use and the user needs to register the proyect on the web application
    if(strlen(token.str)<6){       
      char copy[20];
      payload.toCharArray(copy,20);
      strncpy(token.str,copy,20);
      EEPROM.put(addr,token);
      EEPROM.commit();
      Serial.println("Token: ");
      Serial.println(token.str);
      EEPROM.get(addr,token);
      Serial.print("Token after save in eeprom: ");
      Serial.println(token.str);
    }
    
    payload.toCharArray(tokenToSend,19);
  }
  http.end();   
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { 

    // Send data to slave device (Arduino) in address 8 
    Wire.beginTransmission(8); 
    Wire.write("Hello Arduino");  
    Wire.endTransmission(); 

    // Request data from the sensor of size 25 from I2C slave in adress 8
    Wire.requestFrom(8, 25); 
    int x=0;
    while(Wire.available()){
      char c = Wire.read();
      data[x]=c;
      x++;
    }
    
	/////////////////////////////////////////////////////////////////////Setup HTTP Client 
    HTTPClient https;
    https.begin("http://igem-2018-tec.herokuapp.com/sendData");  
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Accept", "application/json");
    https.addHeader("Accept-Encoding", "gzip, deflate");
    https.addHeader("Cache-Control", "no-cache");
    StaticJsonBuffer<300> JSONbuffer;  
    JsonObject& JSONencoder = JSONbuffer.createObject();  

	///////////////////////////////////////////////////////////////////Getting rid of weird characters
    for(int i =0; i<25; i++){
      if(data[i]<45||data[i]>90){
        data[i]='n';
      }
    }
	
	////////////////////////////////////////////////////Build JSON for the POST request
    JSONencoder["data"] = data;
    if(sizeof(token.str)>10){
      JSONencoder["token"] = tokenToSend;
    }else{
      JSONencoder["token"] = payload;
    }
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    int httpCode = https.POST(JSONmessageBuffer);   
    
	//////////////////////////////////////////////////Send the data to the server
    while (httpCode <= 0) { 
      httpCode = https.POST(JSONmessageBuffer);   
      payload = https.getString();  
    }
    if(httpCode>0){
      payload = https.getString(); 
    }
    https.end();
	
	/////////////////////////////////////////////////This sequence repeats every 15 min
    delay(900000);
  }
}

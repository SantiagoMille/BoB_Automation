#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//const char* ssid = "INFINITUMA87A_2.4";
//const char* password = "7582901969";
const char* ssid = "mille";
const char* password = "12345678";
char data[25] = {'t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e','t','r','u','e'};
uint addr = 0;
struct {
  char str[20] = "";
} token;
String payload;
char tokenToSend[20]="1537430442.8130872";
long x = 1;
 
void setup () {
  //////////////////////////////////////Initial setup (Serial, eeprom wifi and wire)
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Wire.begin(D1, D2); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  EEPROM.begin(512);
  EEPROM.get(addr,token);  

  /////////////////////////////////////////////////////Setup Http and get the token
  HTTPClient http;  
  //http.begin("http://htdf.requestcatcher.com/test"); 
  http.begin("http://igem-2018-tec.herokuapp.com/token");    
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  StaticJsonBuffer<300> JSONbuffer;   
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["token"] = String(token.str);
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  int httpCode = -1;
  while(httpCode<=0){
    httpCode = http.POST(JSONmessageBuffer);
  }

  ////////////////////////////////////////////////Si el codigo del request es mayor a 0 tonc
  if (httpCode > 0) { 
    payload = http.getString();    
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

    /* begin with device address 8 */
    Wire.beginTransmission(8); 
    Wire.write("Hello Arduino");  
    Wire.endTransmission(); 

    /* request data of size 4 from slave */
    Wire.requestFrom(8, 25); 
    int x=0;
    while(Wire.available()){
      char c = Wire.read();
      data[x]=c;
      x++;
    }
    
    HTTPClient https;
    //https.begin("http://htdf.requestcatcher.com/test");  
    https.begin("http://igem-2018-tec.herokuapp.com/sendData");  
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Accept", "application/json");
    https.addHeader("Accept-Encoding", "gzip, deflate");
    https.addHeader("Cache-Control", "no-cache");
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject();  

    for(int i =0; i<25; i++){
      if(data[i]<45||data[i]>90){
        data[i]='n';
      }
    }
    Serial.print("Data to send: ");
    Serial.println(data);
    JSONencoder["data"] = data;
    
    if(sizeof(token.str)>10){
      Serial.print("Token to send: ");
      Serial.println(tokenToSend);
      JSONencoder["token"] = tokenToSend;
    }else{
      Serial.print("Token to send: ");
      Serial.println(payload);
      JSONencoder["token"] = payload;
    }
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    int httpCode = https.POST(JSONmessageBuffer);   //Send the request
    
    while (httpCode <= 0) { 
      httpCode = https.POST(JSONmessageBuffer);   //Send the request
      payload = https.getString();  
      Serial.print("Payload while: ");
      Serial.println(payload);
    }
    if(httpCode>0){
      payload = https.getString(); 
      Serial.print("Payload if: ");
      Serial.println(payload);
    }
    https.end();
    //delay(900000);
    delay(10000);
  }
}

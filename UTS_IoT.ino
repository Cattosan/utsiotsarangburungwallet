#include "DHT.h"              // Download libraries dht dari adafruit beserta library sensornya
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define DHTPIN 18           // PIN menuju DHT 11
#define DHTTYPE DHT11       // Tipe DHT yang digunakan

char auth[] = "CssD9VYrP6Lz3c_kVV3oH5ARlHpE4jgg";
char ssid[] = "Long Time Ago...";
char pass[] = "BackyardiganZ[5]+";

DHT dht (DHTPIN, DHTTYPE);
BlynkTimer timer;
//--------------------------
float suhu;
int suhu2;
//--------------------------
int ledredsuhu = 19;        // LED SUHU DHT11
//int ledgreensuhu = 18;
//--------------------------
int ledredlight = 5;        // LED LDR
//int ledgreenlight = 17;
//--------------------------
int lightdata = 35;         // LDR DATA
int lux;                    
//--------------------------
//Misc
int sliderlight;
int slidersuhu;
int tewashidup;
int stateledlight;
int stateledsuhu;
//int indicator = 33;

WidgetLED ledlight(V6);
WidgetLED ledsuhu(V7);

BLYNK_CONNECTED(){
  Blynk.syncAll();
}
//--------------------------
BLYNK_WRITE(V4){
  sliderlight = param.asInt();
}
BLYNK_WRITE(V5){
  slidersuhu = param.asInt();
}
BLYNK_WRITE(V8){
  tewashidup = param.asInt();
}
//--------------------------

void light(){
  int adc = analogRead(lightdata);
  lux = (0.009768*adc)+10;
  Blynk.virtualWrite(V0, lux);
  Blynk.virtualWrite(V2, lux);
}

void temp(){
  if(isnan(suhu)){
    Serial.println("DHT disconnected!");
    return;
  }
  suhu = dht.readTemperature();
  suhu2 = ("%2.2",suhu);
  Blynk.virtualWrite(V1,suhu2);
  Blynk.virtualWrite(V3,suhu2);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,7), 8080);
  //Blynk.begin(auth, ssid, pass, "admin@admin.com-Blynk", 8080);
  //--------------------------
  pinMode(ledredsuhu, OUTPUT);
  //pinMode(ledgreensuhu, OUTPUT);
  pinMode(ledredlight, OUTPUT);
  //pinMode(ledgreenlight, OUTPUT);
  //pinMode(indicator, OUTPUT);
  /*
  terminal.println("Sangkar Burung v1.0");
  terminal.println("Connected!");
  terminal.flush();
  */
  dht.begin();
  timer.setInterval(1000L, light);
  timer.setInterval(1000L, temp);
}

void loop() {
  // put your main code here, to run repeatedly:  
  if(tewashidup == 1){
    Blynk.run();
    timer.run();
    //digitalWrite(indicator, HIGH);
    Serial.println("On");
    light();
    temp();
    //-------------------------- TEMP SECTION
    if(suhu >= slidersuhu){
      digitalWrite(ledredsuhu, HIGH);
      //digitalWrite(ledgreensuhu, LOW);
      ledsuhu.on();
      stateledsuhu = 1;
    }
    else{
      digitalWrite(ledredsuhu, LOW);
      //digitalWrite(ledgreensuhu, HIGH);
      ledsuhu.off();
      stateledsuhu = 0;
    }
    //-------------------------- LIGHT SECTION
    if(lux >= sliderlight){
      digitalWrite(ledredlight, HIGH);
      //digitalWrite(ledgreenlight, LOW);
      ledlight.on();
      stateledlight = 1;
    }
    else{
      digitalWrite(ledredlight, LOW);
      //digitalWrite(ledgreenlight, HIGH);
      ledlight.off();
      stateledlight = 0;
    }
  }
  else{
    //digitalWrite(indicator, LOW);
    Serial.println("Off");
    if(stateledsuhu == 1){
      ledsuhu.on();
    }
    else{
      ledsuhu.off();
    }

    if(stateledlight == 1){
      ledlight.on();
    }
    else{
      ledlight.off();
    }
    
    digitalWrite(ledredsuhu, LOW);
    //digitalWrite(ledgreensuhu, LOW);
    
    digitalWrite(ledredlight, LOW);
    //digitalWrite(ledgreenlight, LOW);
  }
}

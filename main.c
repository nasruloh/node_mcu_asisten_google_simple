//----------------------My Name is Nasruloh----------------
//---------------------nasruloh0@gmail.com-----------------

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
//---------------RTC----------------- 
//RTC_DS3231 rtc;
 
//char t[32];
//-----------------------------------
#define buzer             D7
#define Relay1            D6
#define Relay2            D5
#define Relay3            D4
#define Relay4            D3
#define indikator         D0
//---------------------------------------------------------
#define WLAN_SSID       "q"             // Your SSID
#define WLAN_PASS       ""        // Your password

//-------------------------------------------blynk----------------------------------
char auth[] = "VS6uALE9xWU9TxvUjbC_ejKI6DN5M0Tu";
char ssid[] = "q";
char pass[] = "";
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "nasruloh"            // Replace it with your username
#define AIO_KEY         "aio_INMs059vKgb4wOtMTi6361z1Ymjr"   // Replace with your Project Auth Key

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay1"); // FeedName
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lampu depan");



void MQTT_connect();

void setup() {
  Serial.begin(115200);
  
  Wire.begin();   //Setting wire (5 untuk SDA dan 4 untuk SCL)

  lcd.begin();
  lcd.setCursor(5,0);
  lcd.print("W");delay(1000); lcd.print("E");delay(500);
  lcd.print("L");delay(500); lcd.print("C");delay(500);
  lcd.print("O");delay(500); lcd.print("M");delay(500);
  lcd.print("E");delay(500); 
  
  lcd.setCursor(3,1);
  lcd.print("S");delay(200); lcd.print("M");delay(200);
  lcd.print("A");delay(200); lcd.print("R");delay(200);
  lcd.print("T");delay(200); lcd.print(" H");delay(200);
  lcd.print("O");delay(200); lcd.print("M");delay(200);
  lcd.print("E");delay(2000);
  lcd.clear();

  //---------------------------------------------------------------
  //rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));  //Setting Time
  
  // Kalian dapat menambahkan bagian dibawah ini untuk set manual jam
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(indikator, OUTPUT);
  pinMode(buzer,OUTPUT);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  digitalWrite(indikator, LOW); 
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  digitalWrite(indikator, HIGH);
  digitalWrite(buzer, HIGH);
  delay(500); 
  lcd.setCursor(0, 1);
  lcd.print("Waiting..");
  digitalWrite(indikator, LOW);
  digitalWrite(buzer, LOW);
  delay(500);
  digitalWrite(indikator, HIGH); 
  digitalWrite(buzer, HIGH);
  delay(500);
  digitalWrite(buzer, LOW);
  digitalWrite(indikator, LOW);
  delay(500);
  digitalWrite(indikator, HIGH);
  digitalWrite(buzer, HIGH); 
  delay(500);
  digitalWrite(indikator, LOW);
  digitalWrite(buzer, LOW);
  delay(1000);
  
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
  
   Blynk.begin(auth, ssid, pass);

}

void loop() {
  //waktu();
  layar();
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Relay1, Light1_State);

    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Relay2, Light2_State);

    }
    if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(Relay3, Light3_State);

    }
    if (subscription == &Light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      digitalWrite(Relay4, Light4_State);

    }
  }
  
  Blynk.run();

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    digitalWrite(indikator, LOW);
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  digitalWrite(indikator, HIGH);
  digitalWrite(buzer, HIGH);
  delay(500);
  digitalWrite(buzer,LOW);
}
/*void waktu()
{
  DateTime now = rtc.now();       //Menampilkan RTC pada variable now

  Serial.print("Tanggal : ");
  Serial.print(now.day());        //Menampilkan Tanggal
  Serial.print("/");
  Serial.print(now.month());      //Menampilkan Bulan
  Serial.print("/");
  Serial.print(now.year());       //Menampilkan Tahun
  Serial.print(" ");
  
  Serial.print("Jam : ");
  Serial.print(now.hour());       //Menampilkan Jam
  Serial.print(":");
  Serial.print(now.minute());     //Menampilkan Menit
  Serial.print(":");
  Serial.print(now.second());     //Menampilkan Detik
  Serial.println();

}*/
void layar(){
    //int u;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("SMART HOME");
    lcd.setCursor(3, 1);
    lcd.print("By:Anas");
    //for (u = 0 ; u < 16; u++) {
    //lcd.scrollDisplayLeft();
    //lcd.scrollDisplayRight();
    delay(500);
  //}
  }

//#include <ArduinoJson.h>
//#include "FS.h"

//Debes cargar la librerías / You must to install the following libraries:

//https://github.com/plerup/espsoftwareserial

//https://github.com/squix78/esp8266-oled-ssd1306




#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <SoftwareSerial.h>
#include "TinyGPS++.h" 

#include <WiFiUdp.h>
#include <EEPROM.h>
 #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
 #include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

 
int LED = 2; //led status Parpadea = Todo OK
//SoftwareSerial swSer(15,-1, false, 256); 
//SoftwareSerial swSer(15,-1, true, 256);//gps OLD
SoftwareSerial swSer(15,-1, false, 256);//gps NEO 6M
 
static const uint8_t GPSBaud4800 = 4800 ;                  //  
static const uint8_t GPSBaud9600 = 9600 ;                  // 
 
uint8_t errorWifi = 0;
uint8_t rotateWifiAt = 10;//
uint8_t currentWifi = 0;
uint8_t blinkLCD = 0;
int gpsFails = 0;
 // Initialize the OLED display using Wire library
 SSD1306  display(0x3c, 5, 4);

 
 WiFiUDP Udp;
// WebServer
ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;
boolean printToWeb = false;
//String printWebString = "";
//unsigned long wdcounter = 0;
//unsigned long timerAPoff = 0;
//uint16_t lowestRAM = 0;
//String lowestRAMfunction = "";


double lastLatSent  = 0;
double lastLongSent = 0;


TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps

int  connectionFailsTotal = 0;
int  connectionFails = 0;
long connectionsGood = 0;
//SimpleTimer timer;

unsigned long currentMillis      = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousEachSecond = 0;
unsigned long distanceKmToLastIP   = 0;  // distancia a la última posición enviada via TCP/IP
//boolean KISS = false;
 
boolean enviado = false;
boolean autoReset = true;
//char * lf = "\r\n";
boolean gotPacket = false;
//AX25Msg incomingPacket;
//char * srccall;
//char * dstcall;
//char * datapayload;
 

struct appSettingsStructure
{
  char          WifiSSID[32];
  char          WifiKey[32];
  char          WifiSSID2[32];
  char          WifiKey2[32];  
  char          APRSid[32];
  char          APRSpass[32];
  char          Ipinterval[32];
  char          distanceLow[32];//<50
  char          distanceHigh[32];//>50  
  char          RFinterval[32];
  char          APRSServer[32];
  char          OK[32];  
  char          GPS[32];  
} appSettings;


int count = 0;
 
 

void setup() {
  loadConfig();  

  connectionsGood = 0;   
  count = 9999999;
  gpsFails = 0;
  int ret;
  Serial.begin(115200);
  // delete old config
  WiFi.disconnect(true);
  pinMode(LED, OUTPUT);
  pinMode(0,OUTPUT);
  
  initLCD();
    
  WiFi.onEvent(WiFiEvent);  
  Serial.print("Connecting to ");
  Serial.println(appSettings.WifiSSID);
  delay(500);
  WiFi.begin(appSettings.WifiSSID, appSettings.WifiKey);
  
  uint8_t timeout = 20; // 10 * 500 ms = 5 sec time out
  // 500ms loop
  while ( ((ret = WiFi.status()) != WL_CONNECTED) && timeout ) {
    Serial.print(".");
    delay(500);
    --timeout;
  }
  setupWebServer();
  if(appSettings.GPS=="9600"){
    swSer.begin(9600);
  }else{
    swSer.begin(4800);
  }
  readGPS();  
}

void executeEachSecond(){
    unsigned long distanceNum = 0;
    unsigned long distanceNumLow  = atol(appSettings.distanceLow);
    unsigned long distanceNumHigh = atol(appSettings.distanceHigh);
    if (currentMillis - previousEachSecond >= 1000 ) {
        sendAPRSTCPIP();
        updateLCD();
        //previousEachSecond += 1000;
        previousEachSecond = millis();
       readGPS();
    }

  
     if(gps.location.isValid() && (lastLatSent != lastLongSent )){      
           distanceKmToLastIP =
            (unsigned long)TinyGPSPlus::distanceBetween(
              gps.location.lat(),
              gps.location.lng(),
              lastLatSent, 
              (lastLongSent)) ;// 1000;
     }

    if(gps.speed.kmph()>50){
      distanceNum = distanceNumHigh;
      }else{
      distanceNum = distanceNumLow;
    }
    if( (distanceKmToLastIP > distanceNum) && (gps.location.isValid() && ( distanceKmToLastIP > 0 )) ){
      Serial.print("Envio por dist MAX - Distancia: ");
      Serial.println(distanceKmToLastIP);
      sendGPStoINET();
    }

  }
 
void loop() {
  readGPS();
  handleWebServer();
  readGPS();
  serial();//test serial command
  readGPS();
  currentMillis = millis();
  executeEachSecond();
  readGPS();
  checkErrors();
}
  
  
static void readGPS(){
    yield();
    ESP.wdtFeed();
    while (swSer.available()>0){
        gps.encode(swSer.read());
    }
}



 

  
void WiFiEvent(WiFiEvent_t event) {
    
    //Serial.printf("[WiFi-event] event: %d\n", event);

    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            rotateWifiAt = 0;
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            Serial.println("WiFi lost connection");
            errorWifi++;
            if(errorWifi>rotateWifiAt){
              if(currentWifi==0){
                currentWifi=1;
                WiFi.disconnect(); 
                delay(100);
                WiFi.mode(WIFI_STA);  
                WiFi.begin(appSettings.WifiSSID2, appSettings.WifiKey2);
                Serial.println("----WIFI ROTATING 2");
                errorWifi = 0;
              }else{
                currentWifi=0;
                WiFi.disconnect(); 
                delay(100);
                WiFi.mode(WIFI_STA);  
                WiFi.begin(appSettings.WifiSSID, appSettings.WifiKey);
                Serial.println("----WIFI ROTATING 1");
                errorWifi = 0;
              }
            }
            break;
    }
}

 
static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
//  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
//  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
//  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
//  smartDelay(0);
}

 

void DegreesToDegMinSec(float x)
{
  int deg=x;
  float minutesRemainder = abs(x - deg) * 60;
  int arcMinutes = minutesRemainder;
  float arcSeconds = (minutesRemainder - arcMinutes) * 60;
  Serial.print(deg);Serial.print(" ");
 
 
  Serial.print(minutesRemainder);
  

}

char *DegreesToDMm(float lat,float lng)
{
  char *response = "                  ";
  char *latCar = "N";
  char *lngCar = "E";
  int degLat=lat;
  float minutesRemainderLat = abs(lat - degLat) * 60;
  int minutosLat = minutesRemainderLat;
  int decMinutosLat = (minutesRemainderLat*100) - (minutosLat*100);
   
  int degLng=lng;
  float minutesRemainderLng = abs(lng - degLng) * 60;  
  int minutosLng = minutesRemainderLng;
  int decMinutosLng = (minutesRemainderLng*100) - (minutosLng*100);   

  if(degLat<0){
     latCar = "S";
     degLat = degLat * -1;
  }
  if(degLng<0){
     lngCar = "W";    
     degLng = degLng * -1; 
  }
 
  //Serial.print(minutesRemainder);
 //4026.34N/00321.72W
  sprintf (response, "%02d%02d.%02d%s/%03d%02d.%02d%s", degLat,minutosLat,decMinutosLat,latCar,degLng,minutosLng,decMinutosLng,lngCar);
  //Serial.print(response);
  return(response);
}




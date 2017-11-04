void sendAPRSTCPIP(){


    boolean allValid = true;


         
        readGPS();
        if(gps.location.isValid()){  
            digitalWrite(LED, !digitalRead(LED));        
        }   
        
        count++;
        readGPS();       
        if(!gps.location.isValid()){
          Serial.print("POSICION INVALIDA - SATS:  ");
          
          String satsVisible = String(gps.satellites.value());
          Serial.print(satsVisible);
          Serial.print("  | charsProcessed:  ");
          printInt(gps.charsProcessed(), true, 6);
          Serial.println();
          gpsFails++;
          allValid = false;
        }else{
          gpsFails = 0;
        }
        if(WiFi.status()!=WL_CONNECTED){
          Serial.println("NO HAY CONEXION WIFI");    
          allValid = false;
        }

        if(allValid){  
              if(count>atol(appSettings.Ipinterval) or count == 9999999 ){
     
                if(sendGPStoINET()!=-1){
                  count = 0;  
                  connectionFails = 0;
                  connectionsGood ++;
                  Serial.print("-------->>>> Correct connections: ");
                  Serial.print(connectionsGood);
                  Serial.println("--------<<<<");
                  Serial.println();
                }else{
                  connectionFails ++;
                  connectionFailsTotal ++;
                }
              
              }else{
              
                Serial.print("-");
                Serial.print(count);
              }
        }


        readGPS();       
        int result = gps.time.minute() % 5; 
        
        if((result == 0) and ( gps.time.second()<=3) and (gps.time.second()>=0 )){
          //sendGPStoINET();
          }

  
          
 
         if(gps.location.isValid()){
 
/*        
        
          printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
          printInt(gps.hdop.value(), gps.hdop.isValid(), 5);
          printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
          printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
          printInt(gps.location.age(), gps.location.isValid(), 5);
          printDateTime(gps.date, gps.time);
          printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
          printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
          printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
          printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);
         unsigned long distanceKmToLondon =
            (unsigned long)TinyGPSPlus::distanceBetween(
              gps.location.lat(),
              gps.location.lng(),
              LONDON_LAT, 
              LONDON_LON) / 1000;
          //printInt(distanceKmToLondon, gps.location.isValid(), 9);
        
          double courseToLondon =
            TinyGPSPlus::courseTo(
              gps.location.lat(),
              gps.location.lng(),
              LONDON_LAT, 
              LONDON_LON);
        
          //printFloat(courseToLondon, gps.location.isValid(), 7, 2);
        
          //const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
        
          //printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
        
          printInt(gps.charsProcessed(), true, 6);
          //printInt(gps.sentencesWithFix(), true, 10);
          printStr("CHECKSUM: ",10);
          printInt(gps.failedChecksum(), true, 9);
        
     
         
          //DegreesToDegMinSec(gps.location.lat());
          //DegreesToDegMinSec(gps.location.lng());
          Serial.print("|");
          Serial.print(DegreesToDMm(gps.location.lat(),gps.location.lng()));
          Serial.print("|");
         
          Serial.println();  
*/             
          }else{
          //printStr("NO DATA",7);
          //Serial.println();  
          }
  

   
}


int sendGPStoINET(){

  char login[60];
  char sentence[150];

  readGPS();
  if(!gps.location.isValid()){
    Serial.println("POSICION INVALIDA");
  }
  if(WiFi.status()!=WL_CONNECTED){
    Serial.println("NO HAY CONEXIÓN WIFI");      
  }
 
  if(gps.location.isValid() and WiFi.status()==WL_CONNECTED){    
        sprintf(login, "user EA4GKQ-6 pass 18869 vers MicroGKQ\r\n");
        Serial.println();
        printDateTime(gps.date, gps.time);  
        Serial.println();
/*         
        WiFiClient client;
  //        if (!client.connect("srvr.aprs-is.net", 8080 )) {
          if (!client.connect("euro.aprs2.net", 14580)) {
//        if (!client.connect("euro.aprs2.net", 10152 )) {
                  //Serial.println( client.read());//client response
                  Serial.println("connection failed: ");
                  client.flush();
                  client.stop();
                  return -1;
        } 
       
        client.print(login);
       
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return -1;
          }
          handleWebServer();
        }
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
*/        
        readGPS();


    const char* callsign = appSettings.APRSid;
    const char* APRSpass = appSettings.APRSpass;
    char* positionCurrent = DegreesToDMm(gps.location.lat(),gps.location.lng());
    // send back a reply, to the IP address and port we got the packet from
    if(appSettings.APRSpass!="99999"){
        Udp.beginPacket(appSettings.APRSServer, 8080);
        char message[255];// = "user "+callsign+" pass 12345 MicroGKQ 0.2\r\n"; 
        //Udp.write("user "+callsign+"-6 pass "+APRSpass+" MicroGKQ 0.2\r\n");    
    
        sprintf(message,"user %s pass %s vers EasyAPRS v1\r\n",callsign,appSettings.APRSpass);
        Udp.write(message);
        Serial.println(message);
        //sprintf(message,"%s>APP601,TCPIP*:!%s>EasyAPRS v1\r\n",callsign,positionCurrent);
        //APP601
        int speedKmph =  int(gps.speed.mph());
        int courseDeg =  int(gps.course.deg());
        if(speedKmph<1){
          courseDeg = 0;
        }
        sprintf(message,"%s>APP601,TCPIP*:!%s(%03d/%03d>EasyAPRS v1 \r\n",callsign,positionCurrent,courseDeg,speedKmph);
        Udp.write(message);
        Serial.println(message);
        Udp.endPacket();      
    }
    lastLatSent  = gps.location.lat();
    lastLongSent = gps.location.lng();    
  }
} 



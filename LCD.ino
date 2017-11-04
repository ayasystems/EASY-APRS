
 void initLCD(){
  char message[32];
  display.init();

  display.flipScreenVertically();

  display.setContrast(255);

  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "   EASY APRS v1");
    
    display.setFont(ArialMT_Plain_16);
    sprintf(message,"Booting");  
    display.drawString(30, 25, message);

    display.setFont(ArialMT_Plain_16);
    sprintf(message,"=-=-=-=-=-=-=-=-=-=-=-=-=-");  
    display.drawString(0, 45, message);
        
    display.display();  
  
  }


  void updateLCD(){
  
     char message[32]; 
 
       
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "   EASY APRS v1");
    display.setFont(ArialMT_Plain_16);
 
    //sprintf(message,"%02d",gps.location.lat());  
    display.drawString(20, 17, String(gps.location.lat()));
    //sprintf(message,"%02d",gps.location.lng());  
    display.drawString(70, 17, String(gps.location.lng()));

    display.setFont(ArialMT_Plain_10);
    display.drawString(0,34,"Last sent:");
    display.drawString(50,34,String(distanceKmToLastIP));
    display.drawString(100,34,String((int)gps.speed.kmph()));
    display.setFont(ArialMT_Plain_10);



    if(blinkLCD==0){
     sprintf(message,"*");  
     blinkLCD = 1;
    }else if(blinkLCD==1){
     sprintf(message,"-");   
     blinkLCD = 2;
    }else if(blinkLCD==2){
     sprintf(message,"|");   
     blinkLCD = 0;
    } 
    
    display.drawString(120, 34, message);

    display.setFont(ArialMT_Plain_10);
    //sprintf(message,"Ip: %02d",gps.satellites.value());
    display.drawString(0, 47, WiFi.localIP().toString());
    
    sprintf(message,"SATs: %02d",gps.satellites.value());  
    display.drawString(83, 47, message);
    
      display.display();
       
    
}

  

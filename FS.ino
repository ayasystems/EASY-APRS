/*
 * Revisar 
 * http://www.esp8266.com/viewtopic.php?f=32&t=4477
 */

 
bool loadConfig() { 
  int EESize = 0;
  EEPROM.begin(512);
  EEPROM.get(EESize, (appSettings.WifiSSID));
  Serial.print("appSettings.WifiSSID  :");
  Serial.println(appSettings.WifiSSID);
  
  EESize = EESize + sizeof(appSettings.WifiKey);
  EEPROM.get(EESize, appSettings.WifiKey);
  Serial.print("appSettings.WifiKey  :");
  Serial.println(appSettings.WifiKey);

  EESize = EESize + sizeof(appSettings.WifiSSID2);
  EEPROM.get(EESize, appSettings.WifiSSID2);
  Serial.print("appSettings.WifiSSID2  :");
  Serial.println(appSettings.WifiSSID2);

  EESize = EESize + sizeof(appSettings.WifiKey2);
  EEPROM.get(EESize, appSettings.WifiKey2);
  Serial.print("appSettings.WifiKey2  :");
  Serial.println(appSettings.WifiKey2);  

  EESize = EESize + sizeof(appSettings.APRSid);
  EEPROM.get(EESize, appSettings.APRSid);
  Serial.print("appSettings.APRSid  :");
  Serial.println(appSettings.APRSid);


  EESize = EESize + sizeof(appSettings.APRSpass);
  EEPROM.get(EESize, appSettings.APRSpass);
  Serial.print("appSettings.APRSpass  :");
  Serial.println(appSettings.APRSpass);

  
  EESize = EESize + sizeof(appSettings.Ipinterval);
  EEPROM.get(EESize, appSettings.Ipinterval);
  Serial.print("appSettings.Ipinterval  :");
  Serial.println(appSettings.Ipinterval);

  
  EESize = EESize + sizeof(appSettings.distanceLow);
  EEPROM.get(EESize, appSettings.distanceLow);
  Serial.print("appSettings.distanceLow  :");
  Serial.println(appSettings.distanceLow);

  EESize = EESize + sizeof(appSettings.distanceHigh);
  EEPROM.get(EESize, appSettings.distanceHigh);
  Serial.print("appSettings.distanceHigh  :");
  Serial.println(appSettings.distanceHigh);
  
  EESize = EESize + sizeof(appSettings.RFinterval);
  EEPROM.get(EESize, appSettings.RFinterval);
  Serial.print("appSettings.RFinterval  :");
  Serial.println(appSettings.RFinterval);


  EESize = EESize + sizeof(appSettings.APRSServer);
  EEPROM.get(EESize, appSettings.APRSServer); 
  Serial.print("appSettings.APRSServer  :");
  Serial.println(appSettings.APRSServer);
 

  EESize = EESize + sizeof(appSettings.OK);
  EEPROM.get(EESize, appSettings.OK); 
  Serial.print("appSettings.OK  :");
  Serial.println(appSettings.OK);
 

  EESize = EESize + sizeof(appSettings.GPS);
  EEPROM.get(EESize, appSettings.GPS); 
  Serial.print("appSettings.GPS  :");
  Serial.println(appSettings.GPS);
  EEPROM.end();  
  
  if(String(appSettings.OK)!="OK"){
    Serial.println("Cleaning settings. May be is the first use");
    clearConfig();
    saveConfig();
    
  }
  
  return true;
}

void clearConfig(){

    strncpy(appSettings.WifiSSID, "microgkq", sizeof(appSettings.WifiSSID));
    strncpy(appSettings.WifiKey, "", sizeof(appSettings.WifiKey));
    strncpy(appSettings.WifiSSID2, "microgkq", sizeof(appSettings.WifiSSID2));
    strncpy(appSettings.WifiKey2, "microgkq", sizeof(appSettings.WifiKey2));
    strncpy(appSettings.APRSid, "EA4----1", sizeof(appSettings.APRSid));
    strncpy(appSettings.APRSpass, "99999", sizeof(appSettings.APRSpass));
    strncpy(appSettings.Ipinterval, "240", sizeof(appSettings.Ipinterval));
    strncpy(appSettings.distanceLow, "300", sizeof(appSettings.distanceLow));
    strncpy(appSettings.distanceHigh, "1000", sizeof(appSettings.distanceHigh));    
    strncpy(appSettings.RFinterval, "240", sizeof(appSettings.RFinterval));
    strncpy(appSettings.APRSServer, "rotate.aprs.net", sizeof(appSettings.APRSServer));
    strncpy(appSettings.OK, "OK", sizeof(appSettings.OK));
    strncpy(appSettings.GPS, "4800", sizeof(appSettings.GPS));    
  }

bool saveConfig() { 
  int EESize = 0;
  EEPROM.begin(512);
  EEPROM.put(EESize, (appSettings.WifiSSID));
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.WifiSSID  :");
  Serial.println(appSettings.WifiSSID);
  
  EESize = EESize + sizeof(appSettings.WifiKey);
  EEPROM.put(EESize, appSettings.WifiKey);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.WifiKey  :");
  Serial.println(appSettings.WifiKey);

  EESize = EESize + sizeof(appSettings.WifiSSID2);
  EEPROM.put(EESize, appSettings.WifiSSID2);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.WifiSSID2  :");
  Serial.println(appSettings.WifiSSID2);

  EESize = EESize + sizeof(appSettings.WifiKey2);
  EEPROM.put(EESize, appSettings.WifiKey2);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.WifiKey2  :");
  Serial.println(appSettings.WifiKey2);  

  EESize = EESize + sizeof(appSettings.APRSid);
  EEPROM.put(EESize, appSettings.APRSid);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.APRSid  :");
  Serial.println(appSettings.APRSid);


  EESize = EESize + sizeof(appSettings.APRSpass);
  EEPROM.put(EESize, appSettings.APRSpass);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.APRSpass  :");
  Serial.println(appSettings.APRSpass);

  
  EESize = EESize + sizeof(appSettings.Ipinterval);
  EEPROM.put(EESize, appSettings.Ipinterval);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.Ipinterval  :");
  Serial.println(appSettings.Ipinterval);

  
  EESize = EESize + sizeof(appSettings.distanceLow);
  EEPROM.put(EESize, appSettings.distanceLow);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.distanceLow  :");
  Serial.println(appSettings.distanceLow);

  EESize = EESize + sizeof(appSettings.distanceHigh);
  EEPROM.put(EESize, appSettings.distanceHigh);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.distanceHigh  :");
  Serial.println(appSettings.distanceHigh);  

  
  EESize = EESize + sizeof(appSettings.RFinterval);
  EEPROM.put(EESize, appSettings.RFinterval);
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.RFinterval  :");
  Serial.println(appSettings.RFinterval);


  EESize = EESize + sizeof(appSettings.APRSServer);
  EEPROM.put(EESize, appSettings.APRSServer); 
  Serial.print(EESize);
  Serial.print(" size - "); 
  Serial.print("appSettings.APRSServer  :");
  Serial.println(appSettings.APRSServer);

  strncpy(appSettings.OK, "OK", sizeof(appSettings.OK));
  EESize = EESize + sizeof(appSettings.OK);
  Serial.print(EESize);
  Serial.print(" size - "); 
  EEPROM.put(EESize, appSettings.OK); 
  Serial.print("appSettings.OK  :");
  Serial.println(appSettings.OK);  

 
  EESize = EESize + sizeof(appSettings.GPS);
  Serial.print(EESize);
  Serial.print(" size - "); 
  EEPROM.put(EESize, appSettings.GPS); 
  Serial.print("appSettings.GPS  :");
  Serial.println(appSettings.GPS);    

  EEPROM.commit();
  EEPROM.end();
    
  return true;
}
 
 

 

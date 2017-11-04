 void bootInfo(){
  
Serial.print("Flash Size: ");  
Serial.print(ESP.getFlashChipRealSize() / 1024);
Serial.println(" kB");

Serial.print("Sketch Size/Free: ");
Serial.print(ESP.getSketchSize() / 1024);
Serial.print(" kB / ");
Serial.print(ESP.getFreeSketchSpace() / 1024);
Serial.println(" kB");

Serial.println();
Serial.print("GPIO 15: ");  
Serial.println(digitalRead(15));  
Serial.print("GPIO  0: ");  
Serial.println(digitalRead(0));
Serial.print("GPIO  2: ");  
Serial.println(digitalRead(2));

  }



void doReboot(){
    Serial.println("    ESP.restart()    ");
    digitalWrite(LED, LOW);
    digitalWrite(0, HIGH);    
    if(autoReset){    
       ESP.reset();
    }  
  
}

void checkErrors(){
    if(gpsFails>120){//si en 2 minutos no hay posición reiniciamos, evitamos errores de arranque del GPS
      doReboot();
    }
 
    if(connectionFails>10){
      doReboot();
    }
    if (millis() > 5000 && gps.charsProcessed() < 10){
        Serial.println(F("No GPS data received: check wiring"));
    }  
 
}   

/********************************************************************************************\
* Get data from Serial Interface
\*********************************************************************************************/
#define INPUT_BUFFER_SIZE          128
#define COMMAND_WIFI 0x01
#define COMMAND_PASS 0x02
#define COMMAND_REBOOT 0x97
#define COMMAND_SAVE 0x98
#define NONE 0x99
int commandRead = NONE;

byte SerialInByte;
int SerialInByteCounter = 0;
char InputBuffer_Serial[INPUT_BUFFER_SIZE + 2];

void serial()
{
  while (Serial.available())
  {
    yield();
    SerialInByte = Serial.read();
    if (SerialInByte == 255) // binary data...
    {
      Serial.flush();
      return;
    }

    if (isprint(SerialInByte))
    {
      if (SerialInByteCounter < INPUT_BUFFER_SIZE) // add char to string if it still fits
        InputBuffer_Serial[SerialInByteCounter++] = SerialInByte;
    }

    if (SerialInByte == '\n')
    {
      InputBuffer_Serial[SerialInByteCounter] = 0; // serial data completed
      Serial.write('>');
      Serial.println(InputBuffer_Serial);
      String action = InputBuffer_Serial;
      if(commandRead == COMMAND_WIFI){

        //appSettings.WifiSSID = InputBuffer_Serial;
        strncpy(appSettings.WifiSSID, InputBuffer_Serial, sizeof(appSettings.WifiSSID));
        saveConfig();
        serialClean();
        commandRead=NONE;
        return;
      }else if(commandRead == COMMAND_PASS){
        strncpy(appSettings.WifiKey, InputBuffer_Serial, sizeof(appSettings.WifiKey));
        saveConfig();
        serialClean();
        return;
      }else if(commandRead == COMMAND_SAVE){
        saveConfig();
        serialClean();
        commandRead=NONE;
        return;
      }else if(commandRead == COMMAND_REBOOT){
        commandRead=NONE;
        doReboot();
        return;
      }
      if (strcmp("WIFI", InputBuffer_Serial) == 0)
     {
        commandRead = COMMAND_WIFI;
        Serial.println("Enter wifi ssid and press INTRO");
        serialClean();
        return;
     } 
      if (strcmp("PASS", InputBuffer_Serial) == 0)
     {
        commandRead = COMMAND_PASS;
        Serial.println("Enter wifi password and press INTRO");
        serialClean();
        return;
     } 
      if (strcmp("SAVE", InputBuffer_Serial) == 0)
     {
        Serial.println("Save settings");
        saveConfig();
        serialClean();
        commandRead=NONE;
        return;
     } 
      if (strcmp("REBOOT", InputBuffer_Serial) == 0)
     {
        Serial.println("Reboot!!!");
        commandRead=NONE;
        doReboot();
        return;
     }  
      if (strcmp("HELP", InputBuffer_Serial) == 0)
     {
        Serial.println("Commands available | Comandos disponibles");
        Serial.println("WIFI -> Setup your wifi");
        Serial.println("PASS -> Set your wifi password");
        Serial.println("SAVE -> Save settings");
        Serial.println("REBOOT -> Reboot device");
        commandRead=NONE;
        return;
     }         
        serialClean();
    }
   
  }
}

void serialClean(){
          SerialInByteCounter = 0;
        InputBuffer_Serial[0] = 0; // serial data processed, clear buffer     
        
  }

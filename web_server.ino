

void handleWebServer(){
  
    WebServer.handleClient();//Procesar peticiones web
  
}

void setupWebServer(){
 
    WebServer.on("/", handleRoot); 
    WebServer.on("/settings",handleSettings);
    WebServer.on("/inline", [](){   
      WebServer.send(200, "text/plain", "this works as well");  
    });
    WebServer.on("/reboot", [](){   
      WebServer.send(200, "text/plain", "rebooting.........");  
      doReboot();
    });    

    WebServer.onNotFound(handleNotFound);
  httpUpdater.setup(&WebServer);
  WebServer.begin();

 
  Serial.println("HTTP server started");
  }

void handleRoot() {
  Serial.println("handleRoot");
  String reply = "";
  char buffer2 [2];
  reply = F("<html>");
  reply += F("<body>");

  reply = F("<!DOCTYPE html>");
  reply = F("<html lang=\"en\">");
  reply = F("<head>");
  reply += F("<title>easyAPRS board - EA4GKQ</title>");
  reply += F("<meta charset=\"utf-8\">");
  reply += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  reply += F("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">");
  reply += F("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>");
  reply += F("<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>");
  reply += F("</head>");
  reply += F("<body>");
  
  reply += F("<div class=\"container\">");
  reply += F("<div class=\"table-responsive\">");
  
  reply += F(" <div class=\"panel panel-primary\">");
  reply += F(" <div class=\"panel-heading\">easyAPRS board - EA4GKQ</div>");
  reply += F("<div class=\"panel-body\">");


        
  reply += F("<table class=\"table table-condensed\">");
  reply += F("<thead>");
  reply += F("<tr>");
  reply += F("<th></th>");
  reply += F("<th></th>"); 
  reply += F("</tr>");
  reply += F("</thead>");
  reply += F("<tbody>");
  reply += F("<tr><td>Code version</td>     <td>beta 0.2</td></tr>");
  String satsVisible = String(gps.satellites.value());
  reply += F("<tr><td>Satelites </td><td> ");
  reply += (satsVisible);
  reply += F("</td></tr>");
  String connGoodString = String(connectionsGood);
  reply += F("<tr><td>Envios APRS.IS OK</td><td>");
  reply += (connGoodString);
  reply += F("</td></tr>");
  
  String connBadString = String(connectionFailsTotal);
  reply += F("<tr><td>Envios APRS.IS KO</td><td>");
  reply += (connBadString);
  reply += F("</td></tr>");
  
  
  reply += F("<tr><td>GPS TIME</td><td>"); 
  sprintf (buffer2, "%02d",gps.time.hour());
  reply += (buffer2);
  reply += F(":");
  sprintf (buffer2, "%02d",gps.time.minute());
  reply += (buffer2);
  reply += F("</td></tr>");
  reply += F("<tr><td>Posición</td><td>");
  String latitudeStr  = String(gps.location.lat());
  String longitudeStr = String(gps.location.lng());
  reply += latitudeStr;
  reply += F("  ,  ");
  reply += longitudeStr;
  reply += F("</td></tr>");
  
  reply += F("<tr><td>Ultima pos. enviada</td><td>");
  String latitudeStrEnviada  = String(lastLatSent);
  String longitudeStrEnviada = String(lastLongSent);
  reply += latitudeStrEnviada;
  reply += F("  ,  ");
  reply += longitudeStrEnviada;
  reply += F("</td></tr>");



  reply += F("<tr><td>Distancia</td><td>");
  String ultimaPosEnviada  = String(distanceKmToLastIP);
  
  reply += ultimaPosEnviada;
  
  reply += F(" mts</td></tr>");
                
 
  
  
  
  reply += F("</tbody>");
  reply += F("</table>");
  reply += F("<button type=\"button\" onclick=\"window.location.href='/settings'\" class=\"btn btn-primary btn-md\">Settings</button>");
  reply += F("</div>");
  reply += F("</div>");
  reply += F("</body>");
  reply += F("</html>");
    
  WebServer.send(200, "text/html", reply);
  
}


void handleSettings(){
  Serial.println("WebServer.hasArg(\"APRSid\")");
  Serial.println((WebServer.method() == HTTP_GET)?"GET":"POST");
  Serial.print("WebServer.args(): ");
  Serial.println(WebServer.args());
  String message;
 
  Serial.println(WebServer.hasArg("APRSid"));
if(WebServer.hasArg("APRSid")){
  //save settings!
  Serial.println("Grabando datos");
  
  String WifiSSID = WebServer.arg("WifiSSID");
  strncpy(appSettings.WifiSSID, WifiSSID.c_str(), sizeof(appSettings.WifiSSID));
  
  String WifiKey = WebServer.arg("WifiKey");
  strncpy(appSettings.WifiKey, WifiKey.c_str(), sizeof(appSettings.WifiKey)); 

  String WifiSSID2 = WebServer.arg("WifiSSID2");
  strncpy(appSettings.WifiSSID2, WifiSSID2.c_str(), sizeof(appSettings.WifiSSID2));
  
  String WifiKey2 = WebServer.arg("WifiKey2");
  strncpy(appSettings.WifiKey2, WifiKey2.c_str(), sizeof(appSettings.WifiKey2)); 
    

  String APRSid = WebServer.arg("APRSid");
  strncpy(appSettings.APRSid, APRSid.c_str(), sizeof(appSettings.APRSid));
  
  String APRSpass = WebServer.arg("APRSpass");
  strncpy(appSettings.APRSpass, APRSpass.c_str(), sizeof(appSettings.APRSpass)); 

  String Ipinterval = WebServer.arg("Ipinterval");
  strncpy(appSettings.Ipinterval, Ipinterval.c_str(), sizeof(appSettings.Ipinterval));   

  String distanceLow = WebServer.arg("distanceLow");
  strncpy(appSettings.distanceLow, distanceLow.c_str(), sizeof(appSettings.distanceLow));   

  String distanceHigh = WebServer.arg("distanceHigh");
  strncpy(appSettings.distanceHigh, distanceHigh.c_str(), sizeof(appSettings.distanceHigh));   
  
  String RFinterval = WebServer.arg("RFinterval");
  strncpy(appSettings.RFinterval, RFinterval.c_str(), sizeof(appSettings.RFinterval));   

  String APRSServer = WebServer.arg("APRSServer");
  strncpy(appSettings.APRSServer, APRSServer.c_str(), sizeof(appSettings.APRSServer));   

  String GPS = WebServer.arg("GPS");
  strncpy(appSettings.GPS, GPS.c_str(), sizeof(appSettings.GPS));     
 
  saveConfig();//save settings into EE
}else{
  loadConfig();//load settings from EE
}

  Serial.println("handleSettings");
  String reply = "";
  char buffer2 [2];
reply = F("<html>");
reply += F("<body>");

reply = F("<!DOCTYPE html>");
reply = F("<html lang=\"en\">");
reply = F("<head>");
  reply += F("<title>easyAPRS board - EA4GKQ</title>");
  reply += F("<meta charset=\"utf-8\">");
  reply += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  reply += F("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">");
  reply += F("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>");
  reply += F("<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>");
reply += F("</head>");
reply += F("<body>");
 

 

reply += F("<div class=\"container\">");
reply += F("<div class=\"table-responsive\">");

reply += F(" <div class=\"panel panel-primary\">");
reply += F(" <div class=\"panel-heading\">easyAPRS board - EA4GKQ</div>");
reply += F("<div class=\"panel-body\">");

reply += F("<table class=\"table table-condensed\">");
reply += F("<thead>");
reply += F("<tr>");
reply += F("<th>");
reply += F("</th>");
reply += F("<th>");
reply += F("</th>");
reply += F("</tr>");
reply += F("</thead>");
reply += F("<tbody>");
      
//reply += F("<form action=\"/settings\" id=\"settings\" method=\"POST\">");
reply += F("</p><form method='POST' action='settings'>");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>WifiSSID</label></td><td><input class=\"form-control\" name='WifiSSID' length=12 value=\"");
  reply += String(appSettings.WifiSSID) ;//
  reply += F("\" ></div></td></tr>");
reply += F("");

 
reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>WifiKey</label></td><td><input class=\"form-control\"name='WifiKey' length=12 value=\"");
  reply += String(appSettings.WifiKey) ;//
  reply += F("\" ></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>WifiSSID2</label></td><td><input class=\"form-control\" name='WifiSSID2' length=12 value=\"");
  reply += String(appSettings.WifiSSID2) ;//
  reply += F("\" ></div></td></tr>");
reply += F("");

 
reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>WifiKey2</label></td><td><input class=\"form-control\" name='WifiKey2' length=12 value=\"");
  reply += String(appSettings.WifiKey2) ;//
  reply += F("\" ></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>APRSid</label></td><td><input class=\"form-control\" name='APRSid' length=12 value=\"");
  reply += String(appSettings.APRSid) ;//EA4GKQ-6
  reply += F("\" maxlength=\"8\" placeholder=\"EA4RCM-6\"></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>APRSpass</label></td><td><input class=\"form-control\" name='APRSpass' length=12 value=\"");
  reply += appSettings.APRSpass ;//EA4GKQ-6
  reply += F("\" maxlength=\"5\" placeholder=\"12345\"></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>Ipinterval</label></td><td><input class=\"form-control\" type ='number' name='Ipinterval' length=12 value=\"");
  reply += appSettings.Ipinterval ;//EA4GKQ-6
  reply += F("\" maxlength=\"3\" placeholder=\"120\">&nbsp;&nbsp;seg</div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>Distancia</label></td><td><input class=\"form-control\" type ='number' name='distanceLow' length=12 value=\"");
  reply += appSettings.distanceLow ;//EA4GKQ-6
  reply += F("\" maxlength=\"4\" placeholder=\"1000\">&nbsp;&nbsp;mts speed<50Km/h</div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>Distancia</label></td><td><input class=\"form-control\" type ='number' name='distanceHigh' length=12 value=\"");
  reply += appSettings.distanceHigh ;//EA4GKQ-6
  reply += F("\" maxlength=\"4\" placeholder=\"1000\">&nbsp;&nbsp;mts speed>50Km/h</div></td></tr>");
reply += F("");
/*
reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>RFinterval</label></td><td><input class=\"form-control\" type ='number' name='RFinterval' length=12 value=\"");
  reply += appSettings.RFinterval ;//
  reply += F("\" maxlength=\"3\" placeholder=\"120\">&nbsp;&nbsp;seg</div></td></tr>");
reply += F("");
*/
reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>APRSServer</label></td><td><input class=\"form-control\" name='APRSServer' length=12 value=\"");
  reply += appSettings.APRSServer ;//
  reply += F("\" maxlength=\"32\" placeholder=\"rotate.aprs.net\"></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<label>GPS Speed</label></td><td><input class=\"form-control\" name='GPS' type ='number' length=12 value=\"");
  reply += appSettings.GPS ;//
  reply += F("\" maxlength=\"32\" placeholder=\"9600\"></div></td></tr>");
reply += F("");

reply += F("<tr><td><div class=\"form-group\">");
reply += F("<input class='btn btn-primary' type='submit'>&nbsp;&nbsp;");
reply += F("</td><td><button onclick=\"window.location.href='/'\" type='button' class='btn btn-primary'>HOME</button>&nbsp;&nbsp;");
reply += F("</td></tr><tr><td><button onclick=\"window.location.href='/reboot'\" type='button' class='btn btn-primary'>REBOOT</button></td><td></td></tr></form></div>");
 
reply += F("</form>");
reply += F("</div>");
reply += F("</div>");


reply += F("</tbody>");
reply += F("</table>");


reply += F("</body>");
reply += F("</html>");
  
  WebServer.send(200, "text/html", reply);
    
  }


  
void handleNotFound(){
 Serial.println("handleNotFound");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += WebServer.uri();
  message += "\nMethod: ";
  message += (WebServer.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += WebServer.args();
  message += "\n";
  for (uint8_t i=0; i<WebServer.args(); i++){
    message += " " + WebServer.argName(i) + ": " + WebServer.arg(i) + "\n";
  }
  WebServer.send(404, "text/plain", message);
 
}




void handleHome() {   
      if (!server.authenticate(www_username, www_password)) {
        return server.requestAuthentication();
      }   
      homepage();
}

void homepage() {
      String page = "<html><head><title>ActivMotion</title>";
      page += "<link rel='icon' href='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAS0lEQVR42s2SMQ4AIAjE+P+ncSYdasgNXMJgcyIIlVKPIKdvioAXyWBeJmVpqRZKWtj9QWAKZyWll50b8IcL9JUeQF50n28ckyb0ADG8RLwp05YBAAAAAElFTkSuQmCC' type='image/x-png' />";
      page += "<style type = \"text/css\">";
      page += ".footer-left {color:black;text-align:left;font-family:sans-serif;font-size: 24px;font-weight:350;}";
      page += ".header-right {text-align:right;font-family:sans-serif;font-size: 24px;font-weight:350;}"; 
      page += ".big-red {color:red;text-align:center;font-family:sans-serif;font-size: 48px;font-weight:700;}";
      page += ".big-black {color:black;text-align:center;font-family:sans-serif;font-size: 48px;font-weight:700;}";
      page += "</style><meta http-equiv=\"refresh\" content=\"10 url='/'\"></head><body style=\"background-color:lightgrey;\"><p class=\"header-right\">Motion AC</p>";
      
      if (!motionStateCurrent) 
            page += "<p class=\"big-black\">";
      else
            page += "<p class=\"big-red\">";
      if (!motionDetectionState){
        if (!motionStateCurrent) {
            page += txtpause;
        } else { 
            page += http901;
        } 
        page += "<br>&nbsp;<br> <a href=""/ON"">ON</a>";       
      } else {
        if (!motionStateCurrent){
            page += txtstart;
        } else {
            page += http999;
        }
        page += "<br>&nbsp;<br> <a href=""/OFF"">OFF</a>";
      }   
      page += "</p><p class=\"footer-left\">"+String(pirStateCurrent)+"-"+String(duration_initial)+":"+String(duration_current); 
      page += "</p></body></html>"; 
      server.send(200, "text/html", page);     
}


void handleON() {    
      if (!server.authenticate(www_username, www_password)) {
          return server.requestAuthentication();
        }

      // alarm system will be activated only if no current motion
      if (!motionStateCurrent) {
          motionDetectionState = true; 
          digitalWrite(ONOFF_LED_PIN, HIGH);    
          duration_current = get_duration();
          duration_initial = duration_current;
      }
      homepage();
}

void handleOFF() {    
      if (!server.authenticate(www_username, www_password)) {
          return server.requestAuthentication();
        } 
      motionDetectionState = false;
      duration_initial = duration_current;
      digitalWrite(ALERT_PIN_LOW, LOW);
      digitalWrite(ONOFF_LED_PIN, LOW);  
      digitalWrite(MOTION_LED_PIN, LOW);
      alertCounter = 0;
      homepage();
}

void handleNotFound() {
      String page = "File Not Found\n\n";
      page += "URI: ";
      page += server.uri();
      page += "\nMethod: ";
      page += (server.method() == HTTP_GET) ? "GET" : "POST";
      page += "\nArguments: ";
      page += server.args();
      page += "\n";
      for (uint8_t i = 0; i < server.args(); i++) {
        page += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", page);
}

void handleStart() {
      if (!server.authenticate(www_username, www_password)) {
            return server.requestAuthentication();
          }
      // alarm system will be activated only if no current motion
      if (motionStateCurrent) {
        server.send(901, "text/plain", http901);
        return;
      }
      motionDetectionState = true; 
      digitalWrite(ONOFF_LED_PIN, HIGH);  
      String page; 
      if (!motionDetectionState)
        page = txtpause;
      else {
        if (!motionStateCurrent)
            page = txtstart;
        else {
            server.send(999, "text/plain", http999);
            return;
        }
      }      
      server.send(200, "text/plain", page);
      duration_current = get_duration();
      duration_initial = duration_current;
}

void handlePause() {     
      if (!server.authenticate(www_username, www_password)) {
            return server.requestAuthentication();
          }
      motionDetectionState = false;
      duration_initial = duration_current;
      digitalWrite(ALERT_PIN_LOW, LOW);
      digitalWrite(ONOFF_LED_PIN, LOW);  
      digitalWrite(MOTION_LED_PIN, LOW);
      alertCounter = 0;
      String page; 
      if (!motionDetectionState)
        page = txtpause;
      else {
        if (!motionStateCurrent)
            page = txtstart;
        else {
            server.send(999, "text/plain", http999);
            return;
        }
      }      
      server.send(200, "text/plain", page);
}

void handleStatus() {
      if (!server.authenticate(www_username, www_password)) {
          return server.requestAuthentication();
        }       
      String page; 
      if (!motionDetectionState){
        page = txtpause;
      } else {
        if (!motionStateCurrent){
            page = txtstart;
        } else {
            server.send(999, "text/plain", http999);
            return;
        }
      }     
      server.send(200, "text/plain", page);
}

int get_duration() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  return pulseIn(ECHO_PIN, HIGH);
}

boolean get_ultrasonic_state(){
  boolean result = false;
  // 25% --> threshold  
  if (duration_initial>0) 
    result = (abs(100*duration_current/duration_initial)-100)>25; 
  return result;
}

boolean get_pir_state(){
  boolean result;
  pirStateCurrent = digitalRead(MOTION_SENSOR_PIN);
  if (pirStateCurrent==HIGH) 
    result = true;
  else 
    result = false;
  // only if BTE16-19  
  result = !result;  
  return result;
}

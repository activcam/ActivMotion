void loop(void) {
  
  server.handleClient();
    
  motionStatePrevious = motionStateCurrent;             // store old state
  duration_current = get_duration();
 

  if (get_pir_state() && get_ultrasonic_state())  motionStateCurrent=true;

  if (!get_pir_state() && motionStateCurrent) motionStateCurrent=false;

  if (!motionStatePrevious && motionStateCurrent && motionDetectionState) {

    digitalWrite(MOTION_LED_PIN, HIGH); // turn on
    if (alertCounter < maxMotionAlert) {
        digitalWrite(ALERT_PIN_LOW, HIGH); // turn on
    }                            
    alertCounter = alertCounter + 1;
  }
  else
  if (motionStatePrevious && !motionStateCurrent) { 
    digitalWrite(MOTION_LED_PIN, LOW);  // turn off
    digitalWrite(ALERT_PIN_LOW, LOW);  // turn off
  }

  delay(100);
  
}

void setup(void) {

  Serial.begin(115200);
  
  Serial.print("Initialize PINs");
  pinMode(ONOFF_LED_PIN, OUTPUT);

  pinMode(MOTION_LED_PIN, OUTPUT);
  pinMode(ALERT_PIN_LOW, OUTPUT);                
  pinMode(MOTION_SENSOR_PIN, INPUT);   
  
  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);     

  digitalWrite(ONOFF_LED_PIN, LOW);
  digitalWrite(MOTION_LED_PIN, LOW);
  digitalWrite(ALERT_PIN_LOW, LOW);

  // While in AP mode, connect to it 
  // then open a browser to the gateway IP, default 192.168.4.1 
  // https://github.com/tzapu/WiFiManager

  WiFiManager wifiManager;
  wifiManager.autoConnect();
      
  duration_current = get_duration();
  duration_initial = duration_current;

  if (duration_initial>0) 
        Serial.println(duration_initial);
    
  client->setInsecure();
  client->setTimeout(5000);
  
  server.getServer().setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));

  server.on("/0/detection/start", handleStart);
  server.on("/0/detection/pause", handlePause);
  server.on("/0/detection/status", handleStatus);
  
  server.on("/", handleHome);
  server.on("/ON", handleON);
  server.on("/OFF", handleOFF);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTPS server started");
    
}

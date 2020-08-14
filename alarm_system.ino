/*
  Alarm system  
*/

#include <UIPEthernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default):
EthernetServer server(12358);

const int ONOFF_LED_PIN     = 7;   // Arduino pin connected to the OUTPUT pin of on/off led
const int MOTION_LED_PIN    = 5;   // Arduino pin connected to the OUTPUT pin of motion led
const int MOTION_SENSOR_PIN = 8;   // Arduino pin connected to the OUTPUT pin of motion sensor
const int DOOR_SENSOR_PIN   = 6;   // Arduino pin connected to the door sensor
const int WARNING_PIN       = 4;   // Arduino pin connected to the IN pin of relay (warning flash)
const int ALERT_PIN         = 3;   // Arduino pin connected to the IN pin of relay (alarm siren)
int motionStateCurrent      = LOW; // current  state of motion sensor's pin
int motionStatePrevious     = LOW; // previous state of motion sensor's pin
int motionDetectionState    = LOW; // motion detection on (HIGH) /off (LOW)
int doorLockState           = HIGH;// door locked (LOW) / unlocked (HIGH) 
int alertCounter            = 0;   // count motion detection 
int maxMotionAlert          = 1;   // set alert counter limit

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  pinMode(ONOFF_LED_PIN, OUTPUT);
  pinMode(MOTION_LED_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);        // set arduino pin to input mode
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);   // set arduino pin to input_pullup mode
  pinMode(ALERT_PIN, OUTPUT);               // set arduino pin to output mode
  pinMode(WARNING_PIN, OUTPUT);             // set arduino pin to output mode

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String readString; 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
        } 
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          // client.println("Content-Type: text/plain");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 3");  // refresh the page automatically every x sec
          client.println();
          // client.println("<!DOCTYPE HTML>");
          client.print("<html><body style='display: flex; align-items: center; justify-content: center;'><h1 style='text-align: center; vertical-align:middle; font-size: 72px'>");
          
          if (motionDetectionState == LOW)
            if (motionStateCurrent == HIGH)
              client.print("alarm off - motion");
            else 
              if (doorLockState == HIGH)
                client.print("alarm off - lock door");
              else
                client.print("detection off");
          else
            if (motionStateCurrent == HIGH)
            {
                client.print("motion alert ");
                client.print(alertCounter);     
            }
            else
              if (doorLockState == HIGH) 
                client.print("door alert");
              else
                client.print("detection on");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      
    }

    if(readString.indexOf("/start") >0)//checks for on
    {
      // if motion is detected then do not activate detection 
      if (motionStateCurrent == LOW) {
        // if door is unlocked then do not activate detection 
        if (doorLockState == LOW) {
          motionDetectionState = HIGH;
          digitalWrite(ONOFF_LED_PIN, HIGH);    // set pin 5 high
        } 
      } 
    }
    
    if(readString.indexOf("/pause") >0)//checks for off
    {
      motionStatePrevious = motionStateCurrent; 
      motionStateCurrent      = LOW; 
      motionDetectionState    = LOW;
      alertCounter = 0;
      digitalWrite(ONOFF_LED_PIN, LOW);   // set pin 5 low
      digitalWrite(MOTION_LED_PIN, LOW);  // set pin 5 low
      digitalWrite(ALERT_PIN, LOW);       // turn off
      digitalWrite(WARNING_PIN, LOW);     // turn off
    }
    //clearing string for next read
    readString="";

    client.print("</h1></body></html>");

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    // Serial.println("client disconnected");
 
  }
    
  
  // ********************** ALARM START **************************
    
    motionStatePrevious = motionStateCurrent;             // store old state
    motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); // read new state
    doorLockState = digitalRead(DOOR_SENSOR_PIN);         
  
    // unauthorised entry ?
    if (motionStatePrevious == LOW && motionStateCurrent == HIGH && motionDetectionState == HIGH) { // pin state change: LOW -> HIGH
      digitalWrite(MOTION_LED_PIN, HIGH); // turn on
      if (alertCounter < maxMotionAlert) 
        digitalWrite(ALERT_PIN, HIGH); // turn on
      alertCounter = alertCounter + 1;
    }
    else
    if (motionStatePrevious == HIGH && motionStateCurrent == LOW) { // pin state change: HIGH -> LOW
      digitalWrite(ALERT_PIN, LOW);  // turn off
    }

    // unauthorised door unlock ?
    if (doorLockState == HIGH && motionDetectionState == HIGH)
      digitalWrite(WARNING_PIN, HIGH);  // turn on
    else
      digitalWrite(WARNING_PIN, LOW);   // turn off

    // ********************** ALARM ENDE ***************************
}

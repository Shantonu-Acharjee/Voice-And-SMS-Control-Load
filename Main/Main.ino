#include <SoftwareSerial.h>

String latestSMS = ""; // Variable to store the latest SMS message
String message = "";

// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx is connected to Arduino #3 & #2

// Define the pin for the LED
const int ledPinP = 8;
const int ledPinN = 9;
const int alarmPin = 11;



void setup() {
  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  // Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); // Handshake test
  updateSerial();

  mySerial.println("AT+CMGF=0"); // Delete all SMS (optional reset command)
  updateSerial();
  delay(1000);

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();

  mySerial.println("AT+CNMI=2,2,0,0,0"); // Forward incoming messages to the terminal
  updateSerial();


    // Initialize the LED pin as output
  pinMode(ledPinP, OUTPUT);
  pinMode(ledPinN, OUTPUT);
  pinMode(alarmPin, OUTPUT);


}

void loop() {
  if (mySerial.available()) {
    String response = mySerial.readString(); // Read the entire incoming message
    Serial.println("Full response: " + response); // Print the full response (for debugging)

    // Check if the response contains the SMS header and extract the actual message
    int indexOfMessage = response.indexOf("\n", response.indexOf("+CMT:"));
    
    if (indexOfMessage >= 0) {
      latestSMS = response.substring(indexOfMessage + 1); // Extract the actual SMS text
      latestSMS.trim(); // Remove any leading/trailing whitespace

      // If there is still metadata in the message, remove it
      int secondNewline = latestSMS.indexOf("\n");
      if (secondNewline >= 0) {
        latestSMS = latestSMS.substring(secondNewline + 1); // Get only the SMS content after the second newline
        latestSMS.trim(); // Remove any trailing whitespace again
      }

      Serial.println("Latest SMS: " + latestSMS); // Print only the latest SMS

      

      if(latestSMS == "Led on"){
        digitalWrite(ledPinP, HIGH); // Turn the LED on
        digitalWrite(ledPinN, LOW);
        Serial.println("LED ON");
      }

      else if(latestSMS == "Led off"){
        digitalWrite(ledPinP, LOW); // Turn the LED on
        digitalWrite(ledPinN, LOW);
        Serial.println("LED OFF");
      }

      else if(latestSMS == "Alarm on"){
        digitalWrite(alarmPin, HIGH);
        Serial.println("Alarm ON");
      }

      else if(latestSMS == "Alarm off"){
        digitalWrite(alarmPin, LOW);
        Serial.println("Alarm OFF");
      }

      else{
        digitalWrite(ledPinP, LOW);
        digitalWrite(ledPinN, LOW);
        digitalWrite(alarmPin, LOW);
      }

      // Delete all SMS after processing the new message
      mySerial.println("AT+CMGDA=\"DEL ALL\"");// or mySerial.println("AT+CMGF=0");
      updateSerial();  // Wait for confirmation from SIM800L that SMSs are deleted
      Serial.println("All SMS deleted.");

      
    }
  }











  // Check if there's any data available from the Serial (Bluetooth module)
  if (Serial.available()) {
    message = Serial.readString(); // Read the incoming message
    message.toLowerCase(); // Convert the message to lowercase
    Serial.println(message); // Print the message in lowercase
  }

  if(message == "led on"){
    digitalWrite(ledPinP, HIGH); // Turn the LED on
    digitalWrite(ledPinN, LOW);
    Serial.println("LED ON");
  }
  else if (message == "led off") {
    digitalWrite(ledPinP, LOW); // Turn the LED off
    digitalWrite(ledPinN, LOW);
    Serial.println("LED OFF");
  }

  else if(message == "alarm on"){
    digitalWrite(alarmPin, HIGH);
    Serial.println("Alarm ON");
  }

  else if(message == "alarm off"){
    digitalWrite(alarmPin, LOW);
    Serial.println("Alarm OFF");
  }


  message = "";

}












void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // Forward data from Serial to SIM800L
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward data from SIM800L to Serial
  }
}

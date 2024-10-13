#include <SoftwareSerial.h>

// Create software serial object to communicate with SIM800L
SoftwareSerial GSM_800L(3, 2); // SIM800L Tx & Rx is connected to Arduino #3 & #2

String sms; // Variable to store the SMS message

void setup() {
  Serial.begin(9600); // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  GSM_800L.begin(9600); // Begin serial communication with Arduino and SIM800L

  Serial.println("Initializing...");
  delay(1000);

  GSM_800L.println("AT"); // Once the handshake test is successful, it will back to OK
  updateSerial();

  GSM_800L.println("AT+CMGF=0"); //Delete all SMS
  updateSerial();
  delay(1000);

  GSM_800L.println("AT+CMGF=1"); // Set SMS to text mode
  updateSerial();

  GSM_800L.println("AT+CMGR=1"); // Read the SMS at index 1 (first message)
  updateSerial();

}

void loop() {

  updateSerial();

  // sms is the message veriable
}




void updateSerial() {
  delay(1000);

  while (GSM_800L.available()) {
    String response = GSM_800L.readString(); // Read the incoming message
    Serial.println(response); // Print the response to the Serial Monitor

    // Store the SMS message if it contains text
    if (response.startsWith("+CMGR")) { // Check if the response starts with the expected SMS format
      sms = response; // Store the response in the variable
      Serial.println(sms);

      GSM_800L.println("AT+CMGF=0"); //Delete all SMS
      updateSerial();
    }
  }
}

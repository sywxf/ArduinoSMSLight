/*
 SMS receiver

 This sketch, for the Arduino GSM shield, waits for a SMS message
 and displays it through the Serial port.

 Circuit:
 * GSM shield attached to and Arduino
 * SIM card that can receive SMS messages

 created 25 Feb 2012
 by Javier Zorzano / TD

 This example is in the public domain.

 http://www.arduino.cc/en/Tutorial/GSMExamplesReceiveSMS

*/

// include the GSM library
#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];

void setup()
{
  // initialize digital pin 9~12 as an output.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // turn the LED 1 on
  digitalWrite(9, HIGH);

  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("SMS Messages Receiver");
  // turn the LED 2 on
  digitalWrite(10, HIGH);

  // connection state
  boolean notConnected = true;

  // Start GSM connection
  while (notConnected)
  {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");

  // turn the LED 3,4 on
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  delay(1000);

  // turn the LEDs off
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop()
{
  char c;

  // If there are any SMSs available()
  if (sms.available())
  {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#')
    {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them
    while (c = sms.read()) {
      Serial.print(c);
      if('k1' == c) digitalWrite(9, HIGH);
      if('k2' == c) digitalWrite(10, HIGH);
      if('k3' == c) digitalWrite(11, HIGH);
      if('k4' == c) digitalWrite(12, HIGH);
      if('g1' == c) digitalWrite(9, LOW);
      if('g2' == c) digitalWrite(10, LOW);
      if('g3' == c) digitalWrite(11, LOW);
      if('g4' == c) digitalWrite(12, LOW);
    }

    Serial.println("\nEND OF MESSAGE");

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }

  delay(1000);

}



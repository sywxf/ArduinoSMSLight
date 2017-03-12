
#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial mySerial(7, 8);

String strCommand = "";
String strBuffer = "";
const String MASTERPHONE = "+8613504638667"; //your phone

void waitResponese()
{
  int count = 0;
  while (count++ < 1000)
  {
    if (mySerial.available()) {
      char data = mySerial.read();
      Serial.write(data);
      if (data == 'K') break;
      if (data == 'R') break;
    } else {
      delay(10);
    }
  }
}

void sendATCommand(String cmd)
{
  mySerial.println(cmd);

  waitResponese();
}

void powerUpOrDown()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(3000);
}

void configModem() 
{
  powerUpOrDown();
  sendATCommand("AT+CFUN=1");
  sendATCommand("AT+CMEE=1");
//  delay(10000);  
  sendATCommand("AT+CMGF=1");
  sendATCommand("AT+CSMS=1");
  sendATCommand("AT+CNMI=3,3,0,0,0");
}

void setup()
{
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  Serial.begin(19200);
  mySerial.begin(19200);
  
  configModem();
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void loop()
{
  if (Serial.available()) {
//    mySerial.write(Serial.read());

    switch (Serial.read())
    {
      case 'l':
        sendATCommand("AT+CMGL=\"ALL\"");
        break;
      case 'r':
        ReadTextMessage();
        break;
      case 't':
        SendTextMessage();
        break;
      case 'x':
        sendATCommand("AT+CMGD=1,4");
        break;
      case 'd':
        DialVoiceCall();
        break;
      case 's':
        sendATCommand("AT+CSQ");
        break;
      case 'b':
        sendATCommand("AT+CFUN=1");
        break;
      case 'o':
        sendATCommand("AT+COPS=?");
        break;
      case 'p':
        sendATCommand("AT+CPOWD=1");
        break;
      case 'k':
        sendATCommand("AT+CMGR=1");
        break;
      case 'g':
        sendATCommand("AT+CMGR=2");
        break;
    }
  }

  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
    if(c == '+') {
      strBuffer = mySerial.readString();
      Serial.println(strBuffer+"==");
      if(strBuffer.startsWith("CMTI:")) {
        String index = strBuffer.substring(11,12);
        Serial.println("Found SM: "+index);
        sendATCommand("AT+CMGR="+index);
      } else if (strBuffer.startsWith("CMGR:")) {
        if (strBuffer.substring(20, 34) == MASTERPHONE) { // if your phone 
          int index = strBuffer.indexOf('\n');
          Serial.println("Found New line at col: "+index);
          strCommand=strBuffer.substring(index+1, index+3);
          strCommand.toLowerCase();
          handleCommand();  //run command
        }
      } else {
//        Serial.println("Not Match");
      }
    }
  }
}

void handleCommand()
{
  if(strCommand != "") {
    Serial.println("Command:"+strCommand);
    if(strCommand == "k1") digitalWrite(10, HIGH);
    if(strCommand == "k2") digitalWrite(11, HIGH);
    if(strCommand == "k3") digitalWrite(12, HIGH);
    if(strCommand == "k4") digitalWrite(13, HIGH);
    if(strCommand == "g1") digitalWrite(10, LOW);
    if(strCommand == "g2") digitalWrite(11, LOW);
    if(strCommand == "g3") digitalWrite(12, LOW);
    if(strCommand == "g4") digitalWrite(13, LOW);
    strCommand = "";
    sendATCommand("AT+CMGD=1,4");
    
  }
}
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.print("AT+CMGF=1\r");
  delay(1000);
  mySerial.print("AT+CMGS=\"+8610010\"\r");
  delay(1000);
  mySerial.print("102\r");
  delay(100);
  mySerial.print((char)26);
  delay(1000);
//  mySerial.println();
}

void ReadTextMessage()
{
  sendATCommand("AT+CMGR=1");
}

///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
//  mySerial.println("ATD+8613357828347;");
  delay(100);
  mySerial.println();
}

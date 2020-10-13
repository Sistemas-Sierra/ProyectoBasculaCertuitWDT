/*
 Name:		ProyectoBasculaCertuitWDT.ino
 Created:	10/13/2020 9:12:47 AM
 Author:	sierr
*/

#include <SoftwareSerial.h>
#include <avr/wdt.h>

SoftwareSerial mySerialSend(8, 9); // RX, TX   Port 0
SoftwareSerial mySerialReceive(10, 11); // RX, TX   Port 1

String weigth;
String uWeigth;
String Units;
int intCC = 0;
String tmpSt;
String Un;

boolean newData = false;
const byte numChars = 71;
char receivedChars[numChars];
const int ESC = 27;
const int h1D = 29;

const int sendPort = 0;
const int recvPort = 1;

// the setup function runs once when you press reset or power the board
void setup() {

 wdt_disable();

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  mySerialSend.begin(9600);
  mySerialReceive.begin(9600);

  resetPrn(sendPort);

  printInitMsg();

  Serial.println("Ready!!");
  Serial.print("Weigth:");
  Serial.println("99999");
  Serial.print(" U / W:");
  Serial.println("99999");
  Serial.print(" Units:");
  Serial.println("99999");

  intCC = 0;

  digitalWrite(LED_BUILTIN, LOW);

  wdt_enable(WDTO_4S);

}

// the loop function runs over and over again until power down or reset
void loop() {

  wdt_reset();
  recvData();
  printData();

}

void recvData() {

  static byte ndx = 0;
  char endMarker = '\r';
  char rc;

  mySerialReceive.listen();
  while (mySerialReceive.available() > 0 && newData == false) {
    rc = mySerialReceive.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void printData() {

  if (newData == true) {
    digitalWrite(LED_BUILTIN, HIGH);
    String sT = receivedChars;
    dataFill(sT);
    newData = false;
    intCC = intCC + 1;
    if (intCC == 3) {
      intCC = 0;
      printTicket();
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void printTicket() {

  resetPrn(sendPort);
  lines(2, sendPort);
  left(sendPort);
  type(32, sendPort);
  printS("Weigth:", sendPort);
  printlnS(weigth, sendPort);
  printS(" U / W:", sendPort);
  printlnS(uWeigth, sendPort);
  printS(" Units:", sendPort);
  printlnS(Units, sendPort);
  barcode(Un, sendPort);
  lines(3, sendPort);
  cut(sendPort);

  Serial.print("Weigth:");
  Serial.println(weigth);
  Serial.print(" U / W:");
  Serial.println(uWeigth);
  Serial.print(" Units:");
  Serial.println(Units);
}

void printInitMsg() {

  lines(1, sendPort);
  type(48, sendPort);
  printlnS("Ready!!", sendPort);
  printlnS("Print!!", sendPort);
  barcode("999999", sendPort);
  lines(1, sendPort);
  cut(sendPort);

}
void dataFill(String sT) {
  String tmpSt = getValue(sT, ':', 1);
  if (sT.startsWith("W")) {
    weigth = tmpSt;
  }
  if (sT.startsWith("U")) {
    uWeigth = tmpSt;
  }
  if (sT.startsWith("C")) {
    Units = tmpSt;
    Un = Units.substring(1, 7);
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Impresor ESC/POS

void resetPrn(int P)
{
  byte message[] = { ESC, 64 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void type(int tp, int P)
{
  byte message[] = { ESC, 33, tp };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void center(int P)
{
  byte message[] = { ESC, 97, 49 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void left(int P)
{
  byte message[] = { ESC, 97, 48 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void rigth(int P)
{
  byte message[] = { ESC, 97, 50 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void lines(int lin, int P)
{
  byte message[] = { ESC, 100, lin };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void cut(int P)
{
  lines(1, P);
  byte message[] = { h1D, 86, 49 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void bold(int bld, int P)
{
  byte message[] = { ESC, 69, bld };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void underline(int udr, int P)
{
  byte message[] = { ESC, 45, udr };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}
void cash(int P)
{
  byte message[] = { ESC, 112, 0, 25, 250 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      break;
  }
}

void barcode(String data, int P) {
  // lines(1, P);
  byte message[] = { h1D, 104, 80, h1D, 119, 3, h1D, 72, 2 , h1D, 107, 73 };
  switch (P)
  {
    case 0:
      mySerialSend.write(message, sizeof(message));
      mySerialSend.write(data.length());
      printS(data, P);
      break;
    case 1:
      mySerialReceive.write(message, sizeof(message));
      mySerialReceive.write(data.length());
      printS(data, P);
      break;
  }

  // lines(1, P);
}

void printlnS(String st, int P) {
  switch (P)
  {
    case 0:
      mySerialSend.println(st);
      break;
    case 1:
      mySerialReceive.println(st);
      break;
  }
}
void printS(String st, int P) {
  switch (P)
  {
    case 0:
      mySerialSend.print(st);
      break;
    case 1:
      mySerialReceive.print(st);
      break;
  }
}

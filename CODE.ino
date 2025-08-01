#include <SoftwareSerial.h>

#include <VoiceRecognitionV3.h>

VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record

uint8_t buf[64];

int led = 12;

int led1 = 11;

int led2 = 10;

int led3 = 9;

int buzzer = 8;

#define White (0)

#define Red (1)

#define Green (2)

#define Blue (3)

#define Off (4)

#define Blink (5)

#define detonate (6)

void printSignature(uint8_t *buf, int len) {

  int i;

  for(i=0; i<len; i++){

    if(buf[i]>0x19 && buf[i]<0x7F){

      Serial.write(buf[i]);

    }

    else {

      Serial.print("[");

      Serial.print(buf[i], HEX);

      Serial.print("]");

    }

  }

}

void printVR(uint8_t *buf) {

  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);

  Serial.print("\t\t");

  if(buf[0] == 0xFF){

    Serial.print("LED");

  }

  else if(buf[0]&0x80){

    Serial.print("UG ");

    Serial.print(buf[0]&(~0x80), DEC);

  }

  else {

    Serial.print("SG ");

    Serial.print(buf[0], DEC);

  }

  Serial.print("\t");

  Serial.print(buf[1], DEC);

  Serial.print("\t\t");

  if(buf[3]>0){

    printSignature(buf+4, buf[3]);

  }

  else {

    Serial.print("NONE");

  }

  Serial.println("\r\n");

}

void setup() {

  myVR.begin(9600);

  Serial.begin(115200);

  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");

  pinMode(led, OUTPUT);

  pinMode(led1, OUTPUT);

  pinMode(led2, OUTPUT);

  pinMode(led3, OUTPUT);

  pinMode(buzzer, OUTPUT);

  if (myVR.clear() == 0) {

    Serial.println("Recognizer cleared.");

  }

  else {

    Serial.println("Not find VoiceRecognitionModule.");

    Serial.println("Please check connection and restart Arduino.");

    while (1);

  }

  if (myVR.load((uint8_t)White) >= 0) {

    Serial.println("onRecord loaded");

  }

  if (myVR.load((uint8_t)Red) >= 0) {

    Serial.println("onRecord1 loaded");

  }

  if (myVR.load((uint8_t)Green) >= 0) {

    Serial.println("onRecord2 loaded");

  }

  if (myVR.load((uint8_t)Blue) >= 0) {

    Serial.println("onRecord3 loaded");

  }

  if (myVR.load((uint8_t)Off) >= 0) {

    Serial.println("offRecord loaded");

  }

  if (myVR.load((uint8_t)Blink) >= 0) {

    Serial.println("blink loaded");

  }

  if (myVR.load((uint8_t)detonate) >= 0) {

    Serial.println("buzzer loaded");

  }

}

void loop() {

  int ret;

  ret = myVR.recognize(buf, 50);

  if (ret > 0) {

    switch (buf[1]) {

      case White:

        digitalWrite(led, HIGH);

        break;

      case Red:

        digitalWrite(led1, HIGH);

        break;

      case Green:

        digitalWrite(led2, HIGH);

        break;

      case Blue:

        digitalWrite(led3, HIGH);

        break;

        case Off:

        digitalWrite(led, LOW);

        digitalWrite(led1, LOW);

        digitalWrite(led2, LOW);

        digitalWrite(led3, LOW);

        break;

      case detonate:

        for (int delayTime = 500; delayTime >= 10; delayTime -=10 ) {

    digitalWrite(buzzer, HIGH);

    delay(100);

    digitalWrite(buzzer, LOW);

    delay(delayTime);

  }

   digitalWrite(buzzer, HIGH);

    digitalWrite(led, HIGH);

  digitalWrite(led1, HIGH);

  digitalWrite(led2, HIGH);

  digitalWrite(led3, HIGH);

    delay(2000);

    digitalWrite(buzzer, LOW);

    digitalWrite(led, LOW);

  digitalWrite(led1, LOW);

  digitalWrite(led2, LOW);

  digitalWrite(led3, LOW);

     break;

       case Blink:

        digitalWrite(led, HIGH);

       delay (100);

       digitalWrite(led, LOW);

       delay (100);

       int n=5;

       for(int i=0;i<=n;i++){

       digitalWrite(led1, HIGH);

       delay (100);

       digitalWrite(led1, LOW);

       delay (100);

       digitalWrite(led2, HIGH);

       delay (100);

       digitalWrite(led2, LOW);

       delay (100);

       digitalWrite(led3, HIGH);

       delay (100);

       digitalWrite(led3, LOW);

       digitalWrite(led2, HIGH);

       delay (100);

       digitalWrite(led2, LOW);

       delay (100);

       digitalWrite(led1, HIGH);

       delay (100);

       digitalWrite(led1, LOW);

       delay (100);

       digitalWrite(led, HIGH);

       delay (100);

       digitalWrite(led, LOW);

       }

        break;

      default:

        Serial.println("Record function undefined");

        break;

    }

    printVR(buf);

  }

}

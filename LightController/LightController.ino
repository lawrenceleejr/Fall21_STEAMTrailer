// Arduino Serial Example #1 Remote Control Blink - Slave


//#include <SoftwareSerial.h>
#include <ReceiveOnlySoftwareSerial.h>

// software serial #1: RX = digital pin 10, TX = digital pin 11

//SoftwareSerial portOne(10, 11);

#define NSERIAL 7

//ReceiveOnlySoftwareSerial serialPorts[NSERIAL];

ReceiveOnlySoftwareSerial serialPorts[NSERIAL] = {
  ReceiveOnlySoftwareSerial(8),
  ReceiveOnlySoftwareSerial(9),
  ReceiveOnlySoftwareSerial(10),
  ReceiveOnlySoftwareSerial(11),
  ReceiveOnlySoftwareSerial(14),
  ReceiveOnlySoftwareSerial(15),
  ReceiveOnlySoftwareSerial(16)
};

//serialPorts[0] = ReceiveOnlySoftwareSerial(10);
//ReceiveOnlySoftwareSerial serial1(10);

char c  = ' ';
byte LED = 13;

char value[NSERIAL];

unsigned long startedWaiting = millis();

void setup() 
{
   pinMode(LED, OUTPUT);
//   serial1.begin(9600);




  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }

  for(int i=0; i<NSERIAL; i++)  
  {
    Serial.print("fft");
    Serial.print(",");
  }
  Serial.print("-1");
  Serial.println();


   
  for (int i=0; i<NSERIAL; i++){
    serialPorts[i].begin(9600);
  }
}
 
 
void loop()
{
  for (int i=0; i<NSERIAL; i++){
//    value[i]=0;
    serialPorts[i].listen();
    startedWaiting = millis();
    while(!serialPorts[i].available() && millis() - startedWaiting <= 500){}
     if(serialPorts[i].available())
     {
        char c = serialPorts[i].read();
        if(float(c)>0) value[i] = c;
        else value[i]=float(0);
//        if (c=='1') { digitalWrite(LED, HIGH); delay(10); digitalWrite(LED, LOW); }
     } else {
      value[i] = float(value[i])-1;
      if(value[i]<0) value[i]=0;
     }
  }

  
  for(int i=0; i<NSERIAL; i++)  
  {
    Serial.print(float(value[i]));
    Serial.print(",");
  }
  Serial.print("-1");
  Serial.println();

 
}

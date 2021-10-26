// *** Light Controller for UTK Fall 21 STEAM Trailer ***
//
// This arduino receives an 8-bit char on a collection
// of digital input serial lines. Each line corresponds
// to a specific frequency band power. Then each number
// can be used to tell the NeoPixel LEDs to light up.

#include <ReceiveOnlySoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define NSERIAL 7

ReceiveOnlySoftwareSerial serialPorts[NSERIAL] = {
  ReceiveOnlySoftwareSerial(8),
  ReceiveOnlySoftwareSerial(9),
  ReceiveOnlySoftwareSerial(10),
  ReceiveOnlySoftwareSerial(11),
  ReceiveOnlySoftwareSerial(14),
  ReceiveOnlySoftwareSerial(15),
  ReceiveOnlySoftwareSerial(16)
};

Adafruit_NeoPixel lights[NSERIAL] = {
  Adafruit_NeoPixel(50,2,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,3,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,4,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,5,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,6,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,7,NEO_GRB+ NEO_KHZ800),
  Adafruit_NeoPixel(50,12,NEO_GRB+ NEO_KHZ800)
};

uint32_t colors[NSERIAL] = {
  lights[0].Color(249,65,68),
  lights[0].Color(243,114,44),
  lights[0].Color(248,150,30),
  lights[0].Color(249,199,79),
  lights[0].Color(144,190,109),
  lights[0].Color(67,170,139),
  lights[0].Color(87,117,144),
};

char c  = ' ';
byte LED = 13;

char value[NSERIAL];

unsigned long startedWaiting = millis();

void setup()
{

  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Native USB only
  //  }

  // Initialize each LED strip. Turn them on one at a time.
  for (int i=0; i<NSERIAL; i++){
    Serial.print("fft");
    Serial.print(",");

    serialPorts[i].begin(9600);

    lights[i].begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    lights[i].show();            // Turn OFF all pixels ASAP

    colors[i] = lights[i].gamma32(colors[i]);
    lights[i].fill(colors[i],0,50);
    lights[i].show();
    delay(50);
  }

  Serial.print("-1");
  Serial.println();

  // Blink the on-board LED to signify that the setup function is over.
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  // Sleep for 1000ms = 1s
  delay(1000);
  // Turn off all LEDs.
  for (int i=0;i<NSERIAL; i++){
    lights[i].setBrightness(0);
    lights[i].show();
  }
  // Double blink on-board LED again
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);

}


void loop()
{
  // Loop over serial connections and read them in turn.
  // Slow because software serial port can only be listened
  // to one at a time.
  for (int i=0; i<NSERIAL; i++){
    serialPorts[i].listen();
    startedWaiting = millis();
    while(!serialPorts[i].available() && millis() - startedWaiting <= 500){}
     if(serialPorts[i].available())
     {
        char c = serialPorts[i].read();
        if(float(c)>0) value[i] = c;
        else value[i]=float(0);
     } else {
      value[i] = float(value[i])-1;
      if(value[i]<0) value[i]=0;
     }
  }

  // By here, I've read in a new value for all the lights.
  // Let's write those values as brightnesses to the LEDs
  for(int i=0; i<NSERIAL; i++)
  {
    int tempValue = 2*(float)value[i];
    Serial.print(tempValue);
    Serial.print(",");
    lights[i].setBrightness(tempValue);
    lights[i].show();
  }
  Serial.print("-1");
  Serial.println();

}

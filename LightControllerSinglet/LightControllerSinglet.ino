// *** Light Controller (singlet) for UTK Fall 21 STEAM Trailer ***
//
// This arduino receives an 8-bit char on ONE digital input serial lines.
// This line corresponds to a specific frequency band power.
// This number can be used to tell the NeoPixel LEDs to light up.

#include <Adafruit_NeoPixel.h>

#define NSERIAL 1

Adafruit_NeoPixel lights[NSERIAL] = {
  Adafruit_NeoPixel(50,2,NEO_GRB+ NEO_KHZ800)
};

uint32_t colors[7] = {
  lights[0].Color(249,65,68),
  lights[0].Color(243,114,44),
  lights[0].Color(248,150,30),
  lights[0].Color(249,199,79),
  lights[0].Color(144,190,109),
  lights[0].Color(67,170,139),
  lights[0].Color(87,117,144),
};

uint32_t chosenColor = 0;

char c  = ' ';
byte LED = 13;

char value[NSERIAL];

unsigned long startedWaiting = millis();

void setup()
{

  pinMode(LED, OUTPUT);

  // Color configuration pins
  // Connect one of these bins to ground to select its color
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);

  for (int i=0; i<7; i++){
    if( digitalRead(i+3) ){chosenColor=i; break;}
  }

  // This refers to the USB connection
  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Native USB only
  //  }

  // This refers to the pin 0/1 serial connection
  Serial1.begin(9600);

  // Initialize each LED strip. Turn them on one at a time.
  Serial.println(chosenColor);
  Serial.print("fft");
  Serial.print(",");

  lights[0].begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  lights[0].show();            // Turn OFF all pixels ASAP

  colors[chosenColor] = lights[0].gamma32(colors[chosenColor]);
  lights[0].fill(colors[chosenColor],0,50);
  lights[0].show();
  delay(50);

  Serial.print("-1");
  Serial.println();

  // Blink the on-board LED to signify that the setup function is over.
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);

  // Sleep for 1000ms = 1s
  delay(1000);

  // Turn off all LEDs.
  lights[0].setBrightness(0);
  lights[0].show();

  // Double blink on-board LED again
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);

}


void loop()
{
  startedWaiting = millis();
  while(!Serial1.available() && millis() - startedWaiting <= 500){}

  if (Serial1.available()){
    char c = Serial1.read();
    if(float(c)>0) value[0] = c;
    else value[0]=float(0);
  } else {
    value[0] = float(value[0])-1;
    if(value[0]<0) value[0]=0;
  }

  // By here, I've read in a new value for the light.
  // Let's write those values as brightnesses to the LEDs
  int tempValue = 2*(float)value[0];
  Serial.print(tempValue);
  Serial.print(",");
  lights[0].setBrightness(tempValue);
  lights[0].show();

  Serial.print("-1");
  Serial.println();

}

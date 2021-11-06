// *** Light Controller (singlet) for UTK Fall 21 STEAM Trailer ***
//
// This arduino receives an 8-bit char on ONE digital input serial lines.
// This line corresponds to a specific frequency band power.
// This number can be used to tell the NeoPixel LEDs to light up.

#include <Adafruit_NeoPixel.h>

#define NSERIAL 1

Adafruit_NeoPixel lights[NSERIAL] = {
  Adafruit_NeoPixel(300,2,NEO_GRB+ NEO_KHZ800)
};

#define WHICHCOLOR 6

uint32_t colors[7] = {
  lights[0].Color(249,65,68),
  lights[0].Color(243,114,44),
  lights[0].Color(248,150,30),
  lights[0].Color(249,199,79),
  lights[0].Color(144,190,109),
  lights[0].Color(67,170,139),
  lights[0].Color(87,117,144),
};

uint32_t chosenColor = WHICHCOLOR;

char c  = ' ';
byte LED = 13;

char value[NSERIAL];

unsigned long startedWaiting = millis();

void setup()
{
  // This refers to the USB connection
  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Native USB only
  //  }
  delay(100);

  // This refers to the pin 0/1 serial connection
  Serial1.begin(9600);

  delay(100);

  pinMode(LED, OUTPUT);

  pinMode(12, OUTPUT);
  digitalWrite(12,HIGH);

  // Color configuration pins
  // Connect one of these pins to pin 12 to select its color
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);

  for (int i=0; i<7; i++){
    Serial.println(digitalRead(i+3) );
    if( digitalRead(i+3)==HIGH ){chosenColor=i; break;}
  }
  Serial.println(WHICHCOLOR);

  // Initialize each LED strip. Turn them on one at a time.
  Serial.print("fft");
  Serial.print(",");

  lights[0].begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  lights[0].show();            // Turn OFF all pixels ASAP

  colors[WHICHCOLOR] = lights[0].gamma32(colors[WHICHCOLOR]);
  lights[0].fill(colors[WHICHCOLOR],0,300);
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
//  lights[0].setBrightness(0);
  lights[0].fill(0,0,300);

  lights[0].show();

  // Double blink on-board LED again
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);
  digitalWrite(LED, HIGH); delay(100); digitalWrite(LED, LOW);delay(50);

}


void loop()
{

//  for (int i=0; i<7; i++){
//    Serial.println(digitalRead(i+3) );
//    if( digitalRead(i+3)==HIGH ){chosenColor=i; break;}
//  }

  startedWaiting = millis();
  while(!Serial1.available() && millis() - startedWaiting <= 500){}

  if (Serial1.available()){
    char c = Serial1.read();
    if(float(c)>0) {
      value[0] = c;
      digitalWrite(LED, HIGH);
    }
    else{
      value[0]=float(0);
      digitalWrite(LED, LOW);
    }
  } else {
    value[0] = float(value[0])-1;
    if(value[0]<0) value[0]=0;
  }

  // By here, I've read in a new value for the light.
  // Let's write those values as brightnesses to the LEDs
  int tempValue = 2*(float)value[0];
  Serial.print(tempValue);
  Serial.print(",");
  lights[0].fill(colors[WHICHCOLOR],0,300);
  lights[0].setBrightness(tempValue);
  lights[0].show();

  Serial.print("-1");
  Serial.println();

}

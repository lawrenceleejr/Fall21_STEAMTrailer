#include "fix_fft.h"
//#include <arduinoFFT.h>

#include <SendOnlySoftwareSerial.h>
//#include <ReceiveOnlySoftwareSerial.h>
// #define EN 6
// #define RW 5
// #define CS 4


#define SAMPLES 256

#define AUDIO A0
#define XRES 64
#define THRESH 3

//SendOnlySoftwareSerial mySerialSend(3);  // Tx pin
//ReceiveOnlySoftwareSerial mySerialRecv(8);

char text[20] ;
char charVal[6];

byte greenLED = 12;

char im[SAMPLES];
char data[SAMPLES];
float barht[SAMPLES];
float output[XRES];
//int data_avgs[8];

//arduinoFFT FFT = arduinoFFT();                                    // FFT object

#define NSERIAL 7

//#include <SoftwareSerial.h>
//SendOnlySoftwareSerial serial1(2); // TX
//SendOnlySoftwareSerial serial2(3); //  TX
//SendOnlySoftwareSerial serial3(4); //  TX
//SendOnlySoftwareSerial serial4(5); //  TX
SendOnlySoftwareSerial serialPorts[NSERIAL] = {
  SendOnlySoftwareSerial(2),
  SendOnlySoftwareSerial(3),
  SendOnlySoftwareSerial(4),
  SendOnlySoftwareSerial(5),
  SendOnlySoftwareSerial(6),
  SendOnlySoftwareSerial(7),
  SendOnlySoftwareSerial(8)
};



void setup()
{
      
//    ADCSRA = 0b11100101;      // set ADC to free running mode and set pre-scalar to 32 (0xe5)
//    ADMUX = 0b00000000;       // use pin A0 and external voltage reference
  
//  mySerialSend.begin(115200); 
//  mySerialRecv.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//  Serial.println("fft0, fft1, fft2, fft3, fft4, fft5, fft6, data0");

  for(int i=0; i<XRES; i++)  
  {
    Serial.print("fft");
    Serial.print(",");
  }
  Serial.print("-1");
//  Serial.print(analogRead(AUDIO));
  Serial.println();



  for (int i=0; i<NSERIAL; i++){
    serialPorts[i].begin(9600);
  }

}

void loop()
{

  
//  portOne.listen();

  
  static int i, j;
  int val;

  // get audio data
  for(i = 0; i < SAMPLES; i++)
  {
    val = analogRead(AUDIO); // 0-1023
    data[i] = (char)(val/4 - 128); // store as char
    im[i] = 0; // init all as 0
  }

  // run FFT
  fix_fft(data, im, 7, 0);

  // extract absolute value of data only, for 64 results
  for(i = 1; i < SAMPLES/2; i++)
  {
    barht[i] = (float)sqrt(data[i] * data[i] + im[i] * im[i]);
    barht[i] = (2-barht[i]/60)*barht[i];
  }

//  int total = 0;
  //for(i = 0, j = 0; i < SAMPLES/2; i++, j += 2)
  //{
  //  barht[i] = barht[j] + barht[j + 1];
  //}


    // ++ re-arrange FFT result to match with no. of columns on display ( xres )
    
//    int step = (SAMPLES/2)/64; 
//    int c=0;
//    for(int i=0; i<(SAMPLES/2); i+=step)  
//    {
//      data_avgs[c] = 0;
//      for (int k=0 ; k< step ; k++) {
//          data_avgs[c] = data_avgs[c] + barht[i+k];
//      }
//      data_avgs[c] = data_avgs[c]/step; 
//      c++;
//    }
    // -- re-arrange FFT result to match with no. of columns on display ( xres )

    
//  for(int i=1;

  for(int i=0; i<XRES; i++) output[i] = 0 ;

  output[0] = mergeBins(1,2,barht);
  output[1] = mergeBins(2,4,barht);
  output[2] = mergeBins(5,10,barht);
  output[3] = mergeBins(11,21,barht);
  output[4] = mergeBins(22,32,barht);
  output[5] = mergeBins(33,50,barht);
  output[6] = mergeBins(51,64,barht);

//  Serial.println(99);

  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0; i<NSERIAL; i++){
//    serialPorts[i].write(100);
    serialPorts[i].write(output[i]);
    Serial.print(output[i]);
    Serial.print(",");
//    delay(2);
  }
//  Serial.println((float(val)/16 - 32));
  Serial.println(val);
  digitalWrite(LED_BUILTIN, LOW);
  
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(2);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(800);
}

int mergeBins(int start, int end, float barht[]){
  float output = 0;
  for(int i=start; i<end+1; i++){
    if(barht[i]>THRESH) output+=barht[i];
  }
//  output /= float(end-start);
  return output;
}

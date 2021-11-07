// *** FFT Spectrum Analyzer for UTK Fall 21 STEAM Trailer ***
//
// This arduino receives a line-level audio signal on A0
// It then performs an FFT, and outputs the strength
// for each frequency band on an individual soft serial
// port as an 8-bit char. This is currently programmed
// to send seven numbers out.
//
// Other arduinos should receive these serial signals.
// And then interpret these numbers to then send signals
// to the LED strips.


#include "fix_fft.h"

#include <SendOnlySoftwareSerial.h>

#define SAMPLES 256

#define AUDIO A0 //This is where the input pin is defined
#define XRES 64
#define THRESH 5
#define OUTPUTTHRESH 20
#define NOUTPUTS 7 // This is the number of output frequency bands

#define SMOOTH 0

char im[SAMPLES];
char data[SAMPLES];
float barht[SAMPLES];
float output[NOUTPUTS];

float outputHistory[NOUTPUTS][SMOOTH+1];
float averageOutput[NOUTPUTS];


SendOnlySoftwareSerial serialPorts[NOUTPUTS] = {
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

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Native USB only
  //  }

  for(int i=0; i<XRES; i++)
  {
    Serial.print("fft");
    Serial.print(",");
  }
  Serial.print("audio");
  Serial.println();

  for (int i=0; i<NOUTPUTS; i++){
    serialPorts[i].begin(9600);
  }
}

void loop()
{

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

  for(int i=0; i<XRES; i++) output[i] = 0 ;

  // This is where we might want to change how the frequency bins
  // are merged.

  output[0] = mergeBins(1,2,barht);
  output[1] = mergeBins(3,4,barht);
  output[2] = mergeBins(5,9,barht);
  output[3] = mergeBins(10,17,barht);
  output[4] = mergeBins(18,32,barht);
  output[5] = mergeBins(33,59,barht);
  output[6] = mergeBins(60,64,barht);

  if(SMOOTH>0){
    for(int iOut; iOut<NOUTPUTS; iOut++){
      averageOutput[iOut] = 0;
      for(int iHistory=0; iHistory<SMOOTH-1; i++){
        outputHistory[iOut][SMOOTH-iHistory-1] = outputHistory[iOut][SMOOTH-iHistory-2];
        averageOutput[iOut] += outputHistory[iOut][SMOOTH-iHistory-1];
      }
      outputHistory[iOut][0] = output[iOut];
      averageOutput[iOut] += output[iOut];
      averageOutput[iOut] /= (SMOOTH+1);
      output[iOut] = averageOutput[iOut];
    }
  }


  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0; i<NOUTPUTS; i++){
    
    if(output[i]<OUTPUTTHRESH) output[i]=0;
    if(output[i]-OUTPUTTHRESH>0) output[i] = 100*log10(output[i]-OUTPUTTHRESH);
    else output[i]=0;
    if(output[i]<0) output[i]=0;
    serialPorts[i].write(output[i]);
    Serial.print(output[i]);
    Serial.print(",");
  }
  Serial.println(val/100.);
  digitalWrite(LED_BUILTIN, LOW);

}

int mergeBins(int start, int end, float barht[]){
  float output = 0;
  for(int i=start; i<end+1; i++){
    if(barht[i]>THRESH) output+=barht[i];
  }
  //  output /= float(end-start);
  return output;
}

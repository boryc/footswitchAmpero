
#include <MIDI.h>  //using MIDI Library boy Francois Best
#include <midi_defs.h>
#include <midi_message.h>
#include <midi_namespace.h>
#include <midi_settings.h>

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut

int inPin1 = 7;         // the number of the input pin
int inPin2 = 6;

int reading1;           // the current reading from the input pin
int previous1;    // the previous reading from the input pin
int reading2;           // the current reading from the input pin
int previous2;    // the previous reading from the input pin

int record = 1;
int overdub = 2;
int play = 3;
int stp = 4;

int button1 = record;
int button2 = play;


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
//  Serial.begin(9600); // setup serial for MIDI
  Serial.begin(31250); // setup serial for MIDI

//  Serial.println("start");
   
  pinMode(inPin1, INPUT);
  pinMode(inPin2, INPUT);
  
  previous1 = digitalRead(inPin1);
  previous2 = digitalRead(inPin2);
}

void loop()
{
  
  reading1 = digitalRead(inPin1);
  reading2 = digitalRead(inPin2);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading1 != previous1 && millis() - time > debounce) {

    if(button1 == record) {
 //     Serial.println("Send Record, [Dub, Play]");
      midiOut.sendControlChange(60,127,1); 
     
      button1 = overdub;
      button2 = play;
    }
    else {
      //Serial.println("Send Overdub, [Dub, Stop]");
      midiOut.sendControlChange(60,0,1); 
      button1 = overdub;
      button2 = stp;
    }
    
    time = millis();
    previous1 = reading1;    
  }
 
  if (reading2 != previous2 && millis() - time > debounce) {
    
    if(button2 == play) {
        //Serial.println("Send Play, [Dub, Stop]");
        midiOut.sendControlChange(61,127,1); 

        button1 = overdub;
        button2 = stp;
    }
    else {
      //Serial.println("Send Stop, [Record, Play]");
      midiOut.sendControlChange(61,0,1); 
      button1 = record;
      button2 = play;
    }

    time = millis();
    previous2 = reading2;    
  }
}

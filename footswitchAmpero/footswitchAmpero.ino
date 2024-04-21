
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
int play = 2;
int stop = 3;

int status = play;



// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 100; // the debounce time, increase if the output flickers
long clear = 2000;  // clear time

void setup()
{
  Serial.begin(31250); // setup serial for MIDI

  Serial.println("start");
   
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

    if(status == stop ) {
      Serial.println("Send Play, Status Play");
      midiOut.sendControlChange(64,127,1); 
     
      status = play;
      
    }
    else if(status == play) {
      Serial.println("Send Record/Overdub, Status Record");
      midiOut.sendControlChange(63,0,1); 
     
      status = record;
      
    }
    else {
      Serial.println("Send Play, Status Play");
      midiOut.sendControlChange(64,127,1); 
      status = play;
    }
    
    time = millis();
    previous1 = reading1;    
  }
 
  if (reading2 != previous2 && millis() - time > debounce) {
         
    Serial.println("Send Stop, Status Stop");
    midiOut.sendControlChange(64,0,1); 

    status = stop;
    time = millis();
    previous2 = reading2;    
  }
  
}

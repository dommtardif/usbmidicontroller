
#include "MIDIUSB.h"
#include "MCP_ADC.h"

MCP3008 adc;

//Number of pots connected to ADC
const int potsQty = 3;

//Pots midi controller number
const int potsMidiController[potsQty] = {16,17,18};

//Pots previous values
int potsPrevVal[potsQty];

//Quanty of buttons connected to digital IO
const int buttonsQty = 4;

//Buttons pin definitions
const int buttonsPin[buttonsQty] = {5,4,3,2};

//Buttons midi controller numner
const int buttonsMidiController[buttonsQty] = {10,11,12,13};

//ADC chip select pin
const int ADC_CSPin = 10;

//Midi channel to operate on
const byte midiChannel = 0;

void midiPresetReturn(){
midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.byte1 == 0xC0) {
        Serial.print("Preset Changed to:");
        Serial.println(rx.byte2);    
      }
      if (rx.byte1 == 0xB0) {
        Serial.print("Bank Changed to:");
        Serial.println(rx.byte3);        
      }      
      // Serial.print("Received: ");
      // Serial.print(rx.header, HEX);
      // Serial.print("-");
      // Serial.print(rx.byte1, HEX);
      // Serial.print("-");
      // Serial.print(rx.byte2, HEX);
      // Serial.print("-");
      // Serial.println(rx.byte3, HEX);
    }
  } while (rx.header != 0);  
}

void setup() {
  Serial.begin(115200);

  adc.begin(ADC_CSPin);

  for (int button = 0; button < buttonsQty; button++){
    pinMode(buttonsPin[button], INPUT);
  }

  for (int port = 0; port < potsQty; port++) {
    potsPrevVal[port] = mapPot(adc.analogRead(port));
  }
}

int mapPot(int value) {
  return int(map(value, 0, 1023, 0, 127));
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { (byte)0x0B, (byte)0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
  // Serial.print("Control:");
  // Serial.print(control);
  // Serial.print("  value:");
  // Serial.print(value);
  // Serial.println(" Sent change");
}

void loop() {
  for (int button = 0; button < buttonsQty; button++) {
    if (digitalRead(buttonsPin[button]) == HIGH) {
      controlChange(midiChannel, buttonsMidiController[button], 127);
      delay(250);
    }
  }
  midiPresetReturn();  
  int tmpRead = 0;
  for (int port = 0; port < potsQty; port++) {
    tmpRead = mapPot(adc.analogRead(port));
    if (tmpRead != potsPrevVal[port]) {
      controlChange(midiChannel,potsMidiController[port],tmpRead);      
      potsPrevVal[port] = tmpRead;
    }
  }
}

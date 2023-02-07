
#include "MIDIUSB.h"
#include "MCP_ADC.h"

//Declare ADC
MCP3008 adc;

//Number of pots connected to ADC
const int potsQty = 4;

//Pots midi controller number
const int potsMidiController[potsQty] = { 16, 17, 18, 19 };

//Pots previous values
int potsPrevVal[potsQty];

//Quanty of buttons connected to digital IO
const int buttonsQty = 8;

//Buttons pin definitions
const int buttonsPin[buttonsQty] = { 9, 8, 7, 6, 5, 4, 3, 2 };

//Buttons midi controller numner
const int buttonsMidiController[buttonsQty] = { 8, 9, 10, 11, 12, 13, 14, 15 };

//ADC chip select pin
const int ADC_CSPin = 10;

//Midi channel to operate on
const byte midiChannel = 0;

void setup() {
  Serial.begin(115200);

  adc.begin(ADC_CSPin);

  //Set all pins to input
  for (int button = 0; button < buttonsQty; button++) {
    pinMode(buttonsPin[button], INPUT);
  }
  //Gather all pots initial values
  for (int port = 0; port < potsQty; port++) {
    potsPrevVal[port] = mapPot(adc.analogRead(port));
  }
}

//Map ADC reading to midi value 0-127
int mapPot(int value) {
  return int(map(value, 0, 1023, 0, 127));
}

//Send midi control change
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { (byte)0x0B, (byte)0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

//Receive guitarX preset change
// void midiPresetReturn() {
//   midiEventPacket_t rx;
//   bool receivedPacket = false;
//   do {
//     rx = MidiUSB.read();
//     switch (rx.header) {
//       case 0xC:
//         Serial.print("Preset Changed to:");
//         Serial.println(rx.byte2);
//         break;
//     }
//   } while (rx.header != 0);
// }

void loop() {
  //Read buttons input and send to midi accordingly
  for (int button = 0; button < buttonsQty; button++) {
    if (digitalRead(buttonsPin[button]) == HIGH) {
      controlChange(midiChannel, buttonsMidiController[button], 127);
      delay(250);
    }
  }

  int tmpRead = 0;  //temporary reading of pots to send midi only on change

  //Read all pots values and send to midi accordingly
  for (int port = 0; port < potsQty; port++) {
    tmpRead = mapPot(adc.analogRead(port));
    if (tmpRead != potsPrevVal[port]) {
      controlChange(midiChannel, potsMidiController[port], tmpRead);
      potsPrevVal[port] = tmpRead;
    }
  }

  //midiPresetReturn();
}

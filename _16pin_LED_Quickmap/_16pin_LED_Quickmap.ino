/*automate the process of figuring out pins for an LED array
1) plug in all of your wires into any pins capable of output: digital 2-7 and analog A0-A5. 
2) Leave pins d0 and d1 open for communication! Rx, Tx
3) set arrayWidth and arrayHeight (default 8x8)
4) Change pinOuts to the actual pins that you used. Order is not important.
5) Upload to your Arduino.
6) Start the Serial Monitor (Ctrl-Shift-M as a shortcut)
7) Type numbers into the Send box at the top of the Serial Monitor screen
  a) type 0 if no lights are on and hit enter
  b) type the column or row number and hit enter
8) Repeat until all pins are mapped
9) Copy/Paste values from the Serial Monitor Output window into the {} in the commented out lines below pinOuts[];
10) Take those two lines over to your project! Each array is in order of the pins on the array.
11) Enjoy!

Let me know if you found this at all useful or if you've found mistakes/improvements

Cheers!

Sam "Samwise" Barnum - barnumster@gmail.com
*/
const int arrayWidth = 8;  //change these values for your given array size. MUST be set to const so array size can be defined
const int arrayHeight = 8; //change these values for your given array size. MUST be set to const so array size can be defined
const int pinCount = arrayWidth * arrayHeight;

const byte pinOuts[pinCount] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A5, A4, A3, A2}; //pinCount is not actually necessary.

//const byte anodePinout[] {insert your pins here};
//const byte cathodePinout[] {insert your pins here};

byte cathodePins[arrayWidth]; //during Brute Force approach in anode search, this array holds all pin numbers NOT anodes. They must then be cathodes.
byte cathodeIndex = 0; //keep track of number of found cathodes

byte GLOBAL_ANODE[arrayWidth]; //this only works for a square...come up with a more general case for numbers of each electrode
byte GLOBAL_CATHODE[arrayHeight];


void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 16; i++) {
    pinMode(pinOuts[i], OUTPUT); //sets pins to output mode
  }
  //turn off all Ano
//  for (byte j = 0; j < arrayWidth; j++) {
//    GLOBAL_ANODE[j] = 0;
//  }
}

void loop() {
  byte YN;
  byte row;
  byte pin;
  int junk; //clear the Serial buffer stream

  //find all the anodes and write them to GLOBAL_ANODE[]
  for (byte a = 0; a < 16; a++)
  {
    allOff();
    YN = setAndCheck(a, 1);
    if (YN) {
      setAnode(a, YN);
    }
    else {
      Serial.println("Cool. You found a cathode! We'll figure out which one shortly\n");
      //Serial.print("Pin: "); Serial.print(pinOuts[a]); Serial.print(" written to index: "); Serial.println(cathodeIndex);
      cathodePins[cathodeIndex] = pinOuts[a];
      cathodeIndex++; //write the next cathodePin one spot over
    }
  }
  Serial.println("All anodes found! Copy paste the pins into a text file using Ctrl-C/Ctrl-V\n\n");


  //find all the cathodes and write them to GLOBAL_CATHODE[]
  for (byte c = 0; c < arrayWidth; c++)
  {
    allOn();
    pin = cathodePins[c];
    digitalWrite(pin, LOW); //turn off the one pin
    Serial.println("Which column is on?");
    while (Serial.available() == 0) ; //wait until there is a character
    row = Serial.parseInt();
    setCathode(pin, row);
  }
  Serial.println("All cathodes found! Copy paste the pins into a text file using Ctrl-C/Ctrl-V\n");
  Serial.println("You should be seing a smiley Face right now. If not...I'm not sure why. Sorry!");
  //test that your pins work
  while (true) displaySmileyFace();
}


void setAnode(byte anode, byte val) {
  byte rowColVal = val;

  Serial.print("Anode on pin: "); Serial.println(pinOuts[anode]);
  Serial.print("Corresponding to row: "); Serial.print(val); Serial.print(" (Index: "); Serial.print(val - 1); Serial.println(" )");
  rowColVal -=  1 ; //to go from 1-8 to index of 0-7
  GLOBAL_ANODE[rowColVal] = pinOuts[anode]; //correlates pinOuts with rows
  Serial.print("anodePinout[] = { ");
  for (int i = 0; i < arrayWidth; i++)
  {
    Serial.print(GLOBAL_ANODE[i]);
    if (i < arrayWidth - 1) Serial.print(","); //to avoid printing the last ','
  }
  Serial.println(" };\n");
}

void setCathode(byte cathodePin, byte val) {
  byte rowColVal = val;

  Serial.print("Cathode on pin: "); Serial.println(cathodePin);
  Serial.print("Corresponding to column: "); Serial.println(val);
  rowColVal -= 1; ////to go from 1-8 to index of 0-7
  GLOBAL_CATHODE[rowColVal] = cathodePin; //correlates pinOuts with rows
  Serial.print("cathodePinOut = { ");
  for (int i = 0; i < arrayWidth; i++)
  {
    Serial.print(GLOBAL_CATHODE[i]);
    if (i < arrayWidth - 1) Serial.print(","); //to avoid printing the last ','
  }
  Serial.println(" };\n");

}



byte setAndCheck(byte p1, boolean onOff) { //returns 1 if user sees a light on, and 0 if no lights
  byte y;
  int junk;

  digitalWrite(pinOuts[p1], onOff);
  junk = Serial.read() ;
  Serial.println("Which row is on? '0' if no lights are on");
  while (Serial.available() == 0) ; //wait until there is a character
  y = Serial.parseInt();

  return y;
}

void allOff() {
  for (int k = 0; k < pinCount; k++)
  {
    digitalWrite(pinOuts[k], LOW);
  }
}

void allOn() {
  for (int k = 0; k < pinCount; k++)
  {
    digitalWrite(pinOuts[k], HIGH);
  }
}

void displaySmileyFace() {
  byte source_Row;
  byte drain_Column;


  byte pic[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  };

  /*
  take this part out if transfering to another program
  */

//  const byte anodePinout[] {3, 12, 16, 8, 2, 17, 9, 19}; //turns on rows
//  const byte cathodePinout[] {11, 6, 5, 4, 18, 7, 10, 13}; //turns on columns
 byte anodePinout[arrayWidth];
 byte cathodePinout[arrayWidth];
  for (int i = 0; i < arrayWidth; i++) {
     anodePinout[i] = GLOBAL_ANODE[i];
     cathodePinout[i] = GLOBAL_CATHODE[i];
   }
  const byte pixels = 64;//total number of display pixels
  int flickerMicro = 300;
  
  for (byte k = 0; k < arrayWidth; k++) //turn off all the pins before starting
  {
    digitalWrite(anodePinout[k], LOW);
    digitalWrite(cathodePinout[k], HIGH);
  }
  
  /*delete between the comment boxes*/
  for (byte i = 0; i < pixels; i++)
  {
    if (pic[i]) //if the picture has something other than a zero
    {
      source_Row = anodePinout[i / 8] ;
      drain_Column = cathodePinout[i % 8];
      digitalWrite(drain_Column, LOW); //Anode doesn't send current in LOW mode. No lights will go on.
      digitalWrite(source_Row, HIGH); //cathode can't accept current when in HIGH mode. No lights will go on
      delayMicroseconds(flickerMicro);
      digitalWrite(drain_Column, HIGH);
      digitalWrite(source_Row, LOW);
      delayMicroseconds(flickerMicro);
    }
  }
}

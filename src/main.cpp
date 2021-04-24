// add delay to tare, only average tare, add display somethign when tare, longer average
// it needs .5v max, which means a 1/6th voltage division(ish), and i think that means we can just stick a 1500 resistor in front of it to limit it

#include <SPI.h>
#include "LCD_Functions.h"
#include <Arduino.h>

void displayReading(bool );

int up = 0;// stores analog read values for the 2 sensors
int down = 0;
int tare = 0;// stores zero point
int dis = 0;// displacement from tare
int ave[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};// used to calculate the average reading
int avePos = 0;// position in array
long aveTare;// average with tare
int aveTen;
int aveTwenty;
int aveForty;

void setup()
{
  Serial.begin(115200);

  lcdBegin(); // This will setup our pins, and initialize the LCD
  updateDisplay(); // with displayMap untouched, SFE logo
  setContrast(40); // Good values range from 40-60
  delay(500);
  pinMode(12, INPUT_PULLUP);
  
  //lcdFunTime(); // Runs a 30-second demo of graphics functions
  
  clearDisplay(WHITE);
  updateDisplay();
}

void loop()
{
  up = analogRead(A0);
  down = analogRead(A1);
  dis = (up-down)-tare;// this means 700-300 and 600-200 would yeild the same result, which is negates horizontal changes(ish)
  ave[avePos] = dis;
  
  if(avePos != 39){
    avePos++;
  }
  else{
    avePos = 0;
  }

  displayReading(true);
  // can probably use proportional control here(ish)
  // store current voltage level, then increase it proportional to the ammount its off from 1 sec ave by, wait a few cycles and readjust
  // that should work so it zeroes in with smaller changes at the end. could also add easing in and easing out of voltage levels for vibration
  // and a "servo" button to toggle it on and off maybe?


  if(digitalRead(12) == LOW){// tare the scale after a delay w/display and getting new readings
    clearDisplay(BLACK);
    setRect(26,16,55,28, true, WHITE); // this is the white outline
    setRect(27,17,54,27, true, BLACK); // inside of outline(should have done false so it wasnt filled
    setStr("TARE", 29, 19, BLACK);
    setLine(28,18,52,18, WHITE);// top horizontal line so it has a border
    setLine(28,19,28,26, WHITE);// this is the left vertical line
    updateDisplay();
    delay(3000);
    for(int i = 0; i < 40; i++){
      displayReading(false);
      delay(10);
    }
    // if this doesnt work could do the readings before i even touched it
    tare += aveForty;// should store the 4s average, it needs to be += to account for past tare
    for(int i=0; i<40; i++){// this makes the average show the tare instantly
      ave[i] = ave[i] - tare;
    }
  }
  delay(100);
}

void displayReading(bool display){
  aveTare = 0;// stores the average, all this calculates the average
  for(int i = 0; i < 10; i++){
    int counter = avePos-i;// this makes it so it uses the last 10 points not the first in the array
    if(counter < 0){
      counter = (39+counter);// this is + cause counter is negative
    }
    aveTare += ave[counter];
  }
  aveTare /= 10;
  aveTen = aveTare;// last 10 readings average

  aveTare = 0;
  for(int i = 0; i < 20; i++){
    int counter = avePos-i;// this makes it so it uses the last 20 points not the first in the array
    if(counter < 0){
      counter = (39+counter);// this is + cause counter is negative
    }
    aveTare += ave[counter];
  }
  aveTare /= 20;
  aveTwenty = aveTare;// last 20 readings average

  aveTare = 0;
  for(int i = 0; i < 40; i++){// this doesnt need counter cause the average is all 40
    aveTare += ave[i];
  }
  aveTare /= 40;
  aveForty = aveTare;// last 40 readings average

  clearDisplay(WHITE);
  char buffer [6];
  itoa(up, buffer,10);// write up into the buffer, then to the screen
  setStr(buffer,0,0,BLACK);
  itoa(down, buffer,10);// write down into the buffer, then to the screen
  setStr(buffer,LCD_WIDTH-(5*5),0,BLACK);
  itoa(dis, buffer,10);// write displacement into the buffer, then to the screen
  setStr(buffer,5*5,0,BLACK);
  itoa(aveTen, buffer,10);// write averages into the buffer, then to the screen
  setStr(buffer,5*5,8,BLACK);
  itoa(aveTwenty, buffer,10);
  setStr(buffer,5*5,16,BLACK);
  itoa(aveForty, buffer,10);
  setStr(buffer,5*5,24,BLACK);

  if(display){
    updateDisplay();
  }
}

void lcdFunTime()
{
  clearDisplay(WHITE); // Begin by clearing the display
  
  setStr("full of stars", 0, LCD_HEIGHT-8, BLACK);
  updateDisplay();
  delay(1000);
  
  /* setRect Example */
  clearDisplay(WHITE); // Start fresh
  // setRect takes six parameters (x0, y0, x1, y0, fill, bw)
  // x0, y0, x1, and y0 are the two diagonal corner coordinates
  // fill is a boolean, which determines if the rectangle is
  // filled in. bw determines the color 0=white, 1=black.
  for (int x=0; x<LCD_WIDTH; x+=8)
  { // Swipe right black
    setRect(0, 0, x, LCD_HEIGHT, 1, BLACK);
    updateDisplay();
    delay(10);
  }
  
  /* setChar & setStr Example */
  // setStr takes 4 parameters: an array of characters to print,
  // x and y coordinates for the top-left corner. And a color
  setStr("Modern Art", 0, 10, WHITE);
  updateDisplay();
  delay(2000);
  
  /* setBitmap Example */
  // setBitmap takes one parameter, an array of the same size
  // as our screen.
  //setBitmap(xkcdSandwich);
  updateDisplay();
}
/*
int read255();// reads 0-255 over serial

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    int pwmVal = read255();
    Serial.println(pwmVal);
    analogWrite(11, pwmVal);
  }
}

int read255(){
  char buffer[4];
  int incomingByte = 0;
  for(int i = 0; i < 4; i++) {
    //read in a byte as a char, append it to the string, if its =10 then exit the loop, once the loop is exited append the null character then call atoi
    incomingByte = Serial.read();
    //Serial.println(incomingByte, DEC);
    if(incomingByte == 13 || i == 3){// assuming the value read isnt a \n, continue
      incomingByte = Serial.read();// discard the \n
      buffer[i] = '\0';
      break;
    }
    buffer[i] = incomingByte;
  }
  //Serial.print("Buffer: ");
  //Serial.println(buffer);
  int total = atoi(buffer);
  
  if(total > 255 || total < 0){
    Serial.print("Bad output: ");
    Serial.println(total);
    return 0;
  }

  else{
    return total;
  }
}*/
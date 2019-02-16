#include<LCD595.h>

#define DATA A1
#define CLK  A2
#define STROB  A3

#define RS 0
#define EN 1
/*
  LCD.RS--->74HC595.Q7
  LCD.RW--->GND
  LCD.EN--->74HC595.Q6
  LCD.D4--->74HC595.Q3
  LCD.D5--->74HC595.Q2
  LCD.D6--->74HC595.Q1
  LCD.D7--->74HC595.Q0
*/


LCD595 lcd(CLK,DATA,STROB);


void setup(){
  lcd.init();
  lcd.print("hello");
  delay(1000);
  lcd.clear();
  lcd.print("Hello World!");
}


void loop(){
 
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  
}

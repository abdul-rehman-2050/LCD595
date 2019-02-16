#include "LCD595.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"


LCD595::LCD595(uint8_t clk, uint8_t data, uint8_t strob){
	CLK = clk;
	DATA = data;
	STROB = strob;
	pinMode(CLK,OUTPUT);
	pinMode(DATA,OUTPUT);
	pinMode(STROB,OUTPUT);
	digitalWrite(STROB,LOW);
	digitalWrite(DATA,LOW);
	digitalWrite(CLK,LOW);
}
void LCD595::setCursor(uint8_t col, uint8_t row){
  uint8_t address; 
   switch(row) { 
     case 0 : address=0x80;break; 
     case 1 : address=0xc0;break; 
     case 2 : address=0x94;break; 
     case 3 : address=0xd4;break; 
   } 
   address+=col; 
   command(address);
}

void LCD595::cursor_on(uint8_t on)
{
   if (on)
   {
      command(0x0F);           //turn LCD cursor ON
   }
   else
   {
      command(0x0C);           //turn LCD cursor OFF
   }
}

void LCD595::clear()
{
  command(0x01);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LCD595::home()
{
  command(0x02);        // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}



void LCD595::flushByteToSend(uint8_t byteToSend){
  digitalWrite(STROB,LOW);
  shiftOut(DATA, CLK, LSBFIRST, byteToSend);
  digitalWrite(STROB,HIGH);
}

void LCD595::pulseEnable(uint8_t EN,uint8_t byteToSend) {
  bitClear(byteToSend, EN);flushByteToSend(byteToSend);
  delayMicroseconds(1);    
  
  bitSet(byteToSend, EN);flushByteToSend(byteToSend);
  delayMicroseconds(1);    // enable pulse must be >450ns
  
  bitClear(byteToSend, EN);flushByteToSend(byteToSend);
  delayMicroseconds(100);   // commands need > 37us to settle
}

void LCD595::write4bits(uint8_t value, uint8_t rs, uint8_t en){
  byte b2send = 0;
  int RS = 0,EN=1;
  
  int i=0;
  for(i=0;i<4;i++){
    if(((value >> i) & 0x01)==0){
      bitClear(b2send,(i+4));
    }else{
      bitSet(b2send,(i+4));
    }
   }
  if(rs){
    bitSet(b2send,RS);
  }
  if(en){
    bitSet(b2send,EN);
  }
  //mlcd.flushByteToSend(b2send);
  pulseEnable(1,b2send);
}

void LCD595::sendNibbleByNibble(uint8_t value, uint8_t mode) {
  write4bits(value>>4,mode,0);
  write4bits(value,mode,0);
}

void LCD595::reset(){

	delay(50); //power on delay, it will be there for sure

	write4bits(0x03,0,0);
	delayMicroseconds(4500); // wait min 4.1ms
    
   // second try
    write4bits(0x03,0,0);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03,0,0); 
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02,0,0); 
  
}

void LCD595::init(){
	reset();
    //now start init()
     command(0x28);   //4bit, 2 line, 5x7
     delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(0x28);
    delayMicroseconds(150);

    // third go
    command(0x28);
    delay(1);


    command(0x0C);  //display, no cursor, no blink
    delay(1);
    command(0x06);    //Auto increment no display shift
    delay(1);
    command(0x80);    //address DDRAM with 0 offset 80h
    delay(1);
}


inline void LCD595::command(uint8_t value){
  sendNibbleByNibble(value, 0);
}

inline size_t LCD595::write(uint8_t value){
  sendNibbleByNibble(value, 1);
  return 1; // assume sucess
}

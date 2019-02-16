#ifndef LCD595_h
#define LCD595_h

#include <inttypes.h>
#include "Print.h"



class LCD595 : public Print{
public:
  LCD595(uint8_t, uint8_t, uint8_t);
  void init();
  virtual size_t write(uint8_t);
  void command(uint8_t);
  void cursor_on(uint8_t on);
  void clear();
  void home();
  void setCursor(uint8_t col, uint8_t row);
  using Print::write;
private:
  uint8_t CLK;
  uint8_t STROB;
  uint8_t DATA;
  void flushByteToSend(uint8_t);
  void pulseEnable(uint8_t,uint8_t);	//ENABLE BIT NUMBER
  void write4bits(uint8_t value, uint8_t rs, uint8_t en);
  void sendNibbleByNibble(uint8_t, uint8_t);
  void reset();
  
};



#endif

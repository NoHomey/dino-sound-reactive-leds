#include <Adafruit_TLC5947.h>

Adafruit_TLC5947::Adafruit_TLC5947(const uint8_t& n) {
  num = n;
  pwmbuffer = (uint16_t *)calloc(24*n, sizeof(uint16_t));
  _REGISTER_TLC5947 = _SETUP_TLC5947;
}

Adafruit_TLC5947::~Adafruit_TLC5947(void) {
  free(pwmbuffer);
}

void Adafruit_TLC5947::write(void) {
  _PORT_TLC5947 &= ~_BV(_LATCH_TLC5947);
  for (int16_t c = 24*num - 1;c >= 0;--c) {
    for (int8_t b=11; b>=0; b--) {
      _PORT_TLC5947 &= ~_BV(_CLOCK_TLC5947);
      (pwmbuffer[c] & (1 << b)) ? _PORT_TLC5947 |= _BV(_DATA_TLC5947) : _PORT_TLC5947 &= ~_BV(_DATA_TLC5947);
      _PORT_TLC5947 |= _BV(_CLOCK_TLC5947);
    }
  }
  _PORT_TLC5947 &= ~_BV(_CLOCK_TLC5947);
  _PORT_TLC5947 |= _BV(_LATCH_TLC5947);
  _PORT_TLC5947 &= ~_BV(_LATCH_TLC5947);
}

void Adafruit_TLC5947::setPWM(const uint16_t& chan, const uint16_t& pwm) {
  uint16_t val;
  val = pwm > 4095 ? 4095 : pwm;
  if (chan > 24*num) return;
  pwmbuffer[chan] = val;  
}

void Adafruit_TLC5947::setLED(const uint8_t& lednum, const uint16_t& b, const uint16_t& r, const uint16_t& g) {
  setPWM(lednum*3, b);
  setPWM(lednum*3+1, r);
  setPWM(lednum*3+2, g);
}

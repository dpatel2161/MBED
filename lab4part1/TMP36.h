#include "mbed.h"
class TMP36
{
public:
    TMP36(PinName pin) : _pin(pin) {}
    float read();
private:
    //class sets up the AnalogIn pin
    AnalogIn _pin;

};



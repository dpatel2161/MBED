#include "mbed.h"
// new class to play a note on Speaker based on PwmOut class
class TMP36
{
 public:
    TMP36(PinName pin) : _pin(pin) {}
    float read();
private:
    //class sets up the AnalogIn pin
    AnalogIn _pin;
    
    
    //instantiate new class to set p15 to analog input //to read and convert TMP36 sensor's voltage output
        
};



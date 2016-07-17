#include "mbed.h"
#include "TMP36.h"

float TMP36::read()
{
    //convert sensor reading to temperature in degrees C
    return ((_pin.read()*3.3)-0.500)*100.0;
}
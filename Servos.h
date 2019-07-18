#include <Servo.h>

// While a good idea, this class for some reason caused a ton of jittering and uncontrollable behavior
#ifndef Servos_h
#define Servos_h

class Servos {

  public:
    Servos(int pin, int _minAngle, int _maxAngle);
    void drop();
    int getMinAngle();
    int getMaxAngle(); 
    int getMidAngle();
    void writeTo(int angle);

  private: 

    Servo servo;
    int _pin;
    int _minAngle;
    int _maxAngle;
};



#endif

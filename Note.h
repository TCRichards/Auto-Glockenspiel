#include <Arduino.h>

#ifndef Note_h
#define Note_h

class Note {

  public:

    Note(int value, int duration);
    Note(int value, int duration, Note* next);
    int getDuration();
    int getValue();
    
    Note* getNext();
    bool hasNext();
    void setNext(Note* next);
    void printOut();

    
    void setDuration(int duration);
    void setValue(int value);
  
  private:
  
    int _duration;
    int _value;
    Note* _next;
  
};

#endif

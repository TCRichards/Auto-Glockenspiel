#ifndef Button_h
#define Button_h

class Button {
  
 public:
  
  Button(int pin);
  bool isOn();
  void reset();
  bool wasPressed();
  void updateButton();
    
 private:
  
  int _pin;
  bool _state;
  bool _lastState;
  unsigned int _startDebounce;
  bool _debouncing;
  unsigned long debounceMS;
};

#endif

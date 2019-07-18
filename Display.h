#ifndef Display_h
#define Display_h
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "Note.h"

class SoftwareSerial;

class Display {
  
  public:

    String noteNames[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  
    Display(int tx);
    void printNote(Note* note);
    void clearDisplay();
    void incrementScreen();
    void decrementScreen();
    
    int getScreen();
    int getSongNum();

    void setNumSongs(int num);
    int getNumSongs();
    
    void incrementSongs();
    void decrementSongs();
    
    void setRecording(bool input);
    void setSongScreen(bool input);

    void writeScreen();
    void moveCursor(int horiz, boolean firstLine);


    SoftwareSerial lcd;

  
  private:
    int _screenNum;
    int _songNum;

    int _maxSongs;
    int _maxScreens;
    
    bool _recording;
    bool _viewingSongs;
    
    void backspace();

};


#endif

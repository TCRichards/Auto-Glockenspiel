#include "Display.h"
#include <SoftwareSerial.h>

Display::Display(int tx):lcd(0, tx) {
  SoftwareSerial s(0,tx);  // assign it a bogus tx pin, since displays don't need to transmit anything
  lcd = s;
  _screenNum = 0;
  _maxScreens = 2;
  
  _recording = false;
  _viewingSongs = false;
  
  lcd.begin(19200);
  lcd.write(22);  // Makes cursor invisible

  writeScreen();  // Show the default menu screen
}


void Display::writeScreen() {
  clearDisplay();
  switch (_screenNum) {
    case 0:
      if (!_recording) {
        lcd.print("RECORD NEW SONG");
      } else {
        lcd.print("DETECTING NOTES..");
      }
      break;
    case 1:
      lcd.print("View Songs");
      break;
    case 2: // Not included anymore
      lcd.print("SCREEN #3");
      break;
  }
}

int Display::getNumSongs() {
  return _maxSongs;
}

void Display::setNumSongs(int num) {
  _maxSongs = num;
}

void Display::incrementScreen() {
  _screenNum = (_screenNum + 1) % _maxScreens;
  writeScreen();
}

int Display::getSongNum() {
  return _songNum;
}
void Display::decrementScreen() {
  _screenNum = (_screenNum + (_maxScreens - 1)) % _maxScreens;  // Janky way to decrement avoiding negatives
  writeScreen();
}

void Display::incrementSongs() {
  _maxSongs++;
}

void Display::decrementSongs() {
  _maxSongs--;
}


void Display::setRecording(bool input) {
  _recording = input;
  writeScreen();
}

void Display::setSongScreen(bool input) {
  _viewingSongs = input;
  writeScreen();
}

int Display::getScreen() {
  return _screenNum;
}
// Form feed: clears the display and moves cursor to position 0
void Display::clearDisplay() {
  lcd.write(12);
}

void Display::backspace() {
  lcd.write(8);
}

// Moves the cursor to one of the 16 allowed postionsu
// @Param pos = horizontal position (hexadecimal), which line to write to 
// Use Hexadecimal characters for 0 - 15 range
void Display::moveCursor(int horiz, boolean firstLine) {
  if (firstLine) {
    lcd.write(map(horiz, 0, 15, 128, 143)); 
    Serial.println(map(horiz, 0, 15, 128, 143));
  } else {
    lcd.write(map(horiz, 0, 15, 148, 163));
  }
}

void Display::printNote(Note* note) {
  clearDisplay();
  moveCursor(0, true);  // Move the cursor to the start of the first line
  lcd.print("Value = ");
  int value = note->getValue();
  int noteIdx = value % 12;
  lcd.print(noteNames[noteIdx]);
  moveCursor(0, false); // Move the curson to the start of the second line
  lcd.print("Duration = ");
  lcd.print(note->getDuration());
}

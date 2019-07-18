/*
 * Code to run marble-dropping, self-playing glockenspiel
 * PHYS 231 Feasibility Study
 * Author: Thomas Richards
 * Date: 5/4/2019
 * 
 * 
 */

#include <Arduino.h>
#include <MIDI.h>
#include <EEPROM.h>
#include <Servo.h>

#include "NoteList.h"
#include "Note.h"
#include "Button.h"
#include "Display.h"
#include "Servos.h"                            // Not using Servos due to some unexplained runtime issues

MIDI_CREATE_DEFAULT_INSTANCE(); 

// Sadly, these also need to be stored in EEPROM
unsigned char numSongsAddr = 0;                // Location in EEPROM that stores the current number of songs
unsigned char songLengths[10];                 // Length of the song in corresponding EEPROM location
// these song Lengths are stored in the following EEPROM locations

unsigned char listStartingAddr;             
unsigned char memAddr[10];                     // Location in EEPROM memory of each list's head

int nextSongIdx;                               // Stores which song index we are currently on

// Hardware components

// LEDS are wired to invert (switched power and ground)
int rxLED = 8;                                 // Blinked every time NoteOn message is received
int recordingLED = 9;                          // Turns on when in recording mode
Display* lcd = new Display(13);       
// Buttons are also wired to invert 
Button* selectBut = new Button(A1);
Button* upBut = new Button(A0);

// Couldn't get these working...
//Servos* servo1 = new Servos(2, 55, 78);       // Attached to D7, minAngle=55, maxAngle=78
//Servos* servo2 = new Servos(3, 55, 78);       // Attached to D3

Servo servo1;
Servo servo2;


int servoMinAngle = 55;
int servoMaxAngle = 80;
int servoMidAngle = 0.5 * (servoMinAngle + servoMaxAngle);

// Linked list storing note values
NoteList* list = new NoteList();


// Booleans for logic control
bool recording;
bool viewingSongs;
bool notePlaying;                           // Is a note currently being held? Used to find duration

void setup() {
  pinMode(rxLED, OUTPUT);
  pinMode(recordingLED, OUTPUT);
  
  digitalWrite(rxLED, HIGH);                // Turn off LEDs
  digitalWrite(recordingLED, HIGH);

  /* EEPROM STUFF
  //int numSongs = EEPROM.read(numSongsAddr);
  lcd->setNumSongs(numSongs);

  int uCharSize = sizeof(numSongsAddr);  // Offset in EEPROM by this
  listStartingAddr = uCharSize * 11;     // Offset by 1 uchar for numSongsAddr, 10 uchars for song lengths
  int listAddr = listStartingAddr;
  // Load the song lengths for as many songs as there are
  int counter = 0;
  for (int i = uCharSize; i < uCharSize * numSongs; i += uCharSize) {
    listAddr = i + listStartingAddr;          //
    songLengths[counter] = EEPROM.read(i);    // Load the length of each song into the array
    memAddr[counter] = EEPROM.read(listAddr); // Load the addresses of each song into the array
    counter++;
  }
  */

  Serial.begin(9600);

  servo1.attach(5);
  servo2.attach(4);

  // Begin servos in closed position
  servo1.write(servoMinAngle);
  servo2.write(servoMinAngle);

  pinMode(rxLED, OUTPUT);
  pinMode(recordingLED, OUTPUT);

  recording = false;                        // Start in main menu - not recording
  nextSongIdx = listStartingAddr;           // The first

  MIDI.setHandleNoteOn(handleNoteOn);       // Assign handleNoteOn to be triggered every time MIDI.read() receives NoteOn message
}

/*
   Triggered every time a NoteOn command is read by MIDI
   Adds the next note to the list and lights an LED
*/
void handleNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(rxLED, LOW);          // Signal to user that note was detected with LED
  delay(200);
  digitalWrite(rxLED, HIGH);
  // "Play" the note in real time (prototyping only)
  if (note == 60) {                   // Servo 1 drops on middle C
    drop(servo1);
  } else if (note == 64) {            // Servo 2 drops on middle E
    drop(servo2);
  }
  // Set duration of new note to 1 (would ideally be time between sequential notes)
  Note* nextNote = new Note(note, 1); // Add the note played to the list regardless of value
  list->addNote(nextNote);
}

// Drops a single marble at a time by 3 write commands
void drop(Servo serv) {
  serv.write(servoMidAngle);
  delay(50);
  serv.write(servoMaxAngle);
  delay(50);
  serv.write(servoMidAngle);
  delay(50);
  serv.write(servoMinAngle);
}



void loop() {
  // Detect button presses
  selectBut->updateButton();
  upBut->updateButton();

  // Flip through the menu screens on the lcd
  if (upBut->wasPressed()) {
    lcd->incrementScreen();
  }
  if (selectBut->wasPressed()) {            // Operation depends on which screen
    int command = lcd->getScreen();         // Which screen were we on when button pressed?
    if (command == 0) {                     // Recording screen
      if (!recording) {                     // If not already recording, start
        lcd->setRecording(true);
        recording = true;
        digitalWrite(recordingLED, LOW);    // Turn on LED
        list->removeAll();                  // Clear the list
      } else {                              // If already recording, stop
        lcd->setRecording(false);
        recording = false;
        digitalWrite(recordingLED, HIGH);   // Turn off LED
        // Update lcd screen
        lcd->clearDisplay();
        lcd->lcd.print("DONE RECORDING");
        delay(1000);
        lcd->clearDisplay();
        lcd->lcd.print("PRESS SELECT TO SAVE TO MEMORY");
        bool selectPressed = false;
        int endTime = millis() + 4000;                 // Give the user 4 secs to select save
        while (!selectPressed && millis() < endTime) { // Debouncing not working with delay, so just digitalRead
          selectPressed = selectBut->isOn();           // Break out of loop when 
        }
        if (selectPressed) {                           // Only save the song in EEPROM if select is pressed
          // save the song to EEPROM (not implemented)
          /*
          Note* iterator = list->getNote(0);
          int nextAddr = memAddr[nextSongIdx];
          for (int counter = 0; counter < list->getSize(); counter++) {
            lcd->clearDisplay();
            EEPROM_writeNote(nextAddr, iterator);
            nextAddr += sizeof(iterator);
            nextAddr %= EEPROM.length();                // If maximum length exceeded, wrap around
            counter++;
          }
          */
          lcd->clearDisplay();
          lcd->lcd.print("SONG SAVED!");
          lcd->moveCursor(0, false);
          lcd->lcd.print("Length = ");
          lcd->lcd.print(list->getSize());
          delay(2500);
        }
        lcd->writeScreen();                             // Restore normal menu options
      }
    }

    else if (command == 1) {                            // On song screen
      //lcd->setSongScreen(true);                       // Didn't finish implementing song selection. Just step through current list
      lcd->clearDisplay();
      Note* iterator = list->getNote(0);
      for (int i = 0; i < list->getSize() && iterator->hasNext(); i++) {
        lcd->printNote(iterator);
        delay(1500);
        iterator = iterator->getNext();
        lcd->clearDisplay();
      }
      lcd->lcd.print("Done Traversing List");
      /*
        // Detect button presses
        upBut->updateButton();
        selectBut->updateButton();
        // Keep track of which song is selected on lcd
        if (upBut->wasPressed()) {
        lcd->incrementSongs();
        }  else if (selectBut->wasPressed()) { // Select the current song to be played
        lcd->setSongScreen(true);
        int songNum = lcd->getSongNum();    // Which song is selected -> corresponds 1:1 with location in EEPROM
        int len = songLengths[songNum];     // Size of the note list
        int addr = memAddr[songNum];        // Address of the head of the note list
        for (int i = 0; i < len; i++) {
          Note* nextNote;
          EEPROM_readNote(addr, nextNote);  // Load value from EEPROM into nextNote
          addr += sizeof(nextNote);
          lcd->clearDisplay();
          lcd->printNote(nextNote);
          if (nextNote->getValue() == 60) { // Only drop the marble if a middle C
           drop(servo1);
          } else if (nextNote->getValue() == 64) {
            drop(servo2);
          }
        }
      */
    }
  }
  
  // Only read inputs if recording
  if (recording) {
    digitalWrite(recordingLED, LOW);
    MIDI.read();
  } else {
    digitalWrite(recordingLED, HIGH);
  }
}


// End of loop
//==============================================================

/*
   Complex canned code taken from: https://playground.arduino.cc/Code/EEPROMWriteAnything/
   Required to write the note data structures to memory
*/
int EEPROM_writeNote(int ee, Note* note) {
  const byte* p = (const byte*)(const void*)note;
  unsigned int i;
  for (i = 0; i < sizeof(note); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

int EEPROM_readNote(int ee, Note* note) {
  byte* p = (byte*)(void*)note;
  unsigned int i;
  for (i = 0; i < sizeof(note); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}


/*
   Manual method of reading MIDI messages.  While the first byte is recieved the next
   two don't make sense.  Opted for library code instead

  void readInput() {
  // continuously read port
  if (Serial.available() > 0) {
    commandByte = Serial.read();
    noteByte = Serial.read();
    velocityByte = Serial.read();

    if (commandByte == noteOFF) {                          // Record the note in the list
      digitalWrite(rxLED, HIGH);                               // Signal to user that note was detected
      delay(100);
      digitalWrite(rxLED, LOW);

      Note* nextNote = new Note(noteByte, 1);
      list->addNote(nextNote);
    }
  }
  }
*/

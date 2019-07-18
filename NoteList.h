#ifndef NoteList_h
#define NoteList_h

#include <Arduino.h>
#include "Note.h"

class NoteList {

  public: 

    NoteList();
    void addNote(Note* nextNote);
    void removeAll();
    int getSize();
    Note* getNote(int index);
    void printOut();

  private:
  
    int _size;
    Note* head;
};

#endif

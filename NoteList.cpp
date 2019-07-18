#include "NoteList.h"
#include <Arduino.h>

NoteList::NoteList() {
  _size = 0;
  head = NULL;
}

void NoteList::addNote(Note* newNote) {
  if (head == NULL) { // Special case for first add
    head = newNote;
  } else { // Iterate through the entire list until we reach the end
    Note* iterator = head;
    while (iterator->hasNext()) {
      iterator = iterator->getNext();
    }
    iterator->setNext(newNote); 
  }
  _size++;
}

Note* NoteList::getNote(int index) {
  if (head == NULL) {
    return NULL;
  }
  Note* iterator = head;
  while (iterator->hasNext() && index > 0) {
    iterator = iterator->getNext();
    index--;
  }
  return iterator;
}

void NoteList::printOut() {
  Note* iterator = head;
  if (iterator == NULL) {
    return;
  }
  while (iterator != NULL) {
    iterator->printOut();
    iterator = iterator->getNext();
  }
}

int NoteList::getSize() {
  return _size;
}

void NoteList::removeAll() { // Deallocate each node manually :( 
  Note* current = head;
  Note* next = current;
  while (current != NULL) {
    next = current->getNext();
    free(current);
    current = next;
  }
  head = NULL;
  _size = 0;
}

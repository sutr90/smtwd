#include "Arduino.h"

#include "ledScroll.h"

const byte ledScroll::fsmTable[5][5] = {
	/* evt_bufferEmpty,        evt_empty,      evt_hasChar,       evt_noChar,         evt_t100, */
	{      st_loopBody,      st_printStr,      st_loopBody,      st_loopBody,      st_loopBody, },/* loopBody */
	{      st_loopHead,      st_loopHead,      st_loopHead,      st_loopHead,      st_loopBody, },/* loopHead */
	{     st_printChar,  st_scrollBuffer,     st_printChar,     st_printChar,     st_printChar, },/* printChar */
	{      st_printStr,      st_printStr,     st_printChar,      st_loopHead,      st_printStr, },/* printStr */
	{      st_printStr,  st_scrollBuffer,  st_scrollBuffer,  st_scrollBuffer,  st_scrollBuffer, },/* scrollBuffer */
};

char ledScroll::string1[] = " Brainy-Bits.com  \0";  // Scrolling Text

void ledScroll::pollEvents() {
	lastEvent = currentEvent;
	/* TODO: update currentEvent */
	evtChanged = currentEvent != lastEvent;
}

void ledScroll::action_loopHead(){
  currentEvent = evt_t100;
	interval = 1000;
	/* TODO: add action for state loopHead */
	c = string1-1;
}

void ledScroll::action_scrollBuffer(){
  /* TODO: add action for state scrollBuffer */
	currentEvent = evt_t100;
	interval = 75;
	if(bufferIndex < buffer[0]+1){
		m.shiftLeft(false, false);
		bufferIndex++;
	} else {
		currentEvent = evt_bufferEmpty;
   interval = 0;
	}
}
void ledScroll::action_printStr(){
	/* TODO: add action for state printStr */
  c++;
  if(*c == 0){
		currentEvent = evt_noChar;
	} else {
		currentEvent = evt_hasChar;
	}
}
void ledScroll::action_printChar(){
	/* TODO: add action for state printChar */
	memcpy_P(buffer, CH + 7*((*c)-32), 7);
	m.writeSprite(maxInUse*8, 0, buffer);
	m.setColumn(maxInUse*8 + buffer[0], 0);
	currentEvent = evt_empty;
	bufferIndex = 0;
}

void ledScroll::action_loopBody(){
	/* TODO: add action for state loopBody */
	m.shiftLeft(false, true);
	currentEvent = evt_empty;
}

void ledScroll::evalState(){
	switch(currentState){
		case st_loopHead:
			action_loopHead();
			break;
		case st_scrollBuffer:
			action_scrollBuffer();
			break;
		case st_printStr:
			action_printStr();
			break;
		case st_printChar:
			action_printChar();
			break;
		case st_loopBody:
			action_loopBody();
			break;
	}
}

ledScroll::ledScroll() : m(data, load, clock, maxInUse){
	interval = 0;
	previousMillis = 0;
	currentState = st_loopHead;
	currentEvent = evt_none;
	/*-----*/
	m.init(); // module MAX7219
	m.setIntensity(5); // LED Intensity 0-15
  action_loopHead();
}

void ledScroll::loop() {
	pollEvents();
	currentMillis = millis();
	if (evtChanged || currentMillis - previousMillis > interval) {
		previousMillis = currentMillis;
		if(!(evtChanged && currentEvent == evt_none)){ //evt did not change to evt_none
			currentState = (State) fsmTable[currentState][currentEvent];
		}
		evalState();
	}
}


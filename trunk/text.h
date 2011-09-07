/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Carl Ljungström

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/


#ifndef TEXT_H_
#define TEXT_H_

#include <stdio.h>
#include <util/delay.h>

#include "config.h"

#ifdef TEXT_ENABLED

#include "xconvert.h"
#include "oem6x8.h"
#include "time.h"
#include "adc.h"
#include "delay.h"
#include "gps.h"
#include "home.h"

// Text vars
static uint16_t const textLines[TEXT_LINES] = {TEXT_TRIG_LINES_LIST};
static char text[TEXT_LINES][TEXT_LINE_MAX_CHARS];
static uint8_t textPixmap[TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT];
#ifdef TEXT_INVERTED_ENABLED
static uint8_t textInverted[TEXT_LINES][TEXT_LINE_MAX_CHARS/8];
#endif // TEXT_INVERTED_ENABLED

// Functions
static void clearText() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		  text[i][j] = 0;
	  }		  
	}
}

static void clearTextPixmap() {
	for (uint16_t j = 0; j < TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
		textPixmap[j] = 0;
	}
}

#ifdef TEXT_INVERTED_ENABLED
static void clearTextInverted() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS/8; ++j) {
	    textInverted[i][j] = 0;
	  }
	}	  
}

static void setCharInverted(uint8_t line, uint8_t pos, uint8_t bitValue) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (bitValue == TEXT_INVERTED_OFF) {
	  textInverted[line][bytePos] ^= ~(1<<bitPos);
	}
	else if (bitValue == TEXT_INVERTED_ON) {
	  textInverted[line][bytePos] |= (1<<bitPos);
	}
	else { //TEXT_INVERTED_FLIP
	  textInverted[line][bytePos] ^= (1<<bitPos);
	}
}

static uint8_t charInverted(uint8_t line, uint8_t pos) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (textInverted[line][bytePos] & (1<<bitPos)) {
		return 1;
	}
	return 0;
}
#endif // TEXT_INVERTED_ENABLED

static uint8_t getCharData(uint16_t charPos) {
	if (charPos >= CHAR_ARRAY_OFFSET && charPos < CHAR_ARRAY_MAX) {
	  return eeprom_read_byte(&(oem6x8[charPos - CHAR_ARRAY_OFFSET]));
	}	
	else {
		return 0x00;
	}	  
}

static void updateTextPixmapLine(uint8_t textId, uint8_t line) {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; j++) {
		uint8_t val;
		if (text[textId][j] == ' ' || text[textId][j] == 0) {
			val = 0;
		}
		else {			
		  uint16_t charPos = (text[textId][j] * TEXT_CHAR_HEIGHT) + line;
		  val = getCharData(charPos);
#ifdef TEXT_INVERTED_ENABLED
		  if (charInverted(textId, j)) {
		    val = ~val;
		  }
#endif // TEXT_INVERTED_ENABLED
		}
		uint16_t bytePos = line*TEXT_LINE_MAX_CHARS + j; 
		textPixmap[bytePos] = val;			
	}
}

static void updateTextPixmap(uint8_t textId) {
	for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {
	  updateTextPixmapLine(textId, i);
	}
}

static uint8_t printText(uint8_t textId, uint8_t pos, char* str) {
	uint8_t length = strlen(str);
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    length = TEXT_LINE_MAX_CHARS;
	}
	strncpy(&text[textId][pos], str, length);
	return length+pos;
}

static uint8_t printNumber(uint8_t textId, uint8_t pos, int32_t number) {
	uint8_t length = 1;
	int32_t tmp = number;
	while (tmp > 9) {
		tmp /= 10;
		++length;
	}
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    return TEXT_LINE_MAX_CHARS;
	}	
	myItoa(number, &text[textId][pos]);
	return pos+length;
}

static uint8_t printNumberWithUnit(uint8_t textId, uint8_t pos, int32_t number, char* unit) {
	pos = printNumber(textId, pos, number);
	return printText(textId, pos, unit);
}

static uint8_t printTime(uint8_t textId, uint8_t pos) {
	if (timeHour < 10) {
		text[textId][pos++] = '0';
	}
	pos = printNumberWithUnit(textId, pos, timeHour, ":");
	if (timeMin < 10) {
		text[textId][pos++] = '0';
	}	
	pos = printNumberWithUnit(textId, pos, timeMin, ":");
	if (timeSec < 10) {
		text[textId][pos++] = '0';
	}	
	return printNumber(textId, pos, timeSec);
}

static uint8_t printAdc(uint8_t textId, uint8_t pos, const uint8_t adcInput) {
	uint8_t low = analogInputs[adcInput].low;
	uint8_t high = analogInputs[adcInput].high;
	pos = printNumber(textId, pos, high);
	text[textId][pos++] = '.';
	if(low < 10) {
		text[textId][pos++] = '0';
	}
	return printNumberWithUnit(textId, pos, low, "V");		
}

static uint8_t printRssiLevel(uint8_t textId, uint8_t pos, const uint8_t adcInput) {
	uint8_t rssiLevel = calcRssiLevel(ANALOG_IN_1);
	return printNumberWithUnit(textId, pos, rssiLevel, "%");
}

static uint8_t printBatterLevel(uint8_t textId, uint8_t pos, const uint8_t adcInput) {
	uint8_t batterLevel = calcBatteryLevel(ANALOG_IN_1);
	return printNumberWithUnit(textId, pos, batterLevel, "%");
}

static void printDebugInfo() {
	// ---- TODO: Cleanup here! ----
	
  //snprintf(text[0], TEXT_LINE_MAX_CHARS, "%02d:%02d:%02d:%02d", hour, min, sec, tick);
	//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d%%", hour, min, sec, adc0High, adc0Low, adc1High, adc1Low, batt1);
	//if (gpsTextType != GPS_TYPE_GPGGA ) {
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, " %s", gpsFullText);
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "Part (%d): %s", gpsTextPartLength, gpsTextPart); //part
		//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsTextType);
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %06ld", gpsTextPart, gpsTime); //time
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLat); //Lat
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLong); //Long
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsFix); //fix?
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsSats); //sats
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsAltitude); //altitude
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "(%s == %d)? => %d", gpsTextPart, gpsChecksum, gpsChecksumValid); //checksum
		//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%.32s", &gpsFullText[30]);		
	//}
	//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%dV %dV %dV", analogInputsRaw[ANALOG_IN_1], analogInputsRaw[ANALOG_IN_2], analogInputsRaw[ANALOG_IN_3]);
}

static void updateText(uint8_t textId) {
  //printDebugInfo();
  uint8_t pos = 0;

  if (textId == 0) {
	  pos = printTime(textId, pos);
	  
	  pos = printAdc(textId, pos+1, ANALOG_IN_1);
	  pos = printAdc(textId, pos+1, ANALOG_IN_2);
#ifdef G_OSD	  
	  //pos = printAdc(pos+1, ANALOG_IN_3);
	  pos = printRssiLevel(textId, pos+1, ANALOG_IN_3);
#endif	  
  }
  else if (textId == 1) {
#ifdef GPS_ENABLED
		
		pos = printText(textId, pos, gpsHomePosSet ? "H-SET" : "");
		pos = printNumberWithUnit(textId, pos+1, gpsHomeDistance, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(textId, pos+1, gpsHomeBearing, "DEG");
#endif //GPS_ENABLED
	}
	else if (textId == 2) {
#ifdef GPS_ENABLED		
		pos = printNumber(textId, pos, gpsLastData.pos.latitude);
		pos = printNumber(textId, TEXT_LINE_MAX_CHARS-1-7, gpsLastData.pos.longitude);
#endif //GPS_ENABLED
	}
	else if (textId == 3) {
#ifdef GPS_ENABLED
		pos = printNumberWithUnit(textId, pos, gpsLastData.sats, "S");
		pos = printText(textId, pos+1, gpsLastData.checksumValid ? "FIX" : "BAD");
		pos = printNumberWithUnit(textId, pos+1, gpsLastData.pos.altitude, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(textId, pos+1, gpsLastData.speed, TEXT_SPEED_UNIT);
		pos = printNumberWithUnit(textId, pos+1, gpsLastData.angle, "DEG");
#endif //GPS_ENABLED
	}
	else {		
		pos = printText(textId, pos, "T:");
		pos = printText(textId, TEXT_LINE_MAX_CHARS-1-4, "V:");
		pos = printNumber(textId, pos+1, textId + 1);
	}
}

static void drawTextLine(uint8_t textId)
{
	_delay_us(3);
	uint8_t currLine = (line - textLines[textId]) / TEXT_SIZE_MULT;
	for (uint8_t i = 0; i < TEXT_LINE_MAX_CHARS; ++i) {
		if (text[textId][i] != ' ' && text[textId][i] != 0) {
			DDRB |= OUT1;
		}
		else {
			DDRB &= ~OUT1;
		}
		SPDR = textPixmap[(uint16_t)(currLine)*TEXT_LINE_MAX_CHARS + i];
		DELAY_4_NOP();
#ifndef TEXT_SMALL_ENABLED
		DELAY_6_NOP();
		DELAY_9_NOP();
#endif //TEXT_SMALL_ENABLED	
	}
	DELAY_10_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif // TEXT_ENABLED

#endif /* TEXT_H_ */
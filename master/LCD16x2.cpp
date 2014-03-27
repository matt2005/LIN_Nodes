/*
 * LCD16x2.cpp
 * 
 * Copyright 2013 OLIMEX LTD <support@olimex.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


#include "LCD16x2.h"
#include "../Wire/Wire.h"


/**
 * Default constructor.
 */
LCD16x2::LCD16x2(){
    X = 0;
    Y = 1;
}

/**
 * Default destructor.
 */
LCD16x2::~LCD16x2(){
}

/**
 * Read the id of the shield.
 * @return      If read value match BOARD_ID(0x65) the method return 1,
 * otherwise - 0.
 */
uint8_t LCD16x2::getID(){
    uint8_t id = 0;
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(GET_ID);
    Wire.endTransmission();
    Wire.requestFrom((int)ADDRESS, 1);
    while(Wire.available() > 0)
        id = Wire.read();
      
    return id;
}

void LCD16x2::lcdSetBlacklight(uint8_t value){
    Wire.beginTransmission(ADDRESS);
    Wire.write(SET_BL);
    Wire.write(value);
    Wire.endTransmission();
}
void LCD16x2::uartEnable(bool state){
    uint8_t en;
    if(state == true)
        en = 0x01;
    else
        en = 0x00;
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(UART_EN);
    Wire.write(en);
    Wire.endTransmission();
}
uint8_t LCD16x2::getFirmwareVersion(){
    uint8_t firm = 0;
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(GET_FRM);
    Wire.endTransmission();
    Wire.requestFrom((int)ADDRESS, 1);
    while(Wire.available() > 0)
        firm = Wire.read();
    
    return firm;
}
/**
 * Set direction of GPIO.
 * @param pin   The pin number according to schematic: GPIO1, GPIO2, etc...
 * @param direction     The direction of the GPIO: OUTPUT or INPUT.
 */
void LCD16x2::pinMode(uint8_t pin, uint8_t direction){
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(SET_TRIS);
    Wire.write(pin);
    Wire.write(!direction);
    Wire.endTransmission();
}

/**
 * If pin is set as output this method define the level of the GPIO.
 * If pin is input - it enables internal pull-up resistor (only available
 * for PORTB pins).
 * @param pin   The number of the desired GPIO: GPIO1, GPIO2, etc...
 * @param level The output level: HIGH or LOW
 */
void LCD16x2::digitalWrite(uint8_t pin, uint8_t level){
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(SET_LAT);
    Wire.write(pin);
    Wire.write(level);
    Wire.endTransmission();
}

/**
 * Read the state of individual GPIO, if configured as input.
 * @param pin   The number of the GPIO: GPIO1, GPIO2, etc...
 * @return      If input level is high - 1, else - 0.
 */
uint8_t LCD16x2::digitalRead(uint8_t pin){
    uint8_t port;
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(GET_PORT);
    Wire.write(pin);
    Wire.endTransmission();
    Wire.requestFrom((int)ADDRESS, 1);
    while(Wire.available() > 0)
        port = Wire.read();
    
    return port;
}

/**
 * Read the state of the 4 buttons.
 * @return      Bitmask with the 4 values: LSB - BUT1, MSB - BUT4
 */
uint8_t LCD16x2::readButtons(){
    uint8_t buttons;
    
    Wire.beginTransmission(ADDRESS);
    Wire.write(GET_BUT);
    Wire.endTransmission();
    Wire.requestFrom((int)ADDRESS, 1);
    while(Wire.available() > 0)
        buttons = Wire.read();
        
    return buttons;
}

/**
 * Clear the LCD screen.
 */
void LCD16x2::lcdClear(){
    Wire.beginTransmission(ADDRESS);
    Wire.write(LCD_CLR);
    Wire.endTransmission();
    delay(100);
}

/**
 * Position the cursor of the LCD to a given X and Y coordinates.
 * @param x     X coordinate
 * @param y     Y coordinate
 */
void LCD16x2::lcdGoToXY(uint8_t x, uint8_t y){
    if(x > 16 || x < 1)
        return;
    else
        X = x - 1;
        
    if(y > 2)
        return;
    Y = y;
}

/**
 * Write string to the LCD screen.
 * @param string        String to be written.
 */
void LCD16x2::lcdWrite(char* string){
    uint8_t len;
    uint8_t x, y;
    x = X;
    y = Y;
    
    len = strlen(string);
    for(int i = 0; i < len; i++){
        Wire.beginTransmission(ADDRESS);
        Wire.write(LCD_WR);
        Wire.write(y);
        Wire.write(x);
        Wire.write(string[i]);
        Wire.endTransmission();
        
        x++;  
        if(x > 15){
            x = 0;
            y++;
            if(y > 2)
                return;
        }
        delay(20);      
    }
}

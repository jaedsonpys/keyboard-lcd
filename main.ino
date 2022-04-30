#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int posY = 0;
int posX = 0;

int columns = 16;
int rows = 1;

bool shiftActive = false;

void setup() {
    Serial.begin(9600);
    
    // lcd setup
    lcd.init();
    lcd.backlight();
    lcd.blink();
}

void printLCD(String text, bool format=false, int x=0, int y=0) {
    if(format) {
        if(text == "ENTER") { // enter - breakline
            posX = 0; // zeroing cursor from X position
            posY = 1;

            lcd.setCursor(posX, posY);
        } else if(text == "BACK") { // backspace - delete last char
            if(posX == 0 && posY == rows) {
                posX = columns;
                posY = rows - 1;
                lcd.setCursor(posX, posY);
            } else if(posX != 0) {
                posX -= 1;
                lcd.setCursor(posX, posY);
                lcd.print(" ");
                lcd.setCursor(posX, posY);
            }
        } else if(text != "SHIFT") {
            if(shiftActive) {
                text.toUpperCase();
                shiftActive = false;

                // if(text == "1") {
                //     text = "!";
                // } else if(text == "2") {
                //     text = "@";
                // }
            }

            lcd.setCursor(posX, posY);
            lcd.print(text);
            posX++;
            
            if(posX == columns) {
                posY = 1;
                posX = 0;
            }
        }
    }
}

void loop() {
    delay(180);
}
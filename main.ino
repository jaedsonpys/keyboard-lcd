#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

String keyMap[] = {
    "0", "0", "0", "0", "0", "0", "0", "0",
    "0", "0", "0", "0", "0", "0", "`", "0",
    "0", "0", "SHIFT", "0", "0", "q","1", "0",
    "0", "0", "z","s","a","w","2", "0", //31
    "0","c","x","d","e","4","3", "0",
    "0"," ","v","f","t","r","5", "0", // 47
    "0","n","b","h","g","y","6", "0",
    "0", "0", "m","j","u","7","8", "0", // 63
    "0",",","k","i","o","0","9", "0",
    "0",".","/","l",";","p","-", "0",
    "0", "0","\"", "0","[", "=", "0", "0",
    "0", "0", "ENTER", "]", "0", "\\", "0", "0",
    "0", "0", "0", "0", "0", "0", "BACK", "0",
    "0","1", "0","4","7", "0", "0", "0",
    "0",".","2","5","6","8", "0", "0",
    "0","+","3","-","*","9", "0", "0",
    "0", "0", "0", "0"
};

LiquidCrystal_I2C lcd(0x27, 16, 2);

int posY = 0;
int posX = 0;

int columns = 16;
int rows = 1;

bool shiftActive = false;

const int CLOCK = 2;
const int DATA = 3;

int lastscan = 0;

void setup() {
    Serial.begin(9600);
    
    // lcd setup
    lcd.init();
    lcd.backlight();
    lcd.blink();

    // usb setup
    pinMode(CLOCK, INPUT_PULLUP);
    pinMode(DATA, INPUT_PULLUP);
}

int listen() {
    int scanval = 0;

    for(int i = 0; i < 11; i++) {
        while(digitalRead(CLOCK));
        scanval |= digitalRead(DATA) << i;
        while(digitalRead(CLOCK) == LOW);
    }

    scanval >>= 1;
    scanval &= 0xFF;
    lastscan = scanval;

    if(lastscan != 0xF0 && scanval != 0xF0) {
        if(keyMap[scanval] == "SHIFT") {
            shiftActive = true;
        }

        // Serial.print("scanval: ");
        // Serial.println(scanval);
        return scanval;
    }
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
    // getting keyboard pressed key code
    int keycode = listen();
    
    printLCD(keyMap[keycode], true);
    delay(180);
}
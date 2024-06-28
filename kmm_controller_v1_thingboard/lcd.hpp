#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C display(0x3F, 20, 4);
 // Array to hold the current content of each line (if needed)
String lines[4] = {"", "", "", ""};

void setupLcd()
{
// initialize the LCD
display.init();
 
// Turn on the blacklight and print a message.
display.backlight();
display.setCursor(5, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
display.print("Welcome To");
 
display.setCursor(3, 1); // ไปที่ตัวอักษรที่ 6 แถวที่ 2
display.print("Royal Smart Farm");

display.setCursor(1, 2); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
display.print("www.komomi.net");
 
display.setCursor(1, 3); // ไปที่ตัวอักษรที่ 2 แถวที่ 2
display.print("R O Y A L - P R O J");
}

void deleteLine(int lineNumber) {
  if (lineNumber < 0 || lineNumber >= 4) {
    Serial.println("Invalid line number");
    return;
  }
  
  // Clear the specified line by setting its content to an empty string
  lines[lineNumber] = "";
  
  // Clear the entire display
  // display.clear();
  
  // Redraw the remaining lines
  for (int i = 0; i < 4; i++) {
    display.setCursor(0, i);
    display.print(lines[i]);
  }
}

void printLine(int col, int line, String msg) {
  if (line < 0 || line >= 4 || col < 0 || col >= 20) {
    Serial.println("Invalid column or line number");
    return;
  }
  
  // Store the message in the corresponding line of the array
  lines[line] = msg;

  // Set the cursor to the specified position and print the message
  display.setCursor(col, line);
  display.print(msg);
}

  // deleteLine(1);
  // printLine(1,3,WiFi.localIP().toString());
#include <LiquidCrystal.h>
char buf[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'}; // The Message
int size = (sizeof(buf)); // How many characters in the message array
int maxIterations = 10;  // How many times we generate a random number 

// Initialize the LCD component 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);  // Initialize LCD size(16 columns, 2 rows)
  writeMessage(0, buf); // Call the writeMessage method with LCD row 0 and buf for the message as parameters
}

void writeMessage(int row, char message[]){
    for(int i = 0; i < size; i++){  // Loop through each column
      for(int j = 0; j < maxIterations; j++){ // Loop through amount of maxIterations 
        for(int k = i; k < size; k++){  // Loop for generating random numbers, starts at i(the column we are on)
          lcd.setCursor(k, row); // Go column k at row
          int r = rand() % 9; // Generate random number 0-9
          lcd.print(r); // Print that number
        }
      }
    delay(50); // Give small delay so it's not instant
    lcd.setCursor(i, row); // Set the cursor location at i at row
    lcd.print(message[i]); // Print the character at array item i
  }
  delay(1000); // Leave message for 1000 ms
}

void loop() {

}

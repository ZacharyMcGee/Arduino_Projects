#include <LiquidCrystal.h>
char buf[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'}; // The Message
int size = (sizeof(buf)); // How many characters in the message array
int maxIterations = 10;  // How many times we generate a random number 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  writeMessage(0, buf);
}

void writeMessage(int row, char message[]){
    for(int i = 0; i < size; i++){
      for(int j = 0; j < maxIterations; j++){
        for(int k = i; k < size; k++){
          lcd.setCursor(k, row);
          int r = rand() % 9;
          lcd.print(r);
        }
      }
    delay(50);
    lcd.setCursor(i, row);
    lcd.print(message[i]);
  }
  delay(1000);
}

void loop() {

}

#include <LiquidCrystal.h> // LCD Screen Library
#include "IRremote.h" // Wireless Remote Library

const char *strings[] = {"Done", "Starting", "Swap", "Good", "Enter Numbers:", "Start Sort: PWR"}; // Strings to display

const int receiver = 13, rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Pins
int amtOfNums = 0; // We are going to recieve 8 numbers, this counts how many have been entered, start at 0
int arr2[8];

IRrecv irrecv(receiver);  
decode_results results;

int speed = 600; // The speed of each step, 0 is no delay, 1000 is 1 second
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int translateIR() { // For receiving wireless input
  switch(results.value){
    case 0xFFA25D: return 10;   // Power button
                   break;
    case 0xFF6897: return 0;    // 0 button
                   break;
    case 0xFF30CF: return 1;    // 1 button
                   break;
    case 0xFF18E7: return 2;    // 2 button
                   break;
    case 0xFF7A85: return 3;    // 3 button
                   break;
    case 0xFF10EF: return 4;    // 4 button
                   break;
    case 0xFF38C7: return 5;    // 5 button
                   break;
    case 0xFF5AA5: return 6;    // 6 button
                   break;
    case 0xFF42BD: return 7;    // 7 button
                   break;
    case 0xFF4AB5: return 8;    // 8 button
                   break;
    case 0xFF52AD: return 9;    // 9 button
                   break;

    default: break;
  }
  delay(1000); 
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); 

  lcd.begin(16, 2);  
  writeMessage(0, 0, strings[4]);  // Begin with "Enter Numbers:" String
}

void swap(int *i, int *j) // The swap function
{ 
    int temp = *i; // Make a temp int and assign to i
    *i = *j; // Make int i the value of j
    *j = temp; // Make int j the temp(i copy)
} 

void bubbleSort(int arr[], int n) 
{ 
   writeMessage(0, 1, strings[1]);
   clearRow(0);
   printArray(arr, n);
   delay(speed);
   clearRow(1);
   delay(speed);
   for (int i = 0; i < n-1; i++){          // Loop size n times
       for (int j = 0; j < n-i-1; j++){    // We can stop at n-i-1 because the last elements have been sorted
           writeInt(j, 1, arr[j]);         // We write the first number to compare at row 1 
           writeInt(j + 1, 1, arr[j + 1]); // Write the second number to compare at row 1
           delay(speed); 
           if (arr[j] > arr[j+1]) {        // Check if the left item is greater than the right item 
              writeMessage(j + 3, 1, strings[2]);
              delay(speed);
              clearRow(1);
              swap(&arr[j], &arr[j+1]);   // Swap them
              writeInt(j, 1, arr[j]);
              writeInt(j + 1, 1, arr[j + 1]);
              delay(speed); 
              printArray(arr, n);        // Display new array after swap
              delay(speed);
           }       
           else
           {
             writeMessage(j + 3, 1, strings[3]);
             delay(speed);
           }
           clearRow(1);
       }
   }
  writeMessage(0, 1, strings[0]);
} 

void printArray(int arr[], int size){  // Print out the array to row 0
  for(int i = 0; i < size; i++){
    lcd.setCursor(i, 0);
    lcd.print(arr[i]);
  }
}

void writeInt(int column, int row, int message) {  // Print a single int
  lcd.setCursor(column, row);
  lcd.print(message);
}

void clearInt(int column, int row){ // Clear a single int
  lcd.setCursor(column, row);
  lcd.print(' ');
}

void clearRow(int row){ // Clear a whole row
  lcd.setCursor (0, row);
  for (int i = 0; i < 16; i++)
  {
    lcd.print(' ');
  }
}

void writeMessage(int column, int row, char message[]){ // Display a string on the LCD
  int size = strlen(message);
  for(int i = 0; i < size; i++) {
    lcd.setCursor(column++, row);
    lcd.print(message[i]);
  }
}

void loop() {
  if (irrecv.decode(&results)) // We got an input from the remote
  {
    int num = translateIR(); // Return the value to int num
    
    if(amtOfNums < 8 && num < 10){ // If we are under 8 numbers and the returned value is under 10(power button)
      if(amtOfNums == 0){  // If this is our first number that has been entered
        clearRow(0); // Remove the string currently displayed so we can put the numbers there
      }   
      arr2[amtOfNums] = num; // Add the number to the array
   
      lcd.setCursor(amtOfNums, 0); 
      lcd.print(arr2[amtOfNums]); // Print the number we just got 
      amtOfNums++; // Increment the counter
    }
      if(num == 10){ // If the value return was code for the power button(which starts the sorting)
        clearRow(0); // Clear row 0
        clearRow(1); // Clear row 1
        int n = sizeof(arr2)/sizeof(arr2[0]); // Get the amount of elements in the array
        bubbleSort(arr2, n); // Start bubble sort on arr2 of size n
        delay(10000); // Display results for 10 seconds
        clearRow(0); // Clear row 0
        clearRow(1); // Clear row 1
        amtOfNums = 0; // Make the amount of nums 0 again for repeat usage
        for(int i = 0; i < n; i++){ // Make the array items 0
          arr2[i] = 0;
        }
        writeMessage(0, 0, strings[4]); // Write message 
      }
   delay(200);
   irrecv.resume();
  }
  if(amtOfNums >= 8){ // We have hit 8 numbers
      writeMessage(0, 1, strings[5]);  // Prompt user to hit the power button to start
  }
}

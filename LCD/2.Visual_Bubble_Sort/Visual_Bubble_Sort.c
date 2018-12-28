#include <LiquidCrystal.h>
#include "IRremote.h"

const char *strings[] = {"Done", "Starting", "Swap", "Good", "Enter Numbers:", "Start Sort: PWR"};

const int receiver = 13, rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int amtOfNums = 0;
int arr2[8];

IRrecv irrecv(receiver);  
decode_results results;

int speed = 600;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int translateIR() {
  switch(results.value){
    case 0xFFA25D: return 10; 
                   break;
    case 0xFF6897: return 0;    
                   break;
    case 0xFF30CF: return 1;   
                   break;
    case 0xFF18E7: return 2;    
                   break;
    case 0xFF7A85: return 3;    
                   break;
    case 0xFF10EF: return 4;    
                   break;
    case 0xFF38C7: return 5;    
                   break;
    case 0xFF5AA5: return 6;    
                   break;
    case 0xFF42BD: return 7;    
                   break;
    case 0xFF4AB5: return 8;    
                   break;
    case 0xFF52AD: return 9;    
                   break;

    default: break;
  }
  delay(1000); 
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); 

  lcd.begin(16, 2);  
  writeMessage(0, 0, strings[4]);
}

void swap(int *i, int *j) 
{ 
    int temp = *i; 
    *i = *j; 
    *j = temp; 
} 

void bubbleSort(int arr[], int n) 
{ 
   writeMessage(0, 1, strings[1]);
   clearRow(0);
   printArray(arr, n);
   delay(speed);
   clearRow(1);
   delay(speed);
   for (int i = 0; i < n-1; i++){        
       for (int j = 0; j < n-i-1; j++){
           writeInt(j, 1, arr[j]);
           writeInt(j + 1, 1, arr[j + 1]);
           delay(speed); 
           if (arr[j] > arr[j+1]) {
              writeMessage(j + 3, 1, strings[2]);
              delay(speed);
              clearRow(1);
              swap(&arr[j], &arr[j+1]); 
              writeInt(j, 1, arr[j]);
              writeInt(j + 1, 1, arr[j + 1]);
              delay(speed); 
              printArray(arr, n);
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

void printArray(int arr[], int size){
  for(int i = 0; i < size; i++){
    lcd.setCursor(i, 0);
    lcd.print(arr[i]);
  }
}

void writeInt(int column, int row, int message) {
  lcd.setCursor(column, row);
  lcd.print(message);
}

void clearInt(int column, int row){
  lcd.setCursor(column, row);
  lcd.print(' ');
}

void clearRow(int row){
  lcd.setCursor (0, row);
  for (int i = 0; i < 16; i++)
  {
    lcd.print(' ');
  }
}

void writeMessage(int column, int row, char message[]){
  int size = strlen(message);
  for(int i = 0; i < size; i++) {
    lcd.setCursor(column++, row);
    lcd.print(message[i]);
  }
}

void loop() {
  if (irrecv.decode(&results)) 
  {
    int num = translateIR();
    
    if(amtOfNums < 8 && num < 10){
      if(amtOfNums == 0){
        clearRow(0);
      }   
      arr2[amtOfNums] = num; 
   
      lcd.setCursor(amtOfNums, 0);
      lcd.print(arr2[amtOfNums]);
      amtOfNums++;
    }
      if(num == 10){
        clearRow(0);
        clearRow(1);
        int n = sizeof(arr2)/sizeof(arr2[0]); 
        bubbleSort(arr2, n);
        delay(10000);
        clearRow(0);
        clearRow(1);
        amtOfNums = 0;
        for(int i = 0; i < n; i++){
          arr2[i] = 0;
        }
        writeMessage(0, 0, strings[4]);
      }
   delay(200);
   irrecv.resume();
  }
  if(amtOfNums >= 8){
      writeMessage(0, 1, strings[5]);  
  }
}

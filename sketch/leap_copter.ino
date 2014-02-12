// include the library code:
#include <LiquidCrystal.h>
#include <SPI.h>

int serialInput = 0;
String inString = "";

int selectY = 10;
int selectX = 9;
int selectZ = 8;
int MOSIpin = 11;
boolean enabled = false;
boolean power = false;
int fingers = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


void printCursor(int t){   
  for(int r = 0; r < t && r < 4; r++)
    lcd.print("\x80");   
  lcd.print("\x92");
  for(int r = t; r < 4 ; r++)
    lcd.print("\x80"); 
}

void spiWrite(int cs, int val){
  digitalWrite(cs, LOW);
  SPI.transfer(val);
  digitalWrite(cs, HIGH);
}

char serialReadString() {
  char mode;
  int i, t, h;
  boolean started = false;
  
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    
    if (inChar == ']' && started) {
      started = false;
      i = inString.toInt();
      
      switch (mode){
        case 'y':    
          t = map(constrain(i, 100, 500), 100, 500, 240, 128);
          
          (fingers >= 2)?
            spiWrite(selectY, t):
            spiWrite(selectY, 255);
            
          lcd.setCursor(2, 0);
          printCursor(map(t, 240, 128, 0, 4));
          break;
        
        case 'x':
          t = map(constrain(i, -150, 150), -150, 150, 255, 128);
          (enabled) ? spiWrite(selectX, t) : spiWrite(selectX, 179);
          lcd.setCursor(10, 1);
          printCursor(map(t, 240, 128, 0, 4));
          break;
        
        case 'z':
          t = map(constrain(i, -50, 100), -50, 100, 150, 200);
          (enabled) ? spiWrite(selectZ, t) : spiWrite(selectZ, 179);
          lcd.setCursor(2, 1);
          printCursor(map(t, 200, 160, 0, 4));
          break;
          
        case 'f':
          lcd.setCursor(10, 0);
          lcd.print(i);
          fingers = i;
          
          if(i > 3){
            lcd.print(" On  ");
            enabled = true;
          } else {
            lcd.print(" Off ");
            enabled = false;
          }            
          break;
       
       case 'e':
          spiWrite(selectY, 255);
          break;
          
        default:
          lcd.setCursor(16, 1);
      }

      inString = ""; 
      
      
    } else {
      switch(inChar){
        case 'y':
        case 'x':
        case 'z':
        case 'f':
        case 'e':
          mode = inChar;
          break;
        
        case '[':
          started = true;
          break;
           
        default:
          inString += (char)inChar; 
      }
    }
  }
}


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Y\x5B\x92\x80\x80\x80\x80\x5D");
  lcd.setCursor(8,0);
  lcd.print("F:2 Off ");
  lcd.setCursor(0,1);
  lcd.print("Z\x5B\x80\x80\x92\x80\x80\x5D");
  lcd.print("X\x5B\x80\x80\x92\x80\x80\x5D");
  
  SPI.begin(); 
  pinMode(MOSIpin, OUTPUT);
  pinMode (selectX, OUTPUT);
  digitalWrite(selectX,HIGH); 
  pinMode (selectY, OUTPUT);
  digitalWrite(selectY,HIGH); 
  pinMode (selectZ, OUTPUT);
  digitalWrite(selectZ,HIGH); 
  
  spiWrite(selectZ, 179);
  spiWrite(selectX, 179);
  
}

void loop() {
 
  
  serialReadString();
  delay(50);

}


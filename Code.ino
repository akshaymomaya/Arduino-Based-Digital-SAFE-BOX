
/* Electronic  Safe
 *  by Akshay Momaya
 *  for Mission Critical ( youtube )
 *  subscribe for more arduino projects

https://www.youtube.com/channel/UCM6rbuieQBBLFsxszWA85AQ?sub_confirmation=1
 *  
 *  
 */

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);

char password[4];

char pass[4],pass1[4];

int i=0;

char customKey=0;

const byte ROWS = 4; //four rows

const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int lock = 13;
int buzzer = 10;

void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  

  lcd.init();
  lcd.backlight();
  pinMode(lock, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.print("  Mission safe ");
  Serial.print("  Mission safe ");
  lcd.setCursor(0,1);
  lcd.print("  Keypad Lock ");
  Serial.print("  Keypad Lock ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter password:");
  Serial.println("Enter password:");
  lcd.setCursor(0,1);
  for(int j=0;j<4;j++)
  EEPROM.write(j, j+48);
  for(int j=0;j<4;j++)
  pass[j]=EEPROM.read(j);

}

void loop()
{
  customKey = customKeypad.getKey();
  if(customKey=='#')
  change();
  if (customKey)
  {
    password[i++]=customKey;
    lcd.print(customKey);
    Serial.print(customKey);
    beep();
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
    if(!(strncmp(password, pass,4)))
    {
      digitalWrite(lock, HIGH);
      beep();
      lcd.clear();
      lcd.print(" Acess  Granted");
      Serial.println(" Acess  Granted");
      digitalWrite(11, LOW);
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("# Change Code");
      Serial.println("# Change Code");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      Serial.println("Enter Password:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(lock, LOW);
    }
    else
    {
      digitalWrite(buzzer, HIGH);
      lcd.clear();
      lcd.print("Access Denied...");
      Serial.println("Access Denied...");
      lcd.setCursor(0,1);
      lcd.print("#Change PassCode");
      Serial.println("#Change PassCode");
      delay(2000);
      lcd.clear();
      lcd.print("Enter PassCode:");
      Serial.println("Enter PassCode:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(buzzer, LOW);
    }
  }
}
void change()
{
  int j=0;
  lcd.clear();
  lcd.print("UR Current Code");
  Serial.println("UR Current Code");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      Serial.print(key);
      beep();
    }
    key=0;
  }
  delay(500);

  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("Wrong PassCode...");
    Serial.println("Wrong PassCode...");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    Serial.println("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("Enter New Code:");
    Serial.println("Enter New Code:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass[j]=key;
        lcd.print(key);
        Serial.print(key);
        EEPROM.write(j,key);
        j++;
        beep();
      }
    }
    lcd.print(" - Accepted");
    Serial.println(" Done......");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Ur Code:");
  Serial.println("Enter Ur Code:");
  lcd.setCursor(0,1);
  customKey=0;
}
void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}

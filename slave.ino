/*Kelompok 7 Sistem Siber-Fisik dan Praktikum 02

	Febriana Pasonang Sidauruk
    Hadina Widyasari
    Salma Shafira Khairaldin
    Zahra Meisya Syumaya
*/

//SLAVE 

#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include<Wire.h>    
#include <avr/sleep.h> 
#define interruptPin 2

LiquidCrystal lcd(A0,A1,A2,A3,12,11);
volatile byte buttonPressType = 0;
char password[4];
char pass_awal[4];

int i=0;
int n = 0;
int value = 0;
int door_pin = 10;

char key_pressed=0;
const byte rows = 4; 
const byte columns = 4; 
char hexaKeys[rows][columns] = {

{'1','2','3','A'},

{'4','5','6','B'},

{'7','8','9','C'},

{'*','0','#','D'}

};

byte row_pins[rows] = {9,8,7,6};
byte column_pins[columns] = {5,4,3,13};   

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
int wrong = 0;


void setup()

{
  Serial.begin(9600);                        //komunikasi serial dengan 9600 bps
  pinMode(door_pin,OUTPUT);                       //door_pin yang mengartikan apakah pintu dibuka atau tidak
  pinMode(interruptPin,INPUT_PULLUP);		//mendefinisikan interruptPin sebagai input pullup
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Alarm AntiMaling");
  delay(5000);
  lcd.clear();
  
  passwordAwal();

}

void passwordAwal(){  //function untuk membaca password yang disimpan dalam EEPROM

  for(int j=0;j<4;j++)

    EEPROM.write(j, j+49);

  for(int j=0;j<4;j++)

    pass_awal[j]=EEPROM.read(j);
} 


void loop()

{
  delay(1000); 
  gonna_sleep(); //memanggil function gonna_sleep
}

void gonna_sleep(){
  sleep_enable();
   attachInterrupt(digitalPinToInterrupt(interruptPin), wakeUp, CHANGE); //interupsi terjadi ketika pin dalam mode CHANGE
   set_sleep_mode(SLEEP_MODE_PWR_DOWN); //diatur untuk sleep
   delay(1000);
   sleep_cpu();
}

void wakeUp(){
  lcd.clear();
  lcd.print("Alarm Aktif"); //ketika diinterupsi maka alarm aktif
  delay(3000);
  sleep_disable();
  Wire.begin(8);  
  Wire.onReceive(receiveEvent); //memanggil fungsi receiveEvent untuk meminta data dari Master
  Wire.onRequest(requestEvent); //memanggil fungsi requestEvent ketika Master meminta data dari Slave
  detachInterrupt(0);
  }

void receiveEvent (int howMany) {
  int x = Wire.read(); //membaca data
  lcd.clear();
  
  if (x==1 && value == 0){
  lcd.print("Password: ");
  digitalWrite(door_pin, LOW);
  key_pressed = keypad_key.getKey();

  if (key_pressed)
  {
   hide();
   }

  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      pass_awal[j]=EEPROM.read(j);
    if(!(strncmp(password, pass_awal,4))) //password yang dimasukkan dibandingkan dengan password pada EEPROM
    {
      lcd.clear();
      lcd.print("Password benar");
      digitalWrite(door_pin, HIGH);
      delay(10000);

      lcd.clear();
    
      lcd.setCursor(0,0);
      lcd.print("Pintu terbuka"); 
      delay(100000);
      digitalWrite(door_pin,LOW);
      wrong=0;
      i=0;
    }

    else
    {
      lcd.clear();
      lcd.print("PASSWORD SALAH");
      delay(10000);
      lcd.clear();
      wrong++;
       i = 0;
       if(wrong == 3){
            i = 0;
            n = 1;
          digitalWrite(door_pin, LOW);
          value = 1;
          }
    }
  }
  }
}

void requestEvent()                            
{
  if(n == 1) {
    lcd.setCursor(0,0);
      lcd.print("WARNING!!!");
     int a = 1;
  Wire.write(a);
    n = 0;
  }
}

void hide(){ //key yang ditekan disimpan dalam variabel password
password[i]=key_pressed;
    lcd.setCursor(i,1);
    lcd.print("*");
  i++;
}
/*Kelompok 7 Sistem Siber-Fisik dan Praktikum 02

	Febriana Pasonang Sidauruk
    Hadina Widyasari
    Salma Shafira Khairaldin
    Zahra Meisya Syumaya
*/

//MASTER

//mendefine masing-masing not berdasarkan frekuensi
#define si0 330
#define do1 370
#define re  415
#define mi  440
#define fa  494
#define sol 554
#define la  587
#define si  659
#define do2 740
#define re2 831
#define mi2 880

#include<Wire.h>                      
#define buttonpin 10
#define LED 9

int x =0;
int n = 1;
const int piezo = 6;

int melody[] = {
  do1, do1, sol, fa, mi, re, re, re, fa, mi,
  re, do1, do1, mi2, re2, mi2, re2,
  mi2, do1, do1, mi2, re2, mi2, re2, mi2,
  mi, mi, mi, mi, mi, mi, sol, sol, sol, sol, sol, sol,
  fa, fa, fa, fa, fa, fa, si, si, si, si, si, si,
  do2, do2, do2, do2, do2, do2, do2, do2, do2, do2, do2, do2
};


int noteDurations[] = {
  6, 8, 8, 6, 6, 6, 8,
  8, 6, 8, 8, 6, 8, 8,
  8, 8, 8, 8, 6, 8, 8, 8, 8, 8, 8,
  16, 16, 8, 16, 16, 8, 16, 16, 8, 16, 16, 8,
  16, 16, 8, 16, 16, 8, 16, 16, 8, 16, 16, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

int lengthOfMelody = sizeof(melody)/sizeof(melody[0]);

void setup() 
{ 
  
  Serial.begin(9600);                  
  pinMode(buttonpin,INPUT);            
  pinMode(LED,OUTPUT);                 
  Wire.begin();                        
}

void loop()
{ 
  int pinvalue = digitalRead(buttonpin);    
  if(pinvalue==HIGH)                         
    {
      x=1;
    }
    
  else
   {
      x=0;
   }
  
  Wire.beginTransmission(8);                 
  Wire.write(x);                             
  Wire.endTransmission();                    
  delay(500);
  
  Wire.requestFrom(8, 1);               
    
  int a = Wire.read();
    if (a==1)
      {
      digitalWrite(LED,HIGH);
    for (int thisNote = 0; thisNote < lengthOfMelody; thisNote++) {
    int noteDuration = 1400/noteDurations[thisNote];
    int pauseBetweenNotes;
    tone(6, melody[thisNote],noteDuration);
    if(noteDurations[thisNote]==6){
      pauseBetweenNotes = noteDuration * 1.95;
      delay(pauseBetweenNotes);
    }
    else{
      pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
    }
    noTone(6);
  }
  }
      delay(1000);
      
   digitalWrite(LED,LOW);
 }


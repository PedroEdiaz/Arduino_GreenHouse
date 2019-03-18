#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

LiquidCrystal_I2C lcd(0x3f,2,16);
DHT11 dht11(2);

float temp, hum, thum, leds, ints;
int s,m,h,n;

byte Gota[8] = {               
  B00100,B00100,B01010,B01010,
  B10001,B10001,B10001,B01110,
};

byte Term[8] = {
  B00100,B01010,B01010,B01010,
  B01110,B01110,B01110,B00100
};

void setup() {
  lcd.init();               
  lcd.setBacklight(1);
  lcd.createChar (0,Gota);  
  lcd.createChar (1,Term);   
  pinMode(13 , OUTPUT);     //Ventilador
  pinMode(10 , OUTPUT);     //LED
  pinMode( 7 , OUTPUT);     //Bomba
  h=8;m=0;s=0;
}

void loop() {
  //Sensores
  thum = analogRead(A0);
  dht11.read(hum, temp);

  //Reloj
  if (s>=60){s-=60;m+=1;}
  if (m>=60){m-=60;h+=1;}
  if (h>=24){h-=24;}   

  //Simular día
  //leds=(s*s-30*s)/(-2.5);  //en un minuto
  leds=(h*h-24*h+108)/(-.36); //en un día
  if (leds<0){leds=0;}

  //Cambiar pantalla cada 5 segundos
  if (s%5 ==0) {lcd.clear();}
  if (s%15>=0  and s%15 <5 ){reloj();}
  if (s%15>=5  and s%15 <10){temperature();}
  if (s%15>=10){humedad();}

  //Regular Clima
  if (temp>28) {
    digitalWrite(13, HIGH);
    analogWrite(10,leds*32/5);
    }
  else {
    digitalWrite(13, LOW);
    analogWrite(10,leds*256/25);
    }
  if (thum<800 or hum>50) 
  {digitalWrite(7, HIGH);}
  else{digitalWrite(7, LOW);}

  //Esperar un segundo
  s+=1;delay(1000);
  }

//Mostrar en LCD
void humedad() {
  lcd.setCursor ( 5, 0 );lcd.print ("Tierra");
  lcd.setCursor ( 4, 1 );lcd.write (0); 
  lcd.setCursor ( 1, 1 );lcd.print (1-(thum*25/256));
}

void temperature() {   
  lcd.setCursor ( 4 , 0 );lcd.print("Ambiente");  
  lcd.setCursor ( 9 , 1 );lcd.write (0);    
  lcd.setCursor ( 10, 1 );lcd.print(hum);    
  lcd.setCursor ( 1 , 1 );lcd.write (1); 
  lcd.setCursor ( 2 , 1 );lcd.print(temp);
}

void reloj() {
  lcd.setCursor ( 2, 0 );lcd.print ("Reloj Virtual");
  lcd.setCursor ( 4, 1 );lcd.print ("00:00:00");
  if (h<10) {n=1;} else {n=0;}
  lcd.setCursor ( 4+n, 1 );lcd.print (h); 
  if (m<10) {n=1;} else {n=0;}
  lcd.setCursor ( 7+n, 1 );lcd.print (m);
  if (s<10) {n=1;} else {n=0;}
  lcd.setCursor ( 10+n, 1 );lcd.print (s);
}

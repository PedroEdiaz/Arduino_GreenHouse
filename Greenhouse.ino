#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

LiquidCrystal_I2C lcd(0x3f,2,16);
DHT11 dht11(2);

float temp, hum, shum, leds;
int s,m,h,n;

byte Drop[8] = {               
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
  lcd.createChar (0,Drop);  
  lcd.createChar (1,Term);   
  pinMode(13 , OUTPUT);     //Cooler
  pinMode(10 , OUTPUT);     //LED
  pinMode( 7 , OUTPUT);     //Pump
  h=8;m=0;s=0;
}

void loop() {
  //Sensors
  shum = analogRead(A0);
  dht11.read(hum, temp);

  //Clock
  if (s>=60){s-=60;m+=1;}
  if (m>=60){m-=60;h+=1;}
  if (h>=24){h-=24;}   

  //Sim a day
  //leds=(s*s-30*s)/(-2.5);  //In a minute
  leds=(h*h-24*h+108)/(-.36); //In a day
  if (leds<0){leds=0;}

  //Change Display every 5 seconds
  if (s%5 ==0) {lcd.clear();}
  if (s%15>=0  and s%15 <5 ){clock();}
  if (s%15>=5  and s%15 <10){temperature();}
  if (s%15>=10){humedad();}

  //Regulate room
  if (temp>28) {
    digitalWrite(13, HIGH);
    analogWrite(10,leds*32/5);
    }
  else {
    digitalWrite(13, LOW);
    analogWrite(10,leds*256/25);
    }
  if (shum<800 or hum>50) 
  {digitalWrite(7, HIGH);}
  else{digitalWrite(7, LOW);}

  //Esperar un segundo
  s+=1;delay(1000);
  }

//Mostrar en LCD
void humedad() {                      
  lcd.setCursor ( 6, 0 );lcd.print ("Soil");
  lcd.setCursor ( 4, 1 );lcd.write (0); 
  lcd.setCursor ( 1, 1 );lcd.print (1-(shum*25/256));
}

void temperature() {   
  lcd.setCursor ( 3 , 0 );lcd.print("Environment");  
  lcd.setCursor ( 9 , 1 );lcd.write (0);    
  lcd.setCursor ( 10, 1 );lcd.print(hum);    
  lcd.setCursor ( 1 , 1 );lcd.write (1); 
  lcd.setCursor ( 2 , 1 );lcd.print(temp);
}

void clock() {
  lcd.setCursor ( 2, 0 );lcd.print ("Virtual Clock");
  lcd.setCursor ( 4, 1 );lcd.print ("00:00:00");
  if (h<10) {n=1;} else {n=0;}
  lcd.setCursor ( 4+n, 1 );lcd.print (h); 
  if (m<10) {n=1;} else {n=0;}
  lcd.setCursor ( 7+n, 1 );lcd.print (m);
  if (s<10) {n=1;} else {n=0;}
  lcd.setCursor ( 10+n, 1 );lcd.print (s);
}

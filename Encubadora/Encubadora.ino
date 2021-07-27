#include <Wire.h>    //Para pantalla LED
#include <LiquidCrystal_I2C.h> //PantallaLED
#include <DHT.h> //Sensor Temp/Hum
#include <DHT_U.h>
#include <Servo.h> //Servo



int sensor=3;
int ventilador=6;
int calor=5;
int Buttom = 1;
int PINSERVO = 7;   // pin 7 conectado a señal del servo

int PULSOMIN = 760;    // pulso minimo en microsegundos
int PULSOMAX = 2420;    // pulso maximo en microsegundos

int temperatura;
int humedad;
String first, second;

int espera=0;
int grados=90;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(sensor, DHT11);
Servo servo1; 

void setup(){
  
  servo1.attach(PINSERVO, PULSOMIN, PULSOMAX);
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  dht.begin();
  pinMode(sensor, INPUT);
  pinMode(ventilador, OUTPUT);
  pinMode(Buttom, INPUT);
  
}

void loop() {
  if (digitalRead(Buttom)==HIGH){ //Modo de Incubación
    temperatura=dht.readTemperature();
    humedad=dht.readHumidity();
    first =(temperatura);
    second = (humedad);
    Serial.println(temperatura);
    while (temperatura<37.8){
      temperatura=dht.readTemperature();
      digitalWrite(ventilador,HIGH);
      digitalWrite(calor,HIGH);
      delay(5000);
      espera+=5000;
      digitalWrite(calor,LOW);
    }
    if (espera >= 3600000){
      if (grados>0){
        for(grados;grados>=0;grados-=3){
          servo1.write(grados);
          delay(200);
          Serial.println(grados);
        }
      }
      else{
        for(grados;grados<=180;grados+=3){
          servo1.write(grados);
          delay(200);
          Serial.println(grados);
        }
      }
      espera=0;
    }
    lcd.clear();
    lcd.print("Temperatura: "+ first);
    lcd.setCursor(0,1);
    lcd.print("Humedad: " + second);
    delay(1000);
    espera+=1000;
    digitalWrite(ventilador,LOW);
  }
  
  if (digitalRead(Buttom)==LOW){ //Modo de Eclosión
    servo1.write(90);
    temperatura=dht.readTemperature();
    humedad=dht.readHumidity();
    first =(temperatura);
    second = (humedad);
    Serial.println(temperatura);
    while (temperatura<36.5){
      temperatura=dht.readTemperature();
      digitalWrite(ventilador,HIGH);
      digitalWrite(calor,HIGH);
      delay(5000);
      digitalWrite(calor,LOW);
    }

    //No hay vueltas en este modo
    
    lcd.clear();
    lcd.print("Temperatura: "+ first);
    lcd.setCursor(0,1);
    lcd.print("Humedad: " + second);
    delay(1000);
    espera+=1000;
    digitalWrite(ventilador,LOW);
  }
}

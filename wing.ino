#include <Servo.h>
#include <Wire.h>
#include <MMA8453_n0m1.h>

Servo left;
Servo right;
MMA8453_n0m1 accel;

int potentiometer = A0;
int sw1 = 8;
int sw2 = 7;

int setZero=8;
int setMax=45;

int respond = 5;

void setup(){
  accel.setI2CAddr(0x1C);
  accel.dataMode(true, 2);
  left.attach(11);
  right.attach(10);
  Serial.begin(9600);
  pinMode(potentiometer, INPUT);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
}

void angle(int left_angle,int right_angle){
  left.write(left_angle);
  right.write(180-right_angle);
}

String mode = "";
int logPotentio=0;
int logAccel=0;
void loop(){
  int potentio = analogRead(potentiometer);
  int rsw1 = digitalRead(sw1);  // on - off
  int rsw2 = digitalRead(sw2);  // auto - manual
  if(rsw1==1){
    if(rsw2==1){
      if(mode!="auto"){
         Serial.println(" Auto");
         mode="auto";
         logAccel=0;
      }
      accel.update();
      int adj=map(accel.x(),-240,240,0,180);
      if(logAccel!=adj&&(adj>logAccel+respond||adj<logAccel-respond)){
        logAccel=adj;
        if(adj>85){
          int limit=map(adj,95,180,setZero,setMax);
          angle(limit,setZero);
          Serial.print("Right: ");
          Serial.print(adj);
          Serial.print("/");
          Serial.println(limit);
        }else if(adj<95){
          int limit=(setMax-map(adj,0,85,setZero,setMax));
          angle(setZero,limit);
          Serial.print("Left: ");
          Serial.print(adj);
          Serial.print("/");
          Serial.println(limit);
        }else{
          angle(setZero,setZero);
          Serial.println("Center");
        }
      }
    }else{
      if(mode!="manual"){
         Serial.println("Manual");
         mode="manual";
         logPotentio=0;
      }
      int adj=map(potentio,48,4000,setZero,setMax);
      if(logPotentio!=adj&&(adj>logPotentio+respond||adj<logPotentio-respond)){
        logPotentio=adj;
        Serial.print(adj);
        Serial.print("/");
        Serial.println(potentio);
        angle(adj,adj);
      }
    }
  }else{
    if(mode!="off"){
      angle(setZero,setZero);
      Serial.println("Off");
      mode="off";
    }  
  }
  
}
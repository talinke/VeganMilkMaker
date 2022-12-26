//-------------------------
//Vegan Milk Maker Project
//-------------------------

//Ports of CNC Shield
//X-Step: 2 Dir: 5
//Y-Step: 3 Dir: 6
//Z-Step: 4 Dir: 7

//SpnEn: 12
//SpnDir : 13
//Abort: A0
//Hold: A1
//Resume: A2
//CoolEn: A3
//SCL: A5
//SDA: A4
//-------------------------

//Variables
boolean forceExceed = false;
int forceData = 0;
boolean upperlimit = false;
int pulse = 200*20;
boolean maschine = false;

//Pins
int motorSmash = 2;
int motorSmashDir = 5;
int motorFunnel = 3;
int motorFunnelDir = 6;
int motorSpiral = 4;
int motorSpiralDir = 7;
int buttonPin = A4;

int forceSensor = A5; //SCL
int lightSensor = 13; //SpnEn
int switch12v = 12; //SpnDir


void setup() {
  Serial.begin(9600);
  //Motors
  pinMode(motorSmash, OUTPUT);
  pinMode(motorSmashDir, OUTPUT);
  pinMode(motorFunnel, OUTPUT);
  pinMode(motorFunnelDir, OUTPUT);
  pinMode(motorSpiral, OUTPUT);
  pinMode(motorSpiralDir, OUTPUT);
  
  pinMode(switch12v, OUTPUT); //LOW = Motors, HIGH = Pump
  digitalWrite(motorSpiralDir, HIGH); //spiral always forward
}

void loop() {

  while(maschine){
    //*************************************************
    //Three runs of adding water, oats, and compressing
    for(int runs = 0; runs < 1; runs++){
      
      //pump water for 10 seconds
      Serial.println("Pumping water");
      digitalWrite(switch12v, HIGH);
      delay(10000);
      //turn on 12v for motors
      digitalWrite(switch12v, LOW);

      //drive motor 2 forward
      Serial.println("Moving Oats into position");
      digitalWrite(motorFunnelDir, HIGH); //FORWARD
      //20 rotations
      for(int j = 0; j < 200; j++){
        digitalWrite(motorFunnel, HIGH);
        delayMicroseconds(2500);
        digitalWrite(motorFunnel, LOW);
        delayMicroseconds(2500);
      }
      
      //drive motor 3 one rotation
      Serial.println("Dumping Oats");
      for(int k = 0; k < 2000; k++){
        digitalWrite(motorSpiral, HIGH);
        delayMicroseconds(500);
        digitalWrite(motorSpiral, LOW);
        delayMicroseconds(500);
      }
      delay(2000);
  
      //drive motor 2 backwards
      Serial.println("Retreating Oats");
      digitalWrite(motorFunnelDir, LOW); //BACKWARD
      //20 rotations
      for(int l = 0; l < 200; l++){
        digitalWrite(motorFunnel, HIGH);
        delayMicroseconds(2500);
        digitalWrite(motorFunnel, LOW);
        delayMicroseconds(2500);
      }
  
      //-----------------------------------------------
      //Repeat three times: Lower masher until sensor hits, then move up 20 rotations
      for(int mashes = 0; mashes < 3; mashes++){
        
        Serial.println("Let's mash!");
        digitalWrite(motorSmashDir, HIGH); //move down??
        while(forceExceed == false){
          
          //Get force sensor data
          forceData = analogRead(forceSensor);
          //Serial.println(forceData);
          //delay(50);
          //Check for exceeded force
          Serial.println(forceData);
          if(forceData > 8){
            forceExceed = true;
            Serial.println("Force Exceeded");
          }
          //Drive Motor 1 down
          digitalWrite(motorSmash, HIGH);
          delayMicroseconds(500);
          digitalWrite(motorSmash, LOW);
          delayMicroseconds(500);
          delay(500);
        }
        
        Serial.println("Moving Up 20 rotations");
        digitalWrite(motorSmashDir, LOW); //switch motor direction
        for(int i = 0; i < pulse; i++){
          digitalWrite(motorSmash, HIGH);
          delayMicroseconds(500);
          digitalWrite(motorSmash, LOW);
          delayMicroseconds(500);
        }
        
      forceExceed = false;
      }
      //-----------------------------------------------
      
      //Drive motor 1 to upper limit
      digitalWrite(motorSmashDir, LOW); //motor direction up
      Serial.println("Moving up until limit");
      while(upperlimit == false){
        
        upperlimit = digitalRead(lightSensor);
        digitalWrite(motorSmash, HIGH);
        delayMicroseconds(500);
        digitalWrite(motorSmash, LOW);
        delayMicroseconds(5000);
      }
      Serial.println("Upper limit reached.");
    }
    maschine = false;
    //*************************************************
  }
  
    
  if(analogRead(buttonPin) > 500){
    maschine = true;
    Serial.println(analogRead(buttonPin));
    Serial.println("Let's go");
  }
}

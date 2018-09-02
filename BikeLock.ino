#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial mySerial(4, 2); // RX, TX

String command = ""; // Stores response of the HC-06 Bluetooth device
String state = "";
int stat = 0;
int pos = 0;
int lock = 0;

Servo myservo;

void setup() {
  // Open serial communications:
  Serial.begin(9600);

  //set servo pin and cable input
  myservo.attach(9);
  pinMode(5,INPUT);
 
 
  // The HC-06 defaults to 9600 according to the datasheet.
  mySerial.begin(9600);
}

void loop() {
    if(digitalRead(5)==HIGH){
      //bike is locked
      lock = 1;
    }
    if(digitalRead(5)==LOW){
      // bike is unlocked/cable has broken connection
      lock = 0;
    }
    
// Read device output if available.
  if (mySerial.available()) {
    //read bluetooth input
    command = (char)mySerial.read();
 
    if(command.equals("0")){
      // bluetooth command of 0 means bike should lock
      state = "Locked";
      pos=0;
      stat = 0;
    }
    if(command.equals("1")){
      // bluetooth command of 1 means bike should unlock
      state = "Unlocked";
      pos = 55;
      stat = 1;
    }

    Serial.println(state); command = "";
    }
    //move servo depending on bluetooth command
    if(stat == 1){
      myservo.write(pos);           
    }
    if(stat == 0){  
        myservo.write(pos);             
    }

    if(stat == 0 && lock == 0){
      //if bike is locked and connection is lost to cable signal alarm and alert mobile device
      Serial.println("BREAK!");
       tone(11, 1000, 500);
       tone(11, 2000, 500);
    }
  
}



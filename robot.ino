#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>


#include <Servo.h> 



const int in_1 = 4;
const int in_2 = 5;
const int in_3 = 6;
const int in_4 = 7;

RH_ASK driver;

Servo myServo;

void setup()
{

    myServo.attach(9);

    pinMode(in_1,OUTPUT) ;   //Logic pins are also set as output
    pinMode(in_2,OUTPUT) ;
    pinMode(in_3,OUTPUT) ;   //Logic pins are also set as output
    pinMode(in_4,OUTPUT) ;

    
    
    Serial.begin(9600);  // Debugging only
    
    if (!driver.init())
         Serial.println("init failed");

    

    
}

void loop()
{
    
    
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    String values;
    char charValues[buflen];

    float x_out;
    float y_out;
    float z_out;
    int flexValue = 77;
    float outputs[4];    

    //Serial.println(flexValue);

    //myServo.write(90);
    //delay(2000);

    //myServo.write(180);
    //delay(2000);
   

    
   
    
    if (driver.recv(buf, &buflen)) // Non-blocking
    {   

      


      
      int i;
      for(int i=0; i<buflen; i++){
        values += char(buf[i]);
      }
      values.toCharArray(charValues, buflen);
      //Serial.println(charValues);

      char delimiter[] = ",";

      // initialize first part (string, delimiter)
      char* ptr = strtok(charValues, delimiter);

      int c = 0;
      while(ptr != NULL) {
          //Serial.println(ptr);
          outputs[c] = atof(ptr);
          c++;
          // create next part
          ptr = strtok(NULL, delimiter);
      }

      x_out = outputs[0];
      y_out = outputs[1];
      z_out = outputs[2];
      flexValue = outputs[3];

      Serial.println(String(x_out) + " " + String(y_out) + " " + String(z_out));
      //Serial.println(flexValue);
      

      
    

      // FORWARD
      if(x_out > -0.30 && x_out < 0.30 && y_out < 0.30 && y_out > -0.30 && z_out < -0.30) {

    
        digitalWrite(in_1,HIGH) ;
        digitalWrite(in_2,LOW) ;
        digitalWrite(in_3,HIGH) ;
        digitalWrite(in_4,LOW) ;
        Serial.println("THE ROBOT IS MOVING FORWARD");     
        
        
      }
      
      // right
      else if(x_out < -0.30 && y_out > -0.30 && y_out < 0.30 && z_out > -0.30 && z_out < 0.30){
    
        digitalWrite(in_1,LOW) ;
        digitalWrite(in_2,HIGH) ;
        digitalWrite(in_3,HIGH) ;
        digitalWrite(in_4,LOW) ;
        Serial.println("THE ROBOT TURNS RIGHT");

    
      }
      
      // backward
      else if(x_out > -0.30 && x_out < 0.30 && y_out > -0.30  && y_out < 0.30 && z_out > 0.30){

    
        digitalWrite(in_1,LOW) ;
        digitalWrite(in_2,HIGH) ;
        digitalWrite(in_3,LOW) ;
        digitalWrite(in_4,HIGH) ;
        Serial.println("THE ROBOT IS MOVING BACKWARD");  
        
      }
    
      // left
      else if(x_out > 0.30 && y_out > -0.30 && y_out < 0.30 && z_out > -0.30 && z_out < 0.30){ 
    
        digitalWrite(in_1,HIGH) ;
        digitalWrite(in_2,LOW) ;
        digitalWrite(in_3,LOW) ;
        digitalWrite(in_4,HIGH) ;
        Serial.println("THE ROBOT TURNS LEFT");
   
        
      }
      
      // No movement
      else if(x_out > -0.30 && x_out < 0.30 && y_out < -0.30 && z_out > -0.30 && z_out < 0.30) {
    
        digitalWrite(in_1,LOW) ;
        digitalWrite(in_2,LOW) ;
        digitalWrite(in_3,LOW) ;
        digitalWrite(in_4,LOW) ;
        
        Serial.println("THE ROBOT IS NOT MOVING");     
    
      }

      int servoPosition;
      
      servoPosition = map(flexValue, 77, 95, 40, 0);
      //Serial.println(servoPosition);
      myServo.write(servoPosition);
      
      delay(20);
      
    }
    
}

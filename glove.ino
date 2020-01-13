#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

RH_ASK driver;

// Used for software SPI
#define LIS3DH_CLK 8   // SCL
#define LIS3DH_MOSI 9  // SDA
#define LIS3DH_MISO 10  // SD0
#define LIS3DH_CS 11  // CS

const int flexPin = A0; 


// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);


void setup()
{
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");

    Serial.println("LIS3DH test!");
    if (! lis.begin(0x18)) {  
      Serial.println("Couldnt start");
      while (1) yield();
    }
    Serial.println("LIS3DH found!");
    
    lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
    Serial.print("Range = "); Serial.print(2 << lis.getRange());  
    Serial.println("G");
}

void loop()
{

    int flexValue;
    flexValue = analogRead(flexPin);
    Serial.print("sensor: ");
    Serial.println(flexValue);

    lis.read();      // get X Y and Z data at once

    /* Or....get a new sensor event, normalized */ 
    sensors_event_t event; 
    lis.getEvent(&event);

    /* Display the results (acceleration is measured in m/s^2) */
    String x_out = String(event.acceleration.x / 9.81);
    String y_out = String(event.acceleration.y / 9.81);
    String z_out = String(event.acceleration.z / 9.81);
  
    String values = x_out + "," + y_out + "," + z_out + "," + flexValue;
    
    int vLen = values.length();
    char charValues [vLen];
    values.toCharArray(charValues, vLen+1);
    const char *sendValues = charValues;
    
    Serial.println(values);

    driver.send((uint8_t *)sendValues, strlen(sendValues));
    driver.waitPacketSent();

    delay(100);
}

// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCS34725
// This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// TCS34725 I2C address is 0x29(41)
#define Addr 0x29

int red = 0, green = 0, blue = 0, cData = 0;
float luminance = 0.0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "TCS34725");
  Particle.variable("red", red);
  Particle.variable("green", green);
  Particle.variable("blue", blue);
  Particle.variable("luminance", luminance);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Wait Time register
  Wire.write(0x83);
  // Set wait time = 2.4 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Atime register
  Wire.write(0x81);
  // Atime = 700 ms, max count = 65536
  Wire.write(0x00);
  // Stop I2C Transmission on the device
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x8F);
  // AGAIN = 1x
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x80);
  // Power ON, RGBC enable, wait time disable
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(800);
}

void loop()
{
  unsigned int data[8];

  // Start I2C Transmission on the device
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x94);
  // Stop I2C Transmission on the device
  Wire.endTransmission();

  // Request 8 byte of data from the device
  Wire.requestFrom(Addr, 8);

  // Read 8 bytes of data
  // cData lsb, cData msb, red lsb, red msb, green lsb, green msb, blue lsb, blue msb
  if (Wire.available() == 8)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
    data[6] = Wire.read();
    data[7] = Wire.read();
  }

  // Convert the data
  cData = (data[1] * 256) + data[0];
  red = (data[3] * 256) + data[2];
  green = (data[5] * 256) + data[4];
  blue = (data[7] * 256) + data[6];

  // Calculate the final lux
  luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue);

  // Output data to dashboard
  Particle.publish("Red Color Luminance   : ", String(red));
  delay(100);
  Particle.publish("Green Color Luminance : ", String(green));
  delay(100);
  Particle.publish("Blue Color Luminance  : ", String(blue));
  delay(100);
  Particle.publish("IR Luminance          : ", String(cData));
  Particle.publish("Ambient Light Luminance : ", String(luminance));
  delay(500);
}

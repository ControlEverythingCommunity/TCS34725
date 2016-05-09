// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCS34725
// This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <Wire.h>

// TCS34725 I2C address is 0x29(41)
#define Addr 0x29

void setup()
{
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
  float cData = (data[1] * 256.0) + data[0];
  float red = (data[3] * 256.0) + data[2];
  float green = (data[5] * 256.0) + data[4];
  float blue = (data[7] * 256.0) + data[6];

  // Calculate the final lux
  float luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue);

  // Output data to serial monitor
  Serial.print("Red color Luminance   : ");
  Serial.print(red);
  Serial.println(" lux");
  Serial.print("Green color Luminance : ");
  Serial.print(green);
  Serial.println(" lux");
  Serial.print("Blue color Luminance  : ");
  Serial.print(blue);
  Serial.println(" lux");
  Serial.print("IR Luminance          : ");
  Serial.print(cData);
  Serial.println(" lux");
  Serial.print("Ambient Light Luminance : ");
  Serial.print(luminance);
  Serial.println(" lux");
  delay(500);
}


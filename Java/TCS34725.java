// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCS34725
// This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class TCS34725
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, TCS34725 I2C address is 0x29(41)
		I2CDevice device = bus.getDevice(0x29);

		// Select enable register
		// Power ON, RGBC enable, wait time disable
		device.write(0x80, (byte)0x03);
		// Select ALS time register
		// Atime = 700 ms
		device.write(0x81, (byte)0x00);
		// Select Wait Time register
		// WTIME : 2.4ms
		device.write(0x83, (byte)0xFF);
		// Select control register
		// AGAIN = 1x
		device.write(0x8F, (byte)0x00);
		Thread.sleep(800);

		// Read 8 bytes of data
		// cData lsb, cData msb, red lsb, red msb, green lsb, green msb, blue lsb, blue msb
		byte[] data = new byte[8];
		device.read(0x94, data, 0, 8);
		// Convert the data
		int cData = ((data[1] & 0xFF) * 256) + (data[0] & 0xFF);
		int red = ((data[3] & 0xFF) * 256) + (data[2] & 0xFF);
		int green = ((data[5] & 0xFF) * 256) + (data[4] & 0xFF); 
		int blue = ((data[7] & 0xFF) * 256) + (data[6] & 0xFF);

		// Calculate final lux
		double luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue);

		// Output data to Screen
		System.out.printf("Red Color Luminance   : %d lux %n", red);
		System.out.printf("Green Color Luminance : %d lux %n", green);
		System.out.printf("Blue Color Luminance  : %d lux %n", blue);
		System.out.printf("IR Luminance          : %d lux %n", cData);
		System.out.printf("Ambient Light Luminance : %d lux %n", luminance);
	}
}

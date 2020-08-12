#define X_PATH	"/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-accel-legacy.3.auto/iio:device1/in_accel_x_raw"

#define Z_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-accel-legacy.2.auto/iio:device0/in_accel_z_raw"
#define Z_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-accel-legacy.3.auto/iio:device1/in_accel_z_raw"

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>

int main(int argc, char **argv)
{
	// Output values of accelerometer
	int		x = 0;
	int		z_base = 0;
	int		z_lid = 0;


	char		tablet = 'Y';	// Y(es), N(o)
	char		rotation = 'N'; // N(one), L(eft), R(ight)
	
	// X file descriptor, Z Lid file descriptor, Z base file descriptor
	FILE		*xfd;
	FILE		*zlfd;
	FILE		*zbfd;

	while (1)
	{
		// Read lid and base Z values
		zbfd	= fopen(Z_PATH_BASE, "r");
		fscanf(zbfd, "%lu", &z_base);
		fclose(zbfd);
		
		zlfd	= fopen(Z_PATH_LID, "r");
		fscanf(zlfd, "%lu", &z_lid);
		fclose(zlfd);
		
		if ((z_lid - z_base) > -100 && tablet != 'Y')
		{
			tablet = 'Y';
			system("handle T");
		}
		
		if ((z_lid - z_base) < 100 && tablet != 'N')
		{
			tablet = 'N';
			system("handle LP");
			system("handle N"); // Rotate back to normal, to use laptop like normal
		}
		
		
		// Update rotation accelerometer position
		xfd	= fopen(X_PATH, "r");
		fscanf(xfd, "%lu", &x);
		fclose(xfd);
		
		usleep(500000);
		if (x < -500)
		{
			if (rotation != 'R' && tablet == 'Y')
			{
				system("handle R");
				rotation = 'R';
			}
		}

		if (x > 500)
		{
			if (rotation != 'L' && tablet == 'Y')
			{
				system("handle L");
				rotation = 'L';
			}
		}
		
		if (x > -500 && x < 500)
		{
			if (rotation != 'N')
			{
				system("handle N");
				rotation = 'N';
			}
		}
	}
	return 0;
}

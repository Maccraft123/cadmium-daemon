#ifdef C100

#define X_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_x_raw"
#define Z_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_z_raw"
#define Y_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_y_raw"
#define Y_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_y_raw"
#define X_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.7.auto/iio:device0/in_accel_x_raw"
#define Z_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.7.auto/iio:device0/in_accel_z_raw"

#else
#define X_PATH_BASE "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.13.auto/iio:device3/in_accel_x_raw"
#define Z_PATH_BASE "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.13.auto/iio:device3/in_accel_z_raw"
#define Y_PATH_BASE "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.13.auto/iio:device3/in_accel_y_raw"
#define Y_PATH_LID "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.11.auto/iio:device1/in_accel_y_raw"
#define X_PATH_LID "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.11.auto/iio:device1/in_accel_x_raw"
#define Z_PATH_LID "/sys/devices/platform/ff200000.spi/spi_master/spi2/spi2.0/cros-ec-dev.0.auto/cros-ec-sensorhub.1.auto/cros-ec-accel.11.auto/iio:device1/in_accel_z_raw"
#endif

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<math.h>
#include	<linux/input.h>
#include	<fcntl.h>

int main(int argc, char **argv)
{
	//setvbuf(stdout, NULL, _IONBF, 0);
	// Output values of accelerometer
	short		x_base = 0;
	//short		y_base = 0;
	short		z_base = 0;
	//short		x_lid = 0;
	//short		y_lid = 0;
	short		z_lid = 0;

	char		mode = '?';

	//FILE		*xlfd;
	//FILE		*ylfd;
	FILE		*zlfd;
	FILE		*xbfd;
	//FILE		*ybfd;
	FILE		*zbfd;
#ifdef GNOME
	int		kbfd;
	int		tpfd;
	tpfd = open("/dev/input/event7", O_RDONLY); //TODO: dynamically do it
	kbfd = open("/dev/input/event0", O_RDONLY);
#endif

	int		trip = 0;

	while (1)
	{
		//xlfd	= fopen(X_PATH_LID, "r");
		//fscanf(xlfd, "%hd", &x_lid);
		//fclose(xlfd);

		//ylfd	= fopen(Y_PATH_LID, "r");
		//fscanf(ylfd, "%hd", &x_lid);
		//fclose(ylfd);

		zlfd    = fopen(Z_PATH_LID, "r");
		fscanf(zlfd, "%hd", &z_lid);
		fclose(zlfd);

        	xbfd    = fopen(X_PATH_BASE, "r");
        	fscanf(xbfd, "%hd", &x_base);
        	fclose(xbfd);

		//ybfd	= fopen(Y_PATH_BASE, "r");
		//fscanf(ybfd, "%hd", &y_base);
		//fclose(ybfd);

                zbfd    = fopen(Z_PATH_BASE, "r");
                fscanf(zbfd, "%hd", &z_base);
                fclose(zbfd);

		if (abs(z_base/32) - z_lid/32 > trip) // tablet
		{
			if (mode != 'T' && abs(x_base) < 900)
			{
#ifdef GNOME
				printf("Grabbing touchpad and keyboard\n");
				ioctl(kbfd, EVIOCGRAB, (void*)1);
				ioctl(tpfd, EVIOCGRAB, (void*)1);
#else
				system("./handle T");
#endif
				mode	= 'T';
				trip = -20;
			}
		}	
		else
		{
			if (mode != 'L' && abs(x_base) < 900)
			{
#ifdef GNOME
				printf("Ungrabbing touchpad and keyboard\n");
				ioctl(kbfd, EVIOCGRAB, (void*)0);
				ioctl(tpfd, EVIOCGRAB, (void*)0);
#else
				system("./handle LP");
#endif
				mode	= 'L';
				trip = 20;
			}
		}
		usleep(100000);
	}
}

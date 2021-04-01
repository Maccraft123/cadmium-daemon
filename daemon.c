#ifdef C100
#define KBDEV "/dev/input/by-path/platform-ff110000.spi-cs-0-platform-ff110000.spi:ec@0:keyboard-controller-event-kbd"
#define TPDEV "/dev/input/by-path/platform-ff160000.i2c-event-mouse"
#define X_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_x_raw"
#define Z_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_z_raw"
#define Y_PATH_BASE "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_y_raw"
#define Y_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.8.auto/iio:device1/in_accel_y_raw"
#define X_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.7.auto/iio:device0/in_accel_x_raw"
#define Z_PATH_LID "/sys/devices/platform/ff110000.spi/spi_master/spi0/spi0.0/cros-ec-dev.1.auto/cros-ec-sensorhub.2.auto/cros-ec-accel-legacy.7.auto/iio:device0/in_accel_z_raw"
#endif

#ifdef KEVIN
#define KBDEV "/dev/input/by-path/platform-ff200000.spi-cs-0-platform-ff200000.spi:ec@0:keyboard-controller-event-kbd"
#define TPDEV "/dev/input/by-path/platform-ff140000.i2c-event-mouse"
#define TSDEV "/dev/input/by-path/platform-ff130000.i2c-event"

#define PENDEV "/dev/input/by-path/platform-gpio-keys-event"

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
	pid_t pid = fork();

	if(pid == -1)
		return 2;

#ifdef PENDEV
	if(pid == 0)
	{
		int             tsfd;
		int             penfd;
		tsfd = open(TSDEV, O_RDONLY);
		penfd = open(PENDEV, O_RDONLY);

		struct          input_event ev[64];
		int             rd;
		while(1)
		{
			rd = read(penfd, ev, sizeof(struct input_event) * 64);
			for (int i = 0; i < rd / sizeof(struct input_event); i++)
			{
				if (ev[i].type == EV_SW)
				{
					if(ev[i].value == 0)
					{
						printf("Grabbing touchscreen\n");
						ioctl(tsfd, EVIOCGRAB, (void*)1);
					}
					else
					{
						printf("Ungrabbing touchscreen\n");
						ioctl(tsfd, EVIOCGRAB, (void*)0);
					}
				}
			}
			usleep(100000);

		}
	}
	else
	{
#endif

		//setvbuf(stdout, NULL, _IONBF, 0);
		// Output values of accelerometer
		short           x_base = 0;
		//short         y_base = 0;
		short           z_base = 0;
		//short         x_lid = 0;
		//short         y_lid = 0;
		short           z_lid = 0;

		char            mode = '?';

		//FILE          *xlfd;
		//FILE          *ylfd;
		FILE            *zlfd;
		FILE            *xbfd;
		//FILE          *ybfd;
		FILE            *zbfd;
		int             kbfd;
		int             tpfd;
		tpfd = open(TPDEV, O_RDONLY);
		kbfd = open(KBDEV, O_RDONLY);

		int             trip = 0;

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
					printf("Grabbing touchpad and keyboard\n");
					ioctl(kbfd, EVIOCGRAB, (void*)1);
					ioctl(tpfd, EVIOCGRAB, (void*)1);
					mode	= 'T';
					trip	= -50;
				}
			}	
			else
			{
				if (mode != 'L' && abs(x_base) < 900)
				{
					printf("Ungrabbing touchpad and keyboard\n");
					ioctl(kbfd, EVIOCGRAB, (void*)0);
					ioctl(tpfd, EVIOCGRAB, (void*)0);
					mode	= 'L';
					trip	= 50;
				}
			}
			usleep(100000);
		}
	#ifdef PENDEV
	}
	#endif
}

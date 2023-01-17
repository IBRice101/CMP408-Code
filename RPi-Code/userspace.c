#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44
#define REGISTER_SAPP _IO('R', 'g')

// This app serves as a userspace intermediary between the LKM running on the Pi and the Speedtester app
// Python is seemingly unable to interface between itself and the LKM so as a result it was easier to implement this in C
// Partial credit is due to Johannes4Linux who provided boilerplate to allow for the LKM to actually interface with a userspace application

// FIXME: run only once and then wait

void signalhandler(int sig) {
	FILE *fp;
    char path[1035];

    printf("Running speedtester.py\n");

    /* Open the command for reading. */
    fp = popen("python3 speedtester.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }

    /* close */
    pclose(fp);

    printf("Speedtest completed\n");

    return 0;
}

int main() {
	int fd;
	signal(SIGTX, signalhandler);

	printf("PID: %d\n", getpid());

	/* Open the device file */
	fd = open("/dev/irq_signal", O_RDONLY);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

	/* Register app to KM */
	if(ioctl(fd, REGISTER_SAPP, NULL)) {
		perror("Error registering app");
		close(fd);
		return -1;
	}


	/* Wait for Signal */
	printf("Wait for signal...\n");
	while(1)
		sleep(1);

	return 0;
}


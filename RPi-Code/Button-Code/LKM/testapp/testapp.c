#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44
#define REGISTER_UAPP _IO('R', 'g')

void signalhandler(int sig) {
	FILE *fp;
    char path[1035];

    printf("Running speedtester.py\n");

    /* Open the command for reading. */
    fp = popen("python3 ../../../speedtester.py", "r");
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
	if(ioctl(fd, REGISTER_UAPP, NULL)) {
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


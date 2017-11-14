#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>

#include "config.h"

#define PWM_MODE HWMON PWM1_ENABLE
#define PWM_VALUE HWMON PWM1

#define PWM_MODE_AUTO   0
#define PWM_MODE_MANUAL 1

void usage(char *progname) {
	printf("Usage: %s <speed>\n\n"
			"  where speed is:\n\n"
			"    0       automatic fan control\n"
			"    10-100  manual override (in %% of max fan speed)\n\n",
			basename(progname));
}

/* 0 on failure, 1 on success */
int setval(char *fname, int value) {
	int success = 1;
	FILE *f;

	f = fopen(fname, "w");
	if (f == NULL) {
		fprintf(stderr, "Error opening %s\n", fname);
		success = 0;
		goto out;
	}

	if (fprintf(f, "%u\n", value) < 0 || !!fflush(f)) {
		fprintf(stderr, "Error writing %u to %s\n", value, fname);
		success = 0;
		goto out;
	}

out:
	if (f != NULL) {
		if (fclose(f)) {
			fprintf(stderr, "Error closing %s\n", fname);
			success = 0;
		}
	}

	return success;
}


int digit(char c) {
	return c >= '0' && c <= '9';
}

int digitsonly(char *val) {
	int cnt = 0;

	while (*val) {
		if (digit(*val++) == 0) {
			return 0;
		}
		cnt++;
	}

	return !!cnt;
}

int convert(char *val) {
	return digitsonly(val) ? strtol(val, NULL, 10) : -1;
}

int main(int argc, char *argv[]) {
	int success = 0;
	int value;

	if (argc != 2) {
		usage(argv[0]);
		goto out;
	}

	value = convert(argv[1]);

	if (value == 0) {
		success = setval(PWM_MODE, PWM_MODE_AUTO);

		if (success) {
			printf("Fan speed set to automatic mode\n");
		}
	} else if (value >= 10 && value <= 100) {
		success = setval(PWM_MODE, PWM_MODE_MANUAL)
			&& setval(PWM_VALUE, (value * 255) / 100);

		if (success) {
			printf("Fan speed set to %d%%\n", value);
		}
	} else {
		usage(argv[0]);
		fprintf(stderr, "Invalid value: %s\n", argv[1]);
		goto out;
	}


out:
	return !success;
}

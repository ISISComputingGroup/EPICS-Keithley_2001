#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "KeithleyUtils.h"

#define MAX_INT_CHANNELS 10

void find_active_channels(int channels[], int number_of_active_channels, int* activated_channels) {
	int position = 0;
	for (int i = 0; i < MAX_INT_CHANNELS; i++) {
		if (channels[i] == 1) {
			activated_channels[position] = i + 1;
			position++;
		}
	}
}

void generate_scan_channel_string(int *active_channels, int number_of_active_channels, char *scan_channels_string) {
	for (int i = 0; i < number_of_active_channels; i++) {
		char *temp = (char *)malloc(sizeof(char) * 3 + 1);
		memset(temp, 0, sizeof(temp));
		if (i == (number_of_active_channels - 1)) {
			sprintf(temp, "%d", active_channels[i]);
		}
		else {
			sprintf(temp, "%d,", active_channels[i]);
		}
		strncat(scan_channels_string, temp, sizeof(scan_channels_string));
		free(temp);
	}
}

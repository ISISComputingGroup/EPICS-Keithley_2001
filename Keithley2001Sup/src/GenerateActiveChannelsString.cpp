#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "GenerateActiveChannelsString.h"

void GenerateActiveChannelsString_impl(int *active_channels, int number_of_active_channels, char *scan_channels_string) {
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

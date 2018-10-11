#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include "KeithleyUtilsCpp.h"
#include "KeithleyUtils.h"

#define MAX_NUMBER_OF_CHANNELS 10
#define ACTIVE 1

void find_active_channels(int channels[],  int activated_channels[]){
	int position = 0;
	for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++) {
		if (channels[i] == ACTIVE) {
			activated_channels[position] = i + 1;
			position++;
		}
	}
}

void generate_scan_channel_string(int active_channels[], int number_of_active_channels,  char set_channels_to_scan[]) {
	GenerateActiveChannelsString(active_channels, number_of_active_channels, set_channels_to_scan);
}

int get_number_of_active_channels(int channels[]) {
	return GetNumberOfActiveChannels(channels);
}

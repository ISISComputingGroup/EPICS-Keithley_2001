#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include "sequencer_utils_cpp.h"
#include "sequencer_utils.h"

#define MAX_NUMBER_OF_CHANNELS 10
#define ACTIVE 1

// Find active channels and enters their position into an array.
void find_active_channels(int channels[],  int activated_channels[]){
	int position = 0;
	for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++) {
		if (channels[i] == ACTIVE) {
			activated_channels[position] = i + 1;
			position++;
		}
	}
}

// Calls the c++ function that generates the comma sepatated list of active channels.
void generate_scan_channel_string(int active_channels[], int number_of_active_channels,  char set_channels_to_scan[]) {
	GenerateScanChannelsString(active_channels, number_of_active_channels, set_channels_to_scan);
}

// Calls the c++ function that gets the sum of active channels.
int sum_of_active_channels(int channels[]) {
	return SumOfActiveChannels(channels);
}

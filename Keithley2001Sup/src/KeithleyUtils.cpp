#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include "GenerateActiveChannelsString.h"
#include "KeithleyUtils.h"

#define MAX_INT_CHANNELS 10

void find_active_channels(int channels[],  int activated_channels[]){
	int position = 0;
	for (int i = 0; i < MAX_INT_CHANNELS; i++) {
		if (channels[i] == 1) {
			activated_channels[position] = i + 1;
			position++;
		}
	}
}

void generate_scan_channel_string(int active_channels[], int number_of_active_channels,  char set_channels_to_scan[]) {
	GenerateActiveChannelsString_impl(active_channels, number_of_active_channels, set_channels_to_scan);
}

int get_number_of_active_channels(int channels[]) {
	return GetNumberOfActiveChannels(channels);
}

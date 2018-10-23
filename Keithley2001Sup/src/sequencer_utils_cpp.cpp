#include <iostream>
#include <string>
#include <cstring>
#include <numeric>

#include "sequencer_utils_cpp.h"

#define MAX_NUMBER_OF_CHANNELS 10

// Generates a comma separated list from an array of integers.
void GenerateScanChannelsString(int active_channels[], int number_of_active_channels, char set_channels_to_scan[]) {
	std::string channel_string;

	for (int i = 0; i < number_of_active_channels; i++) {
		channel_string += std::to_string(static_cast<long long>(active_channels[i]));
		if (i != (number_of_active_channels - 1)) {
			channel_string += ",";
		}
	}
	strncpy(set_channels_to_scan, channel_string.c_str(), 39);
}

// Sum of the active channels.
int SumOfActiveChannels(int channels[]) {
	return std::accumulate(channels, channels + MAX_NUMBER_OF_CHANNELS, 0);
}

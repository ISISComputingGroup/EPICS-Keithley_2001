#include <iostream>
#include <string>
#include <cstring>
#include <numeric>

#include "GenerateActiveChannelsString.h"

void GenerateActiveChannelsString_impl(int active_channels[], int number_of_active_channels, char set_channels_to_scan[]) {
	if (number_of_active_channels > 10) {
		throw std::invalid_argument("Number of active channels is greater than 10.");
	}
	
	std::string channel_string;

	for (int i = 0; i < number_of_active_channels; i++) {
		channel_string += std::to_string(active_channels[i]);
		if (i != (number_of_active_channels - 1)) {
			channel_string += ",";
		}
	}
	strncpy(set_channels_to_scan, channel_string.c_str(), 39);
}

int GetNumberOfActiveChannels(int channels[]) {
	return std::accumulate(channels, channels + 10, 0);
}

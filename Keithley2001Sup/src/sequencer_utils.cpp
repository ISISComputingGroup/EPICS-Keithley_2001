#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <numeric>

#include "sequencer_utils.h"
#include "definitions.h"
#include <errlog.h>


// Find active channels and enters their position into an array.
long find_active_channels(int channels[],  int activated_channels[]){
    int position = 0;
    if (sizeof(activated_channels) / sizeof(int) > MAX_NUMBER_OF_CHANNELS) {
        return 1;
    }

    for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++) {
        if (channels[i] == ACTIVE) {
            activated_channels[position] = i + 1;
            position++;
        }
    }
    return 0;
}

// Calls the c++ function that generates the comma sepatated list of active channels.
void generate_scan_channel_string(int active_channels[], int number_of_active_channels,  char set_channels_to_scan[]) {
    std::string channel_string;

    for (int i = 0; i < number_of_active_channels; i++) {
        channel_string += std::to_string(static_cast<long long>(active_channels[i]));
        if (i != (number_of_active_channels - 1)) {
            channel_string += ",";
        }
    }
    strncpy(set_channels_to_scan, channel_string.c_str(), 39);
}

// Calls the c++ function that gets the sum of active channels.
int sum_of_active_channels(int channels[]) {
    return std::accumulate(channels, channels + MAX_NUMBER_OF_CHANNELS, 0);
}

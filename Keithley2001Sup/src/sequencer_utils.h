#ifndef sequencer_utils
#define sequencer_utils

#ifdef __cplusplus
extern "C" {
#endif

    long find_active_channels(int channels[], int activated_channels[]);
    void generate_scan_channel_string(int active_channels[], int number_of_active_channels, char set_channels_to_scan[]);
    int sum_of_active_channels(int channels[]);

#ifdef __cplusplus
}
#endif

#endif // !sequencer_utils

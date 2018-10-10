#ifndef KeithleyUtils
#define KeithleyUtils

#ifdef __cplusplus
extern "C" {
#endif

	void find_active_channels(int channels[], int number_of_active_channels, int *activated_channels);
	void generate_scan_channel_string(int *active_channels, int number_of_active_channels, char set_channels_to_scan[]);

#ifdef __cplusplus
}
#endif

#endif // !KeithleyUtils

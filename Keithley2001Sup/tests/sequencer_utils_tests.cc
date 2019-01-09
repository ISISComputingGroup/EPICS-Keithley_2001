#include <string>
#include "gtest/gtest.h"

#include "..\src\sequencer_utils.h"

namespace {
    TEST(FindActiveChannels, test_that_GIVEN_three_active_channels_THEN_they_are_both_found) {
        // Given:
		int channels [10] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0};
		int number_of_active_channels = 3;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0};

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[10] = { 1, 5, 9, 0, 0, 0, 0, 0, 0, 0};
		for (int i = 0; i < 10; i++) {
		    EXPECT_EQ(activated_channels[i], expected_channels[i]);
        }
    }

    TEST(FindActiveChannels, test_that_GIVEN_all_active_channels_THEN_they_are_all_found) {
        // Given:
		int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		int number_of_active_channels = 10;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		for (int i = 0; i < 10; i++) {
		    EXPECT_EQ(activated_channels[i], expected_channels[i]);
		}
    }

    TEST(FindActiveChannels, test_that_GIVEN_one_active_channels_THEN_it_is_found) {
        // Given:
		int channels[10] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 1;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[10] = { 1,0,0,0,0,0,0,0,0,0 };
		for (int i = 0; i < 10; i++) {
			EXPECT_EQ(activated_channels[i], expected_channels[i]);
		}
    }

    TEST(FindActiveChannels, test_that_GIVEN_no_active_channels_THEN_none_are_found) {
       // Given:
		int channels[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 0;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };
		for (int i = 0; i < 10; i++) {
			EXPECT_EQ(activated_channels[i], expected_channels[i]);
		}
    }

    TEST(GenerateScanChannelsString, test_that_GIVEN_two_active_channels_THEN_the_expected_string_is_created) {
      // Given:
		int channels[10] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 2;
		char scan_channels_string[40];
		int activated_channels[10] = { 1,2,0,0,0,0,0,0,0,0 };

		// When
        generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,2";
		EXPECT_STREQ(scan_channels_string, expected_string);
    }

    TEST(GenerateScanChannelsString, test_that_GIVEN_all_channels_active_THEN_the_expected_string_is_created) {
      // Given:
		int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		int number_of_active_channels = 10;
		char scan_channels_string[40];
		int activated_channels[10] = { 1,2,3,4,5,6,7,8,9,10 };

		// When
        generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,2,3,4,5,6,7,8,9,10";
		EXPECT_STREQ(scan_channels_string, expected_string);
    }

    TEST(GenerateScanChannelsString, test_that_GIVEN_odd_channels_active_THEN_a_string_with_only_odd_channels_is_created) {
        // Given:
		int channels[10] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
		int number_of_active_channels = 5;
		char scan_channels_string[40];
		int activated_channels[10] = { 1,3,5,7,9,0,0,0,0,0 };

		// When
        generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,3,5,7,9";
		EXPECT_STREQ(scan_channels_string, expected_string);
    }
    
    TEST(GenerateScanChannelsString, test_that_GIVEN_a_loop_through_the_channels_add_a_new_one_each_time_THEN_the_correct_string_is_generated_each_time) {
       // Given:
		int channels[10] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 2;
		std::string expected = "1,2";
		char scan_channels_string[40];
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };
		
		for (int i = 1; i < 10; i++) {
			memset(activated_channels, 0, sizeof(activated_channels));
			find_active_channels(channels, activated_channels);

			// When:
            generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

			// Then:
			EXPECT_EQ(scan_channels_string, expected);

			// Set up next loop
			if (i == 9) {
				break;
			}
			channels[i + 1] = 1;
			number_of_active_channels++;
			expected += "," + std::to_string(static_cast<long long>(i + 2));
		}
    }

} // namespace

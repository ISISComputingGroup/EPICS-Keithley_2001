// KeithleyTestSuite1.h

#include <cxxtest/TestSuite.h>
#include <stdlib.h>
#include <string>

#include "..\src\KeithleyUtils.h"
#include "..\src\GenerateActiveChannelsString.h"


class FindActiveChannelsTestSuite : public CxxTest::TestSuite
{
public:
    void test_that_GIVEN_two_active_channels_THEN_they_are_both_found(void) {
        // Given:
		int channels [10] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
		int number_of_active_channels = 2;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0};

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[2] = { 1, 2 };
		for (int i = 0; i < number_of_active_channels; i++) {
			TS_ASSERT_EQUALS(activated_channels[i], expected_channels[i]);
		}
    }


	void test_that_GIVEN_all_active_channels_THEN_they_are_all_found(void)	{
		// Given:
		int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		int number_of_active_channels = 10;
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		// When
		find_active_channels(channels, activated_channels);

		// Then
		int expected_channels[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		for (int i = 0; i < number_of_active_channels; i++) {
			TS_ASSERT_EQUALS(activated_channels[i], expected_channels[i]);
		}
	}
};

class GenerateScanChannelStringTestSuite : public CxxTest::TestSuite
{
public:
	void test_that_GIVEN_two_active_channels_THEN_the_expected_string_is_found(void) {
		// Given:
		int channels[10] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 2;
		char scan_channels_string[40];

		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };
		find_active_channels(channels, activated_channels);

		// When
		generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,2";
		TS_ASSERT_EQUALS(scan_channels_string, expected_string);
	}

	void test_that_GIVEN_all_channels_active_THEN_the_expected_string_is_found(void) {
		// Given:
		int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		int number_of_active_channels = 10;
		char scan_channels_string[40];

		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };
		find_active_channels(channels, activated_channels);

		// When
		generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,2,3,4,5,6,7,8,9,10";
		TS_ASSERT_EQUALS(scan_channels_string, expected_string);
	}

	void test_that_GIVEN_odd_channels_active_THEN_odd_channels_are_added_to_the_string(void) {
		// Given:
		int channels[10] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
		int number_of_active_channels = 5;
		char scan_channels_string[40];

		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };
		find_active_channels(channels, activated_channels);

		// When
		generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);

		// Then
		char* expected_string = "1,3,5,7,9";
		TS_ASSERT_EQUALS(scan_channels_string, expected_string);
	}

	void test_that_GIVEN_a_loop_THEN_we_dont_get_a_heap_error(void)	{
		// Given:
		int number_of_active_channels = 2;
		int channels[10] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		char scan_channels_string[40];		
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		for (int i = 1; i < 10; i++) {
			memset(activated_channels, 0, sizeof(activated_channels));
			find_active_channels(channels, activated_channels);

			// When/Then:
			TS_ASSERT_THROWS_NOTHING(generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string));
			if (i == 9) {
				break;
			}
			channels[i + 1] = 1;
			number_of_active_channels++;
		}
	}

	void test_that_GIVEN_a_loop_THEN_we_get_the_correct_string(void) {
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
			TS_ASSERT_THROWS_NOTHING(generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string));

			// Then:
			TS_ASSERT_EQUALS(scan_channels_string, expected);

			// Set up next loop
			if (i == 9) {
				break;
			}
			channels[i + 1] = 1;
			number_of_active_channels++;
			expected += "," + std::to_string(i + 2);
		}
	}

	void test_that_GIVEN_a_loop_which_switches_between_5_and_7_channels_THEN_we_dont_get_a_heap_error(void) {
		// Given:
		int number_of_active_channels = 5;
		int channels[10] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
		std::string expected_5 = "1,2,3,4,5";
		std::string expected_7 = "1,2,3,4,5,6,7";
		bool five_channels = true;
		char scan_channels_string[40];
		int activated_channels[10] = { 0,0,0,0,0,0,0,0,0,0 };

		for (int i = 0; i < 100; i++) {
			memset(activated_channels, 0, sizeof(activated_channels));
			find_active_channels(channels, activated_channels);

			// When:
			TS_ASSERT_THROWS_NOTHING(generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string));

			// Then:
			if (five_channels) {
				TS_ASSERT_EQUALS(scan_channels_string, expected_5);
				// Set to scan on severn channels
				channels[5] = 1;
				channels[6] = 1;
				number_of_active_channels = 7;
				five_channels = false;
			}
			else {
				TS_ASSERT_EQUALS(scan_channels_string, expected_7);
				// Set to scan on five channels
				channels[5] = 0;
				channels[6] = 0;
				number_of_active_channels = 5;
				five_channels = true;
			}
		}
	}
};

class GetNumberOfActiveChannelsTests : public CxxTest::TestSuite {
public:
	void test_that_GIVEN_an_array_with_a_single_one_THEN_the_number_of_active_channels_is_one(void) {
		// Given:
		int channels[10] = { 1,0,0,0,0,0,0,0,0,0};
		
		// When:
		int result = GetNumberOfActiveChannels(channels);
		
		// Then:
		int expected_result = 1;
		TS_ASSERT_EQUALS(result, expected_result);
	}

	void test_that_GIVEN_an_array_of_ones_THEN_the_number_of_active_channels_is_ten(void) {
		// Given:
		int channels[10] = { 1,1,1,1,1,1,1,1,1,1 };

		// When:
		int result = GetNumberOfActiveChannels(channels);

		// Then:
		int expected_result = 10;
		TS_ASSERT_EQUALS(result, expected_result);
	}

	void test_that_GIVEN_an_array_of_four_ones_THEN_the_number_of_active_channels_is_four(void) {
		// Given:
		int channels[10] = { 1,0,1,0,1,0,0,0,0,1 };

		// When:
		int result = GetNumberOfActiveChannels(channels);

		// Then:
		int expected_result = 4;
		TS_ASSERT_EQUALS(result, expected_result);
	}
};

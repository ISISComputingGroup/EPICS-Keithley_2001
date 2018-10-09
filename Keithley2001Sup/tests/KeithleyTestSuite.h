// KeithleyTestSuite1.h

#include <cxxtest/TestSuite.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "..\src\KeithleyUtils.h"


class FindActiveChannelsTestSuite : public CxxTest::TestSuite
{
public:
    void test_that_GIVEN_two_active_channels_THEN_they_are_both_found(void)
    {
        // Given:
		int channels [10] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
		int number_of_active_channels = 2;
		int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
		memset(activated_channels, 0, sizeof(activated_channels));

		// When
		find_active_channels(channels, number_of_active_channels, activated_channels);

		// Then
		int expected_channels[2] = { 1, 2 };
		for (int i = 0; i < number_of_active_channels; i++) {
			TS_ASSERT_EQUALS(activated_channels[i], expected_channels[i]);
		}
		free(activated_channels);
    }


	void test_that_GIVEN_all_active_channels_THEN_they_are_all_found(void)
	{
		// Given:
		int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		int number_of_active_channels = 10;
		int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
		memset(activated_channels, 0, sizeof(activated_channels));

		// When
		find_active_channels(channels, number_of_active_channels, activated_channels);

		// Then
		int expected_channels[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		for (int i = 0; i < number_of_active_channels; i++) {
			TS_ASSERT_EQUALS(activated_channels[i], expected_channels[i]);
		}
		free(activated_channels);
	}
};

class GenerateScanChannelStringTestSuite : public CxxTest::TestSuite
{
public:
	void test_that_GIVEN_two_active_channels_THEN_the_expected_string_is_found(void) {
		// Given:
		int channels[10] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		int number_of_active_channels = 2;

		int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
		memset(activated_channels, 0, sizeof(activated_channels));
		find_active_channels(channels, number_of_active_channels, activated_channels);

		char* scan_channels_string = static_cast<char*>(malloc(sizeof(char) * 20));
		memset(scan_channels_string, 0, sizeof(scan_channels_string));

		// When
		generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);
		free(activated_channels);

		// Then
		char* expected_string = "1,2";
		TS_ASSERT_EQUALS(scan_channels_string, expected_string);
		free(scan_channels_string);
	}

	void test_that_GIVEN_all_channels_active_THEN_the_expected_string_is_found(void) {
	// Given:
	int channels[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	int number_of_active_channels = 10;
	int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
	memset(activated_channels, 0, sizeof(activated_channels));
	find_active_channels(channels, number_of_active_channels, activated_channels);

	char* scan_channels_string = static_cast<char*>(malloc(sizeof(char) * 20));
	memset(scan_channels_string, 0, sizeof(scan_channels_string));

	// When
	generate_scan_channel_string(activated_channels, number_of_active_channels, scan_channels_string);
	free(activated_channels);

	// Then
	char* expected_string = "1,2,3,4,5,6,7,8,9,10";
	TS_ASSERT_EQUALS(scan_channels_string, expected_string);
	free(scan_channels_string);
	}

	void test_that_GIVEN_odd_channels_active_THEN_odd_channels_are_added_to_the_string(void) {
	// Given:
	int channels[10] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
	int number_of_active_channels = 5;
	int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
	memset(activated_channels, 0, sizeof(activated_channels));
	find_active_channels(channels, number_of_active_channels, activated_channels);

	char* scan_channels_string = static_cast<char*>(malloc(sizeof(char) * 20));
	memset(scan_channels_string, 0, sizeof(scan_channels_string));

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

		for (int i = 1; i < 10; i++) {
			int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
			memset(activated_channels, 0, sizeof(activated_channels));
			find_active_channels(channels, number_of_active_channels, activated_channels);

			char* scan_channels_string = static_cast<char*>(malloc(sizeof(char) * 20));
			memset(scan_channels_string, 0, sizeof(scan_channels_string));

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
		
		for (int i = 1; i < 10; i++) {
			int* activated_channels = static_cast<int*>(malloc(sizeof(int) * number_of_active_channels));
			memset(activated_channels, 0, sizeof(activated_channels));
			find_active_channels(channels, number_of_active_channels, activated_channels);

			char* scan_channels_string = static_cast<char*>(malloc(sizeof(char) * 20));
			memset(scan_channels_string, 0, sizeof(scan_channels_string));

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
};

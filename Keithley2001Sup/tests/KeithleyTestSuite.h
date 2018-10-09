// KeithleyTestSuite1.h

#include <cxxtest/TestSuite.h>
#include <stdlib.h>
#include <string>

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

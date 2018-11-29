#include <ios>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <vector>

#include "epicsTypes.h"
#include "gtest/gtest.h"
#include "..\src\buffer_parsing_utils.h"

namespace {

    TEST(SettingDoubles, test_that_GIVEN_channel_map_with_one_value_THEN_the_double_value_is_set) {
        // Given:
        std::map<int, void*> channel_output;
        double result = 0;
        void* pt = static_cast<void*>(&result);
        channel_output.insert(std::pair<int, void*>(1, pt));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "5"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            int channel = set_double_value(it, channel_output);
            ASSERT_TRUE(channel < 0);
        }

        // Then:
        double expected = 5;
        ASSERT_EQ(result, expected);
    }

    TEST(SettingDoubles, test_that_GIVEN_channel_map_with_multiple_values_THEN_the_double_values_are_set) {
        // Given:
        std::map<int, void*> channel_output;
        std::vector<void*> pointers(3);

        static const double arr[] = { 0,0,0 };
        std::vector<double> results(arr, arr + sizeof(arr) / sizeof(arr[0]));

        for (int i = 0; i < 3; ++i) {
            pointers[i] = static_cast<void*>(&(results[i]));
            channel_output.insert(std::pair<int, void*>(i, pointers[i]));
        }

        std::map<int, std::string> channel_readings;
        for (int i = 0; i < 3; ++i) {
            channel_readings.insert(std::pair<int, std::string>(i, "5"));
        }

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            int channel = set_double_value(it, channel_output);
            ASSERT_TRUE(channel < 0);
        }

        // Then:
        double expected = 5;
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQ(results[i], expected);
        }
    }

    TEST(SettingDoubles, test_that_GIVEN_an_channel_not_in_output_THEN_an_error_state_is_returned) {
        // Given:
        std::map<int, void*> channel_output;
        double result = 0;
        void* pt = static_cast<void*>(&result);
        channel_output.insert(std::pair<int, void*>(1, pt));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(0, "5"));

        int channel;
        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            channel = set_double_value(it, channel_output);
        }

        // Then:
        int expected_error_status = 0; // Channel 0 was not found
        ASSERT_EQ(channel, expected_error_status);
    }
} // namespace

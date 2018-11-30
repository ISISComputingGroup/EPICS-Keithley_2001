#include <ios>
#include <string>
#include <sstream>
#include <iomanip>
#include "gtest/gtest.h"

#include "..\src\buffer_parsing_utils.h"

namespace {
    TEST(Reading, test_that_GIVEN_a_reading_from_the_keithley_2001_with_read_and_unit_elements_THEN_the_reading_is_parsed_out) {
        // Given:
		double expected_result = 9.4;
		std::string expected_unit = "VDC";
		std::stringstream reading_stream;
		reading_stream << std::scientific << expected_result << expected_unit;
		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		// When:
		double result = parse_reading(buffer_reading);
		
		// Then:
		EXPECT_EQ(result, expected_result);
    }

    TEST(Channel, test_that_GIVEN_the_channel_part_of_a_reading_from_the_keithley_2001_with_chan_and_unit_elements_THEN_the_tenth_channel_is_found) {
        // Given:
		int expected_result = 10;
		std::string expected_unit = "INTCHAN";
		std::stringstream reading_stream;
		reading_stream << std::setfill('0') << std::setw(2) << expected_result << expected_unit;
		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);
		
		// When:
		int result = parse_channel(buffer_reading);
		
		// Then:
		EXPECT_EQ(result, expected_result);
    }

    TEST(Channel, test_that_GIVEN_the_channel_part_of_a_reading_from_the_keithley_2001_with_chan_and_unit_elements_THEN_the_first_channel_is_found) {
        // Given:
		int expected_result = 1;
		std::string expected_unit = "INTCHAN";
		std::stringstream reading_stream;
		reading_stream << std::setfill('0') << std::setw(2) << expected_result << expected_unit;

		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		// When:
		int result = parse_channel(buffer_reading);
		
		// Then:
		EXPECT_EQ(result, expected_result);
    }

    TEST(Unit, test_that_GIVEN_a_reading_from_the_keithley_2001_with_read_and_unit_elements_THEN_the_unit_is_parsed_as_VDC) {
		// Given:
		std::string expected_unit = "VDC";
        std::string reading = "9.412350E+00VDC";

		// When:
		std::string result = parse_reading_unit(reading);
		
		// Then:
		EXPECT_EQ(result, expected_unit);
    }

    TEST(Unit, test_that_GIVEN_a_reading_from_the_keithley_2001_with_read_and_unit_elements_THEN_the_unit_is_parsed_as_mVDC) {
        // Given:
        std::string expected_unit = "mVDC";
        std::string reading = "9.412350E+00mVDC";

        // When:
        std::string result = parse_reading_unit(reading);

        // Then:
        EXPECT_EQ(result, expected_unit);
    }
} // namespace

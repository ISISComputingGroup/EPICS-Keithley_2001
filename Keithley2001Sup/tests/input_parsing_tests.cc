#include <ios>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <iomanip>

#include "gtest/gtest.h"
#include "..\src\buffer_parsing_utils.h"



namespace {
    
    TEST(InputParsing, test_that_GIVEN_input_data_THEN_the_readings_are_categorized_by_channel) {
        // Given:
        const int MAX_NUMBER_OF_ELEMETNS_IN_BUFFER = 20;
        std::stringstream reading_stream;
        reading_stream << std::scientific << 9.41235 << "VDC";
        std::string reading_string = reading_stream.str();
        std::vector<std::string> channel_strings;

        for (int i = 0; i < 6; i += 2) {
            std::stringstream channel_stream;
            channel_stream << std::setfill('0') << std::setw(2) << i+1 << "INTCHAN";
            channel_strings.push_back(channel_stream.str());
        }

        const char* buffer_reading[MAX_NUMBER_OF_ELEMETNS_IN_BUFFER];

        for (int i = 0; i < 6; i += 2) {

            buffer_reading[i] = reading_string.c_str();
            buffer_reading[i+1] = channel_strings[i / 2].c_str();
        }

        for (int i = 6; i < MAX_NUMBER_OF_ELEMETNS_IN_BUFFER; i++) {
            buffer_reading[i] = "0";
        }
        
        // When:
        std::map<int, std::string> result = parseInput(buffer_reading, MAX_NUMBER_OF_ELEMETNS_IN_BUFFER);

        // Then:
        std::map<int, std::string> expected;
        expected.insert(std::pair<int, std::string>(1, reading_stream.str()));
        expected.insert(std::pair<int, std::string>(3, reading_stream.str()));        
        expected.insert(std::pair<int, std::string>(5, reading_stream.str()));        

        ASSERT_EQ(result, expected);
    }

    TEST(InputParsing, test_that_GIVEN_maximum_amount_of_input_data_THEN_the_readings_are_categorized_by_channel) {
        // Given:
        const int MAX_NUMBER_OF_ELEMETNS_IN_BUFFER = 6;
        std::stringstream reading_stream;
        reading_stream << std::scientific << 9.41235 << "VDC";
        std::string reading_string = reading_stream.str();
        std::vector<std::string> channel_strings;

        for (int i = 0; i < 6; i += 2) {
            std::stringstream channel_stream;
            channel_stream << std::setfill('0') << std::setw(2) << i + 1 << "INTCHAN";
            channel_strings.push_back(channel_stream.str());
        }

        const char* buffer_reading[MAX_NUMBER_OF_ELEMETNS_IN_BUFFER];

        for (int i = 0; i < 6; i += 2) {

            buffer_reading[i] = reading_string.c_str();
            buffer_reading[i + 1] = channel_strings[i / 2].c_str();
        }

        // When:
        std::map<int, std::string> result = parseInput(buffer_reading, MAX_NUMBER_OF_ELEMETNS_IN_BUFFER);

        // Then:
        std::map<int, std::string> expected;
        expected.insert(std::pair<int, std::string>(1, reading_stream.str()));
        expected.insert(std::pair<int, std::string>(3, reading_stream.str()));
        expected.insert(std::pair<int, std::string>(5, reading_stream.str()));

        ASSERT_EQ(result, expected);
    }
    
    /*TEST(InputParsing, test_that_GIVEN_input_data_and_a_length_that_is_too_long_THEN_the_readings_does_not_raise_an_out_if_bounds_exception) {
        // Given:
        std::stringstream reading_stream;
        reading_stream << std::scientific << 9.41235 << "VDC";
        std::string reading_string = reading_stream.str();
        std::vector<std::string> channel_strings;

        for (int i = 0; i < 6; i += 2) {
            std::stringstream channel_stream;
            channel_stream << std::setfill('0') << std::setw(2) << i + 1 << "INTCHAN";
            channel_strings.push_back(channel_stream.str());
        }

        const char* buffer_reading[6];

        for (int i = 0; i < 6; i += 2) {

            buffer_reading[i] = reading_string.c_str();
            buffer_reading[i + 1] = channel_strings[i / 2].c_str();
        }

        // When/Then:
        EXPECT_NO_THROW(parseInput(buffer_reading, 7));
    }*/

} // namespace

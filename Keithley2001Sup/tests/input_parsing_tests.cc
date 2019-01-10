#include <ios>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <iomanip>

#include "epicsTypes.h"
#include "gtest/gtest.h"
#include "buffer_parsing_utils.h"

namespace {
    
    TEST(InputParsing, test_that_GIVEN_input_data_THEN_the_readings_are_categorized_by_channel) {
        // Given:
        const int MAX_NUMBER_OF_ELEMETNS_IN_BUFFER = 10;
        epicsOldString value = "9.412350E+00VDC";
        std::string s(value, sizeof(epicsOldString));

        epicsOldString buffer_reading[MAX_NUMBER_OF_ELEMETNS_IN_BUFFER] = {
            "9.412350E+00VDC", "01INTCHAN", 
            "9.412350E+00VDC", "03INTCHAN", 
            "9.412350E+00VDC", "05INTCHAN",
            0,0,0,0
        };

        // When:
        std::map<int, std::string> result = parse_input(static_cast<void*>(buffer_reading), MAX_NUMBER_OF_ELEMETNS_IN_BUFFER);

        // Then:
        std::map<int, std::string> expected;
        expected.insert(std::pair<int, std::string>(1, s));
        expected.insert(std::pair<int, std::string>(3, s));
        expected.insert(std::pair<int, std::string>(5, s));

        ASSERT_EQ(result, expected);
    }

    TEST(InputParsing, test_that_GIVEN_maximum_amount_of_input_data_THEN_the_readings_are_categorized_by_channel) {
        // Given:
        const int MAX_NUMBER_OF_ELEMETNS_IN_BUFFER = 6;
        epicsOldString value = "9.412350E+00VDC";
        std::string s(value, sizeof(epicsOldString));

        epicsOldString buffer_reading[MAX_NUMBER_OF_ELEMETNS_IN_BUFFER] = {
            "9.412350E+00VDC", "01INTCHAN",
            "9.412350E+00VDC", "03INTCHAN",
            "9.412350E+00VDC", "05INTCHAN",
        };

        // When:
        std::map<int, std::string> result = parse_input(static_cast<void*>(buffer_reading), MAX_NUMBER_OF_ELEMETNS_IN_BUFFER);

        // Then:
        std::map<int, std::string> expected;
        expected.insert(std::pair<int, std::string>(1, s));
        expected.insert(std::pair<int, std::string>(3, s));
        expected.insert(std::pair<int, std::string>(5, s));

        ASSERT_EQ(result, expected);
    }

    TEST(InputParsing, test_that_GIVEN_empty_input_data_THEN_the_parsed_map_is_empty) {
        // Given:
        const int MAX_NUMBER_OF_ELEMETNS_IN_BUFFER = 10;

        epicsOldString buffer_reading[MAX_NUMBER_OF_ELEMETNS_IN_BUFFER] = {
            "", "", "", "", "", "", "", "", "", ""
        };

        // When:
        std::map<int, std::string> result = parse_input(static_cast<void*>(buffer_reading), MAX_NUMBER_OF_ELEMETNS_IN_BUFFER);

        // Then:
        ASSERT_TRUE(result.empty());
    }
} // namespace

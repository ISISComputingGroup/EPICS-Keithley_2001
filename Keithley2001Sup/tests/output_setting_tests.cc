#include <ios>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <vector>
#include <stdexcept>

#include <menuFtype.h>
#include "epicsTypes.h"

#include "gtest/gtest.h"
#include "buffer_parsing_utils.h"

namespace {

    TEST(SettingDoubles, test_that_GIVEN_channel_map_with_one_value_THEN_the_double_value_is_set) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        double result = 0;
        void* pt = static_cast<void*>(&result);
        
        menuFtype output_type = menuFtypeDOUBLE;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));
        
        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "5"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            set_double_value(it, channel_output);
        }

        // Then:
        double expected = 5;
        ASSERT_EQ(result, expected);
    }

    TEST(SettingDoubles, test_that_GIVEN_channel_map_with_multiple_values_THEN_the_double_values_are_set) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        menuFtype output_type = menuFtypeDOUBLE;
        std::vector<void*> pointers(3);

        std::vector<double> results(3);

        for (int i = 0; i < 3; ++i) {
            pointers[i] = static_cast<void*>(&(results[i]));
            channel_output.insert(std::pair<int, aSubOutputParameters>(i+1, aSubOutputParameters(pointers[i], output_type)));
        }

        std::map<int, std::string> channel_readings;
        for (int i = 0; i < 3; ++i) {
            channel_readings.insert(std::pair<int, std::string>(i+1, "5"));
        }

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            set_double_value(it, channel_output);
        }

        // Then:
        double expected = 5;
        for (int i = 0; i < 3; ++i) {
            ASSERT_EQ(results[i], expected);
        }
    }

    TEST(SettingDoubles, test_that_GIVEN_an_channel_not_in_output_THEN_an_invalid_argument_exception_is_thrown) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        double result = 0;
        void* pt = static_cast<void*>(&result);
        menuFtype output_type = menuFtypeDOUBLE;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(0, "5"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            // Then:
            ASSERT_THROW(set_double_value(it, channel_output), std::invalid_argument);
        }
    }

    TEST(SettingDoubles, test_that_GIVEN_an_output_channel_with_the_wrong_output_type_THEN_an_invalid_argument_exception_is_thrown){
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        double result = 0;
        void* pt = static_cast<void*>(&result);
        menuFtype output_type = menuFtypeSTRING;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "5"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            // Then:
            ASSERT_THROW(set_double_value(it, channel_output), std::invalid_argument);
        }
    }

    TEST(SettingUnits, test_that_GIVEN_channel_map_with_one_value_THEN_the_unit_is_set) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        epicsOldString result;
        void* pt = static_cast<void*>(&result);

        menuFtype output_type = menuFtypeSTRING;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "9.4000E+00VDC"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            set_unit_value(it, channel_output);
        }

        // Then:
        std::string expected = "VDC";
        ASSERT_STREQ(result, expected.c_str());
    }

    TEST(SettingUnits, test_that_GIVEN_channel_map_with_multiple_values_THEN_the_units_are_set) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        menuFtype output_type = menuFtypeSTRING;
        std::vector<void*> pointers(3);
        epicsOldString results[3];

        for (int i = 0; i < sizeof(results)/sizeof(epicsOldString); ++i) {
            pointers[i] = static_cast<void*>(&(results[i]));
            channel_output.insert(std::pair<int, aSubOutputParameters>(i + 1, aSubOutputParameters(pointers[i], output_type)));
        }

        std::map<int, std::string> channel_readings;
        for (int i = 0; i < 3; ++i) {
            channel_readings.insert(std::pair<int, std::string>(i + 1, "9.4000E+00VDC"));
        }

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            set_unit_value(it, channel_output);
        }

        // Then:
        std::string expected = "VDC";
        for (int i = 0; i < 3; ++i) {
            ASSERT_STREQ(results[i], expected.c_str());
        }
    }

    TEST(SettingUnits, test_that_GIVEN_an_channel_not_in_output_THEN_an_invalid_argument_exception_is_thrown) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        epicsOldString result;
        void* pt = static_cast<void*>(&result);
        menuFtype output_type = menuFtypeSTRING;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "9.4000E+00VDC"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            // Then:
            ASSERT_THROW(set_double_value(it, channel_output), std::invalid_argument);
        }
    }

    TEST(SettingUnits, test_that_GIVEN_an_output_channel_with_the_wrong_output_type_THEN_invalid_argument_exception_is_thrown) {
        // Given:
        std::map<int, aSubOutputParameters> channel_output;
        epicsOldString result;
        void* pt = static_cast<void*>(&result);
        menuFtype output_type = menuFtypeDOUBLE;

        channel_output.insert(std::pair<int, aSubOutputParameters>(1, aSubOutputParameters(pt, output_type)));

        std::map<int, std::string> channel_readings;
        channel_readings.insert(std::pair<int, std::string>(1, "9.4000E+00VDC"));

        // When:
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            // Then:
            ASSERT_THROW(set_unit_value(it, channel_output), std::invalid_argument);
        }
    }
} // namespace

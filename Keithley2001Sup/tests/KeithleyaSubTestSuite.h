// KeithleyaSubTestSuite.h
#ifndef KeithleyaSubTestSuite
#define KeithleyaSubTestSuite

#include <cxxtest/TestSuite.h>
#include <ios>
#include <string>
#include <sstream>
#include <iomanip>

#include "..\src\BufferParsingUtils.h"

class ParseBufferReadingTestSuite : public CxxTest::TestSuite {
public:
	void test_that_GIVEN_a_reading_from_the_keithley_2001_with_read_and_unit_elements_THEN_the_reading_is_parsed_out() 
	{
		double expected_result = 9.2;
		std::string expected_unit = "VDC";
		std::stringstream reading_stream;
		reading_stream << std::scientific << expected_result << expected_unit;

		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		double result = ParseReading(buffer_reading);
		TS_ASSERT_EQUALS(result, expected_result);
	}
};

class ParseBufferChannelTestSuite : public CxxTest::TestSuite {
public:
	void test_that_GIVEN_the_channel_part_of_a_reading_from_the_keithley_2001_with_chan_and_unit_elements_THEN_the_tenth_channel_is_found() 
	{
		int expected_result = 10;
		std::string expected_unit = "INTCHAN";
		std::stringstream reading_stream;
		reading_stream << std::setfill('0') << std::setw(2) << expected_result << expected_unit;

		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		int result = ParseChannel(buffer_reading);
		TS_ASSERT_EQUALS(result, expected_result);
	}

	void test_that_GIVEN_the_channel_part_of_a_reading_from_the_keithley_2001_with_chan_and_unit_elements_THEN_the_first_channel_is_found() 
	{
		int expected_result = 1;
		std::string expected_unit = "INTCHAN";
		std::stringstream reading_stream;
		reading_stream << std::setfill('0') << std::setw(2) << expected_result << expected_unit;

		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		int result = ParseChannel(buffer_reading);
		TS_ASSERT_EQUALS(result, expected_result);
	}
};


class ParseBufferUnitTestSuite : public CxxTest::TestSuite {
public:
	void test_that_GIVEN_a_reading_from_the_keithley_2001_with_read_and_unit_elements_THEN_the_unit_is_parsed() 
	{
		double expected_result = 9.2;
		std::string expected_unit = "VDC";
		std::stringstream reading_stream;
		reading_stream << std::scientific << expected_result << expected_unit;

		char buffer_reading[40];
		reading_stream.get(buffer_reading, 40);

		const char* result = ParseUnit(buffer_reading);
		TS_ASSERT_EQUALS(result, expected_unit);
	}
};
#endif //!KeithleyaSubTestSuite.h
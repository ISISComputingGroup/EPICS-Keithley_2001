#include "gtest/gtest.h"

int main(int argc, char **argv) {
        ::testing::InitGoogleTest( &argc, argv );
        ::testing::GTEST_FLAG(catch_exceptions) = false;
        return RUN_ALL_TESTS();
    }

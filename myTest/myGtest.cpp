#include "gtest/gtest.h"
#include "..\\headers.h"

TEST(sample_test_case, sample_test)
{
EXPECT_EQ(1, 1);
}

TEST(sample_test_case, testFilePath)
{
    ConverterJSON converterJson;
    EXPECT_EQ(3, converterJson.filePath.size());
}
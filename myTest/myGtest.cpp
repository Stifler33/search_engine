#include "gtest/gtest.h"
#include "..\\headers.h"
ConverterJSON converterJson;
TEST(sample_test_case, sample_test)
{
EXPECT_EQ(1, 1);
}

TEST(sample_test_case, GetResponsesLimit){

    cout << converterJson.GetResponsesLimit();
    EXPECT_EQ(5, converterJson.GetResponsesLimit());
}

TEST(sample_test_case, reading_text_files){
    for (auto i : converterJson.GetTextDocuments()){
        cout << i << " ";
    }
    cout << endl;
}
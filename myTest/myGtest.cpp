#include "gtest/gtest.h"
#include "../headers.h"
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
    EXPECT_EQ(converterJson.GetTextDocuments().size(), converterJson.jsonConfig["files"].size());
}

TEST(sample_test_case, reading_requests){
    for (auto i : converterJson.GetRequests()){
        cout << i << " ";
    }
    cout << endl;
    EXPECT_EQ(converterJson.jsonRequests["requests"].size(), converterJson.GetRequests().size());
}

TEST(sample_test_case, putAnswer){
    vector<vector<std::pair<int, float>>> answers = {{make_pair(0, 0.1f)}, {make_pair(1, 0.02f)},
                                                     {{make_pair(2, 0.1f)}}};
    converterJson.putAnswers(answers);
    cout << converterJson.jsonAnswer.dump(3) << endl;
}
TEST(sample_test_case, UpdateDocumentBase){
    InvertedIndex invertedIndex;
    ConverterJSON convert;
    invertedIndex.UpdateDocumentBase(convert.GetTextDocuments());
    invertedIndex.getDocks();
}
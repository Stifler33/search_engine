#include "gtest/gtest.h"
#include "../headers.h"
//ConverterJSON converterJson;

TEST(sample_test_case, UpdateDocumentBase){
    InvertedIndex invertedIndex;
    ConverterJSON convert;
    invertedIndex.UpdateDocumentBase(convert.GetTextDocuments());
    invertedIndex.getDocks();
}

TEST(sample_test_case, GetWordCount){
    InvertedIndex invertedIndex;
    ConverterJSON convert;
    invertedIndex.UpdateDocumentBase(convert.GetTextDocuments());
    invertedIndex.fill_dictionary();
    invertedIndex.getFreq();
}
TEST(sample_test_case, search){
    InvertedIndex invertedIndex;
    ConverterJSON convert;
    SearchServer search(invertedIndex);
    invertedIndex.UpdateDocumentBase(convert.GetTextDocuments());
    invertedIndex.fill_dictionary();
    search.search(convert.GetRequests());
}
void TestInvertedIndexFunctionality(
        const vector<string>& docs,
        const vector<string>& requests,
        const std::vector<vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            },
            {
                    {0, 1}, {1, 3}
            }};
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "Americano Cappuccino"
    };
    const vector<string> requests = {"milk", "water", "Cappuccino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
                    {}
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water sugar sugar",
            "milk milk milk milk milk water water water water water",
            "Americano Cappuccino"
    };
    const vector<string> request = {"milk water milk water", "sugar Cappuccino"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
                    {}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    idx.fill_dictionary();
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}
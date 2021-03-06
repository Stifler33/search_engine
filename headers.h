//
// Created by bagi3 on 04.04.2022.
//

#ifndef SEARCH_ENGINE_HEADERS_H
#define SEARCH_ENGINE_HEADERS_H
//#include "nlohmann/json.hpp"
#include "cmake-build-debug/_deps/json-src/single_include/nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
using json = nlohmann::json;
using namespace std;

class ConverterJSON
{
public:
    ConverterJSON();
    ~ConverterJSON();

    /**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    vector<string> GetTextDocuments();
/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit();
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    vector<string> GetRequests();
/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(vector<vector<pair<int, float>>> answers);

    json jsonAnswer ;
    json jsonConfig;
    json jsonRequests;
    ifstream readingFile;
    ofstream writeFile;
    array<string, 3> filePath =
            {
                    "../jsonFiles/config.json",
                    "../jsonFiles/answers.json",
                    "../jsonFiles/requests.json"
            };
};
struct Entry {
    size_t doc_id, count;
// Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex{
public:
    InvertedIndex() = default;
    /**
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param texts_input содержимое документов
*/
    void UpdateDocumentBase(vector<string> input_docs);
/**
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    vector<Entry> GetWordCount(const string& word);
    void getDocks();
private:
    vector<string> docs; // список содержимого документов
    map<string, vector<Entry>> freq_dictionary; // частотный словарь
};

#endif //SEARCH_ENGINE_HEADERS_H

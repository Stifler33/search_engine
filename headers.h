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
    void putAnswers(vector<vector<pair<size_t, float>>> answers);

    json jsonAnswer ;
    json jsonConfig;
    json jsonRequests;
    ifstream readingFile;
    ofstream writeFile;
    array<string, 3> filePath =
            {{
                     "../jsonFiles/config.json",
                     "../jsonFiles/answers.json",
                     "../jsonFiles/requests.json"
             }};
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
    void fill_dictionary();
    void getDocks();
    void getFreq();
    const map<string, vector<Entry>> &_freq_dictionary = freq_dictionary;
private:
    vector<string> docs; // список содержимого документов
    map<string, vector<Entry>> freq_dictionary; // частотный словарь
};

struct RelativeIndex{
size_t doc_id;
float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
*
чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
InvertedIndex _index;
map<size_t, vector<string>> listUniqWords;
};
#endif //SEARCH_ENGINE_HEADERS_H

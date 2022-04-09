//
// Created by bagi3 on 04.04.2022.
//

#ifndef SEARCH_ENGINE_HEADERS_H
#define SEARCH_ENGINE_HEADERS_H
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <iostream>
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
    std::vector<std::string> GetTextDocuments();
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
    std::vector<std::string> GetRequests();
/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>
                    answers);


    void loadJson();
    json jsonAnswer ;
    json jsonConfig;
    json jsonRequests;
    ifstream readingFile;
    ofstream writeFile;
    array<string, 3> filePath =
            {
                    "..\\jsonFiles\\config.json",
                    "..\\jsonFiles\\answers.json",
                    "..\\jsonFiles\\requests.json"
            };

};
#endif //SEARCH_ENGINE_HEADERS_H

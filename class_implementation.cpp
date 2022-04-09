//
// Created by bagi3 on 04.04.2022.
//
#include "headers.h"

ConverterJSON::ConverterJSON() {
    for (auto i : filePath){
        auto nameFile = [&i](){
            return i.substr(i.rfind("\\") + 1, i.size());
        };
            if (nameFile() == "config.json"){
                readingFile.open(i);
                if(!readingFile.is_open()){
                    cout << "config file is empty\n";
                    break;
                }else {
                    readingFile >> jsonConfig;
                    jsonConfig.at("config");
                    cout << jsonConfig["config"]["name"] << endl;
                    readingFile.close();
                }
            };
            if (nameFile() == "answers.json"){
                readingFile.open(i);
                if (!readingFile.is_open()){
                    jsonAnswer["answers"]["requests"] = {{"result", false}};
                    jsonAnswer["answers"]["requests"]["relevance"][0] = {{"rank", 0.0}};
                    writeFile.open(i);
                    writeFile << jsonAnswer;
                    writeFile.close();
                }else {
                    readingFile >> jsonAnswer;
                    readingFile.close();
                }
            };
            if (nameFile() == "requests.json"){
                readingFile.open(i);
                if (!readingFile.is_open()){
                    writeFile.open(i);
                    jsonRequests["requests"] = {"some words.."};
                    writeFile << jsonRequests;
                    writeFile.close();
                }else {
                    readingFile >> jsonRequests;
                    readingFile.close();
                }
            };
    }
}

ConverterJSON::~ConverterJSON() = default;
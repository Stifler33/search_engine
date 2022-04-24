//
// Created by bagi3 on 04.04.2022.
//
#include "headers.h"

ConverterJSON::ConverterJSON() {
    for (auto i : filePath){
        auto nameFile = [&i](){
            return i.substr(i.rfind("/") + 1, i.size());
        };
            if (nameFile() == "config.json"){
                readingFile.open(i);
                if(!readingFile.is_open()){
                    cerr << "config file is empty\n";
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
                    jsonAnswer["answers"]["requests"]["relevance"] = {{"docid", 0}, {"rank", 0.0f}};
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
int ConverterJSON::GetResponsesLimit(){
    return jsonConfig["config"]["max_responses"];
}
vector<string>ConverterJSON::GetTextDocuments(){
    vector<string> textDocuments;
    for (const auto &nameFile : jsonConfig["files"]){
        readingFile.open(nameFile);
        textDocuments.emplace_back("");
        if (!readingFile.is_open()){
            cerr << "file " << nameFile << " not open !\n";
        }else{
            char c;
            while (readingFile.get(c)){
                textDocuments.back() += c;
            }
            readingFile.close();
        }
    }
    return textDocuments;
}
vector<string> ConverterJSON::GetRequests(){
    vector<string> listRequests;
    for (const auto &request : jsonRequests["requests"]){
        listRequests.push_back(request);
    }
    return listRequests;
}
void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> answers) {
    for (int numberRequest = 0; numberRequest < answers.size(); numberRequest++){
        string nameRequest = "requests" + to_string(numberRequest);
        for (auto &relevance : answers[numberRequest]){
            jsonAnswer["answers"][nameRequest]["relevance"]["docid"] = relevance.first;
            jsonAnswer["answers"][nameRequest]["relevance"]["rank"] = relevance.second;
        }
    }
    writeFile.open(filePath[1]);
    writeFile << jsonAnswer;
    writeFile.close();
}
ConverterJSON::~ConverterJSON() = default;

void InvertedIndex::UpdateDocumentBase(vector<string> input_docs) {
    stringstream inputStream ;
    initializer_list<char> marks = {'.', ',', '!', '-', '_', '?', ':', ';', '\r', '\n', '\\', '\t'};
    for (auto &part_docs : input_docs){
        docs.emplace_back("");
        for (auto letter = part_docs.begin(); letter < part_docs.end(); letter++){
            if (!any_of(marks.begin(), marks.end(), [letter](char mr){return mr == *letter;})){
                docs.back().push_back(*letter);
            }
        }
    }
}

void InvertedIndex::getDocks() {
    for (auto &i : docs){
            cout << "\"" << i << "\"";
        cout << endl;
    }
}



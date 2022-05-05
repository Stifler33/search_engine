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

vector<Entry> InvertedIndex::GetWordCount(const string &word) {
    vector<Entry> bufferEntry;
    for (size_t id = 0; id < docs.size(); id++){
        size_t count = 0;
        stringstream currentStream(docs[id]);
        string wordFromStream;
        while (!currentStream.eof()){
            currentStream >> wordFromStream;
            if (wordFromStream == word) count++;
        }
        if (count != 0) bufferEntry.push_back({id, count});
    }
    return bufferEntry;
}

void InvertedIndex::fill_dictionary() {
    for (auto &doc : docs){
        stringstream streamDoc(doc);
        while (!streamDoc.eof()){
            string word;
            streamDoc >> word;
            freq_dictionary[word] = GetWordCount(word);
        }
    }
}

void InvertedIndex::getFreq() {
    for (auto &freq : freq_dictionary){
        cout << freq.first << " ";
        for (auto entry : freq.second){
            cout << "{" <<entry.doc_id << ", " << entry.count << "}";
        }
        cout << endl;
    }
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(const vector<std::string> &queries_input) {
    vector<vector<RelativeIndex>> relativeIndices;
    for (auto &request : queries_input){
        stringstream requestStream(request);
        string queryWord;
        vector<pair<string, vector<Entry>>> queryWords;
        while (!requestStream.eof()){
            requestStream >> queryWord;
            auto foundElement = _index._freq_dictionary.find(queryWord);
            if (foundElement != _index._freq_dictionary.end()){
                queryWords.emplace_back(*foundElement);
            }
        }
        for (auto a = queryWords.begin(); a != queryWords.end(); a++){
            for (auto b = queryWords.begin(); b != queryWords.end(); b++){
                if (a->second.size() < b->second.size()){
                    auto c = *a;
                    *a = *b;
                    *b = c;
                }
            }
        }
        vector<size_t> docIdRare;
        for (auto docId : queryWords.begin()->second){
            docIdRare.push_back(docId.doc_id);
        }
        float rankAbsolute;
        for (auto &id : docIdRare){
            for (auto &entry : queryWords){
                for (auto &count : entry.second){
                    if (count.doc_id == id) {
                        rankAbsolute += count.count;
                    }
                }
            }

            for (auto i : relativeIndices){
                for (auto j : i){
                    if (j.doc_id == id){
                        j.rank += rankAbsolute;
                        break;
                    }
                }
                relativeIndices.push_back({{id, rankAbsolute}});
            }
            cout << "";
        }

    }
    return std::vector<std::vector<RelativeIndex>>();
}

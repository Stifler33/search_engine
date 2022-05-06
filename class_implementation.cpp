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
void ConverterJSON::putAnswers(vector<vector<pair<size_t, float>>> answers) {
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
    float maxAbsoluteRelevance = 0;
    for (auto &request: queries_input) {
        vector<RelativeIndex> listRelativeIndex;
        //получаем список уникальных слов
        vector<string> uniqueWords;
        stringstream requestStream(request);
        string queryWord;
        while (!requestStream.eof()) {
            requestStream >> queryWord;
            if (find(uniqueWords.begin(), uniqueWords.end(), queryWord) == uniqueWords.end()){
                uniqueWords.push_back(queryWord);
            }
        }
        //сортировать в порядке частоты встречаемости в документах
        vector<size_t> freqWord;
        for (auto &word : uniqueWords){
            size_t count = 0;
            for (auto entry : _index.GetWordCount(word)){
                count += entry.count;
            }
            freqWord.push_back(count);
        }
        for (int id_a = 0; id_a < freqWord.size(); id_a++){
            for (int id_b = 0; id_b < freqWord.size(); id_b++){
                if (id_a > id_b){
                    auto valueFreq = freqWord[id_a];
                    freqWord[id_a] = freqWord[id_b];
                    freqWord[id_b] = valueFreq;
                    auto valueWord = uniqueWords[id_a];
                    uniqueWords[id_a] = uniqueWords[id_b];
                    uniqueWords[id_b] = valueWord;
                }
            }
        }
        //по самому редкому слову найти документы в которых оно встречается.
        vector<size_t> docWithWords;
        for (auto i : _index.GetWordCount(*uniqueWords.begin())){
            docWithWords.push_back(i.doc_id);
        }
        //в этих документах найти остальные слова из запроса и посчитать их абсолютную релевантность
        vector<RelativeIndex> relativeRequest;
        for (auto word : uniqueWords){
            for (auto idDoc: docWithWords){
                auto entry = _index.GetWordCount(word);
                for (auto i : entry){
                    if (i.doc_id == idDoc){
                        if (!any_of(relativeRequest.begin(), relativeRequest.end(), [idDoc, i](auto rel){
                            if (rel.doc_id == idDoc){
                                rel.rank += i.count;
                                return true;
                            }
                            return false;
                        })){
                            relativeRequest.push_back({idDoc, float(i.count)});
                        }
                    }
                }
            }
        }
        for (auto i : relativeRequest){
            if (i.rank > maxAbsoluteRelevance){
                maxAbsoluteRelevance = i.rank;
            }
        }
        relativeIndices.push_back(relativeRequest);
    }
    //Теперь посчитаем относительную релевантность
    for (auto &i : relativeIndices){
        for (auto &j : i){
            j.rank /= maxAbsoluteRelevance;
        }
    }
    return relativeIndices;
}
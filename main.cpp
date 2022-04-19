#include "headers.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    ConverterJSON convJson;
    for (auto i : convJson.GetTextDocuments()){
        cout <<"\"" << i << "\""<< endl;
    }
    cout << convJson.GetTextDocuments().size() << endl;
    cout << convJson.jsonConfig["files"].size();
    return 0;
}

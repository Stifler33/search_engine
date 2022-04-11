#include "headers.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    ConverterJSON convJson;
    for (auto i : convJson.GetTextDocuments()){
        cout << i << " ";
    }
    return 0;
}

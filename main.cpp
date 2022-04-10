#include "headers.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    ConverterJSON convJson;
    cout << convJson.jsonConfig.dump(3) << endl;
    cout << convJson.jsonRequests.dump(3) << endl;
    cout << convJson.jsonAnswer.dump(3) << endl;
    return 0;
}

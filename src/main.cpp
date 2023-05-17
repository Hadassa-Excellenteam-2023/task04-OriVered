#include <fstream>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <string>
#include <utility>
#include <stdexcept>
#include <iostream>
#include "Controller.h"

const std::string DATA_FILE = "data.txt";

int main() {
    try {
        Controller controller;
        controller.load_data(DATA_FILE);
        controller.run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << '\n';

    }
    return EXIT_FAILURE;
}

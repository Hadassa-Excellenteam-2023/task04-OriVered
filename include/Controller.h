#include <unordered_map>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <map>

/* Namespcace */
using Coordinates = std::pair<double, double>;
using CityMap = std::unordered_map<std::string, Coordinates>;
using SortedCityMap = std::multimap<double, std::string>;
using NormFunction = std::function<double(double, double)>;
using FunctionMap = std::map<int, NormFunction>;

class Controller {
public:
    /* Constants */
    static constexpr const char* OPEN_FILE_ERROR = "Failed to open file";
    static constexpr const char* READ_LINE_ERROR = "Failed to read line";
    static constexpr const char* PARSE_COORDINATES_ERROR = "Failed to parse coordinates";
    static constexpr const char* READ_FILE_ERROR = "Error occurred while reading file";
    static constexpr const char* BYE = "0";
    static constexpr const char* INPUT_FAILURE = "Failed to read input";
    static constexpr const char* ERROR_PREFIX = "ERROR: ";
    static constexpr const char* ERROR_SUFFIX = ". Please try again.\n";
    static constexpr const char* SEARCH_RESULT_PROMPT = "Search result:\n";
    static constexpr const char* CITY_COUNT_PROMPT = " city/cities found in the given radius.\n";
    static constexpr const char* NORTH_COUNT_PROMPT = " cities are to the north of the selected city.\n";
    static constexpr const char* CITY_LIST_PROMPT = "City list:\n";
    static constexpr const char* BYE_PROMPT = "Bye\n";
    static constexpr const char* CITY_PROMPT = "Please enter selected city name (with line break after it), or enter '0' to exit:";
    static constexpr const char* CITY_NOT_FOUND = "City not found in the city list";
    static constexpr const char* RADIUS_PROMPT = "Please enter the wanted radius:";
    static constexpr const char* INVALID_INPUT = "Invalid input";
    static constexpr const char* NORM_PROMPT = "Please enter the wanted norm (0 - L2, Euclidean distance, 1 - Linf, Chebyshev distance, 2 - L1, Manhattan distance):";
    static constexpr const char* INVALID_NORM = "Invalid norm";

    static FunctionMap NORM_FUNCTIONS;

    /* Constructor */
    Controller() = default;

    /* Loads city data from a file */
    void load_data(const std::string& file_path);

    /* Runs the controller */
    void run();




private:
    /* Prive functions */

    /* Returns cities within a radius of a given city */
    SortedCityMap getCitiesInRadius(const std::string& city, double radius, NormFunction normFunction);

    /* Counts cities north of a given city */
    _int64 countCitiesNorthOf(const std::string& city, const SortedCityMap& citiesInRadius);

    /* Prints search results */
    void printSearchResults(const SortedCityMap& citiesInRadius, _int64 northCount);

    /* Validates a city name */
    std::string validateCity();

    /* Validates a radius value */
    double validateRadius();

    /* Validates a norm value */
    NormFunction validateNorm();

    /* Computes the distance between two coordinates using a given norm function */ 
    double computeDistance(NormFunction norm, const Coordinates& coord1, const Coordinates& coord2);

    template<typename T>
    T validateInput(const std::string& prompt, const std::string& error, std::function<bool(T)> isValid, std::function<void(T&)> readFunc);

    /* Prive members */
    CityMap m_city_coords;
};

/* Template function for input validation */ 
template <typename T>
T Controller::validateInput(const std::string& prompt, const std::string& error, std::function<bool(T)> isValid, std::function<void(T&)> readFunc) {
    T input{};
    while (true) {
        std::cout << prompt << std::endl;
        readFunc(input);
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error(Controller::INPUT_FAILURE);
        }
        else if (!isValid(input)) {
            std::cout << Controller::ERROR_PREFIX << error << Controller::ERROR_SUFFIX;
        }
        else {

            break;
        }
    }
    std::cout << std::endl;
    return input;
}
#include "Controller.h"
#include <stdexcept>

// Function to load city data from a file
void Controller::load_data(const std::string& file_path) {
    std::ifstream file(file_path); // Open the file
    if (!file.is_open()) { // Check if the file opening failed
        throw std::runtime_error(Controller::OPEN_FILE_ERROR); // Throw a runtime_error with the error message
    }

    std::string line;
    while (std::getline(file, line)) { // Read each line from the file
        const std::string cityName = line; // The first line is the city name

        if (!std::getline(file, line)) { // Read the next line containing the coordinates
            throw std::runtime_error(READ_LINE_ERROR); // Throw an error if the line reading failed
        }

        double lat, lon;
        std::istringstream iss(line); // Create a string stream to parse the coordinates
        if (!(iss >> lat && iss.ignore(3) && iss >> lon)) { // Parse the latitude and longitude
            throw std::runtime_error(PARSE_COORDINATES_ERROR); // Throw an error if the parsing failed
        }

        m_city_coords[cityName] = std::make_pair(lat, lon); // Add the city coordinates to the city map
    }

    if (file.bad()) { // Check if there was an error while reading the file
        throw std::runtime_error(READ_FILE_ERROR); // Throw an error indicating file read failure
    }
}

// Norm functions initialization
FunctionMap Controller::NORM_FUNCTIONS = {
    {0, [](double x, double y) { return std::sqrt(x * x + y * y); }}, // L2 norm function (Euclidean distance)
    {1, [](double x, double y) { return std::max(std::abs(x), std::abs(y)); }}, // Linfinity norm function (Chebyshev distance)
    {2, [](double x, double y) { return std::abs(x) + std::abs(y); }} // L1 norm function (Manhattan distance)
};

// Function to compute distance between two coordinates using a specified norm function
double Controller::computeDistance(NormFunction norm, const Coordinates& coord1, const Coordinates& coord2) {
    double dx = std::abs(coord1.first - coord2.first); // Compute the difference in latitude
    double dy = std::abs(coord1.second - coord2.second); // Compute the difference in longitude
    return norm(dx, dy); // Compute the distance using the specified norm function
}

// Main function to run the controller
void Controller::run() {
    std::string line;

    while (true) {
        std::string city = validateCity(); // Validate and get the selected city
        if (city == BYE) { // Check if the user wants to exit
            break;
        }

        double radius = validateRadius(); // Validate and get the desired radius
        NormFunction normFunction = validateNorm(); // Validate and get the desired norm function

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any remaining input

        SortedCityMap citiesInRadius = getCitiesInRadius(city, radius, normFunction); // Get cities within the radius
        _int64 northCount = countCitiesNorthOf(city, citiesInRadius); // Count cities north of the selected city

        printSearchResults(citiesInRadius, northCount); // Print the search results
    }

    std::cout << BYE_PROMPT; // Print the exit prompt
}

// Function to get cities within a specified radius of a given city
SortedCityMap Controller::getCitiesInRadius(const std::string& city, double radius, NormFunction normFunction) {
    std::vector<std::pair<double, std::string>> citiesAndDistances;

    // Compute the distance between the selected city and all other cities
    std::transform(m_city_coords.begin(), m_city_coords.end(), std::back_inserter(citiesAndDistances),
        [&](const std::pair<std::string, Coordinates>& cityData) {
            double distance = computeDistance(normFunction, m_city_coords[city], cityData.second);
            return std::make_pair(distance, cityData.first);
        });

    // Remove cities that are outside the specified radius or are the same as the selected city
    citiesAndDistances.erase(std::remove_if(citiesAndDistances.begin(), citiesAndDistances.end(),
        [&](const std::pair<double, std::string>& cityData) {
            return cityData.first > radius || cityData.second == city;
        }), citiesAndDistances.end());

    // Create a multimap from the vector to sort the cities by distance
    SortedCityMap citiesInRadius(citiesAndDistances.begin(), citiesAndDistances.end());

    return citiesInRadius;
}

// Function to count cities north of a given city
_int64 Controller::countCitiesNorthOf(const std::string& city, const SortedCityMap& citiesInRadius) {
    _int64 northCount = std::count_if(citiesInRadius.begin(), citiesInRadius.end(), [&](const std::pair<const double, std::string>& cityData) {
        return m_city_coords[cityData.second].second > m_city_coords[city].second;
        });

    return northCount;
}

// Function to print the search results
void Controller::printSearchResults(const SortedCityMap& citiesInRadius, _int64 northCount) {
    std::cout << SEARCH_RESULT_PROMPT;
    std::cout << citiesInRadius.size() << CITY_COUNT_PROMPT;
    std::cout << northCount << NORTH_COUNT_PROMPT;
    std::cout << CITY_LIST_PROMPT;

    // Print the city names in the search results
    std::transform(citiesInRadius.begin(), citiesInRadius.end(),
        std::ostream_iterator<std::string>(std::cout, "\n"),
        [](const std::pair<double, std::string>& cityData) {
            return cityData.second;
        });
}

// Function to validate a city name
std::string Controller::validateCity() {
    return validateInput<std::string>(
        Controller::CITY_PROMPT,
        Controller::CITY_NOT_FOUND,
        [this](const std::string& city) { return city == "0" || m_city_coords.find(city) != m_city_coords.end(); },
        [](std::string& input) { std::getline(std::cin, input); });
}

// Function to validate a radius value
double Controller::validateRadius() {
    return validateInput<double>(
        Controller::RADIUS_PROMPT,
        Controller::INVALID_INPUT,
        [](double radius) { return radius >= 0; }, // Assume the radius should be non-negative
        [](double& input) { std::cin >> input; });
}

// Function to validate a norm value
NormFunction Controller::validateNorm() {
    int normIndex = validateInput<int>(
        Controller::NORM_PROMPT,
        Controller::INVALID_NORM,
        [](int norm) { return NORM_FUNCTIONS.find(norm) != NORM_FUNCTIONS.end(); },
        [](int& input) { std::cin >> input; });
    return NORM_FUNCTIONS.at(normIndex);
}

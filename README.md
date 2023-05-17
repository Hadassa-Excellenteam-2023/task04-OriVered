# task04-OriVered
# City Search Program

This program allows users to search for cities within a specified radius of a selected city and count the number of cities located north of the selected city. It utilizes various data structures for efficient runtime.

## How to Run

To run the program, follow these steps:

1. Ensure that you have a file named "city_data.txt" in the same directory as the program. The file should contain the city data in the following format:
   - Each city name should be on a separate line.
   - The coordinates of each city should be on the next line, separated by a space. The latitude and longitude values should be represented as `double` values.

2. Compile the program using a C++ compiler. For example, you can use the following command with g++:
   - `g++ main.cpp -o city_search`

3. Execute the program by running the compiled binary:
   - `./city_search`

4. The program will prompt you to enter a city name, radius, and norm. Follow the prompts and provide the required input.
   - Enter the name of the selected city (or '0' to exit).
   - Enter the desired radius for the search.
   - Enter the index of the desired norm (0 for L2, 1 for Linf, 2 for L1).

5. After entering the input, the program will display the search results, including the number of cities found within the radius, the number of cities located north of the selected city, and a list of the cities found.

6. Repeat steps 4 and 5 to perform additional searches or exit the program by entering '0' as the city name.


## Data Structures

The program uses the following data structures:

### `Coordinates`

The `Coordinates` structure represents the latitude and longitude of a city. It is defined as a pair of `double` values.

### `CityMap`

The `CityMap` is an unordered map that stores the city names as keys and their corresponding coordinates as values. This data structure allows for efficient lookup of city coordinates based on the city name.

### `SortedCityMap`

The `SortedCityMap` is a multimap that stores cities sorted by their distances from the selected city. It associates a distance value with each city, allowing for efficient sorting and retrieval of cities within a specified radius.

### `NormFunction`

The `NormFunction` is a function type that represents a distance calculation method. It takes two `double` parameters (x and y) and returns the distance between them based on a specific norm.

### `FunctionMap`

The `FunctionMap` is a map that associates an integer index with a `NormFunction`. It allows users to select a norm for distance calculation by specifying the corresponding index.

## Reasoning

The chosen data structures provide efficient runtime for the following reasons:

- `CityMap` (unordered map): It allows for constant-time lookup of city coordinates based on the city name, providing fast retrieval when computing distances and filtering cities.

- `SortedCityMap` (multimap): By associating a distance value with each city, the program can efficiently sort cities by their distances from the selected city. This data structure simplifies the process of retrieving cities within a specified radius.

- `NormFunction` (function type): By using a function type, the program can easily switch between different distance calculation methods (L2, Linf, L1) based on user input. The selection is made by mapping an integer index to the corresponding `NormFunction` in the `FunctionMap`.

By leveraging these data structures, the program achieves efficient lookup, sorting, and filtering of cities, resulting in a faster and more optimized execution.



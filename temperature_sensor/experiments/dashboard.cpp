#include <iostream>
#include <iomanip>

int main() {
    // Example data
    const int rows = 4;
    const int cols = 3;
    std::string headers[cols] = {"Name", "Age", "City"};
    std::string data[rows][cols] = {
        {"Alice", "30", "New York"},
        {"Bob", "25", "Los Angeles"},
        {"Charlie", "35", "Chicago"},
        {"David", "40", "Houston"}
    };

    // Print headers
    for (int i = 0; i < cols; ++i) {
        std::cout << std::setw(15) << headers[i];
    }
    std::cout << std::endl;

    // Print data
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(15) << data[i][j];
        }
        std::cout << std::endl;
    }

    return 0;
}
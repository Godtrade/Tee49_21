#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
   
    if (argc == 1) {
        std::cout << "Please input numbers to find average." << std::endl;
        return 0;
    }

    double sum = 0.0;
    int count = argc - 1;

    for (int i = 1; i < argc; ++i) {
        sum += std::stod(argv[i]); 
    }

    double average = sum / count;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "Average of " << count << " numbers = " << average << std::endl;
    std::cout << "---------------------------------" << std::endl;

    return 0;
}

// Программа 1: сколько чисел массива кратны хотя бы одному из чисел 2..9.
//
// Запуск:  count_divisible <входной_файл> [выходной_файл]
// Входной файл: n, затем n целых чисел. Результат печатается на экран
// и (если указан второй аргумент) записывается в файл.

#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "algorithms.hpp"
#include "io.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [output_file]\n";
        return 2;
    }
    try {
        std::ifstream in(argv[1]);
        if (!in) {
            throw std::runtime_error(std::string("cannot open input file: ") + argv[1]);
        }
        const lab::Array<long long> a = lab::read_array(in);
        const std::size_t result = lab::count_divisible(a);

        std::cout << result << '\n';
        if (argc == 3) {
            std::ofstream out(argv[2]);
            if (!out) {
                throw std::runtime_error(std::string("cannot open output file: ") + argv[2]);
            }
            out << result << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

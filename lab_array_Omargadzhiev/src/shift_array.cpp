// Программа 2: сдвиг элементов массива влево или вправо на заданное число шагов.
//
// Запуск:  shift_array <входной_файл> [выходной_файл]
// Входной файл: n, затем n целых чисел, затем направление (left/right)
// и число шагов. Освободившиеся ячейки заполняются нулями.

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
        lab::Array<long long> a = lab::read_array(in);
        const lab::Direction dir = lab::read_direction(in);
        const std::size_t steps = lab::read_steps(in);

        lab::shift(a, dir, steps);

        lab::print_array(std::cout, a);
        if (argc == 3) {
            std::ofstream out(argv[2]);
            if (!out) {
                throw std::runtime_error(std::string("cannot open output file: ") + argv[2]);
            }
            lab::print_array(out, a);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

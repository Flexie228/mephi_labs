#include <vector>
#include "UI.hpp"
#include "Benchmark.hpp"
using namespace std;

int main() {
    // BENCHMARK

    vector<size_t> test_sizes = {1000, 10000, 50000, 100000, 500000, 1000000, 2000000, 3000000};
    Benchmark<NewDeque<int>>("NewDeque", test_sizes);
    Benchmark<ArrayDeque<int>>("ArrayDeque", test_sizes);

    /*
    DequeUI ui;
    ui.Run();
    */
    return 0;
}
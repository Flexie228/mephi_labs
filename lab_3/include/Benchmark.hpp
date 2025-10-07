#ifndef BENCHMARK
#define BENCKMARK

#include <vector>
#include <string>
#include "Deque/ArrayDeque.hpp"
#include "Deque/NewDeque.hpp"

template<typename DequeType>
void Benchmark(const std::string& name, const std::vector<size_t>& test_sizes);

#endif
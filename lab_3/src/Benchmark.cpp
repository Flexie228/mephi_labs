#include <iostream>
#include <chrono>
#include <cassert>
#include "Deque/ArrayDeque.hpp"
#include "Deque/NewDeque.hpp"

using namespace std;
using namespace std::chrono;

template<typename DequeType>
void Benchmark(const string& name, const vector<size_t>& test_sizes) {
    cout << "Benchmarking " << name << ":\n";

    for (size_t size : test_sizes) {
        vector<int> test_data(size);
        for (size_t i = 0; i < size; ++i) {
            test_data[i] = i;
        }

        DequeType deque;
        // Тест PushBack

        auto start = high_resolution_clock::now();
        for (const auto& item : test_data) {
            deque.PushBack(item);
        }
        auto end = high_resolution_clock::now();
        auto push_back_time = duration_cast<milliseconds>(end - start).count();
        
        // Тест PushFront

        start = high_resolution_clock::now();
        for (size_t i = 0; i < size; ++i) {
            deque.PushFront(i);
        }
        end = high_resolution_clock::now();
        auto push_front_time = duration_cast<milliseconds>(end - start).count();
        
        // Тест PopBack

        start = high_resolution_clock::now();
        while (!deque.IsEmpty()) {
            deque.PopBack();
        }
        end = high_resolution_clock::now();
        auto pop_back_time = duration_cast<milliseconds>(end - start).count();
        
        // Тест PopFront

        for (const auto& item : test_data) {
            deque.PushBack(item);
        }
        start = high_resolution_clock::now();
        while (!deque.IsEmpty()) {
            deque.PopFront();
        }
        end = high_resolution_clock::now();
        auto pop_front_time = duration_cast<milliseconds>(end - start).count();
        
        // Тест доступа по индексу

        for (const auto& item : test_data) {
            deque.PushBack(item);
        }
        int sum = 0;
        start = high_resolution_clock::now();
        for (size_t i = 0; i < deque.GetSize(); ++i) {
            sum += deque.Get(i);
        }
        end = high_resolution_clock::now();
        auto get_time = duration_cast<milliseconds>(end - start).count();
        
        cout << "Size: " << size << " elements\n";
        cout << "  PushBack: " << push_back_time << " ms\n";
        cout << "  PushFront: " << push_front_time << " ms\n";
        cout << "  PopBack: " << pop_back_time << " ms\n";
        cout << "  PopFront: " << pop_front_time << " ms\n";
        cout << "  Get: " << get_time << " ms\n";
        cout << "  Sum check: " << sum << " \n\n";
    }
}

template void Benchmark<ArrayDeque<int>>(const string&, const vector<size_t>&);
template void Benchmark<NewDeque<int>>(const string&, const vector<size_t>&);
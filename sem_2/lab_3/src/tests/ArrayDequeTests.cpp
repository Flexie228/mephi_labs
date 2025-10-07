#include <cassert>
#include <iostream>
#include "./include/Deque/ArrayDeque.hpp"

void testArrayDeque() {
    // Тест конструкторов
    {
        ArrayDeque<int> d1;
        assert(d1.IsEmpty());
        
        int items[] = {1, 2, 3};
        ArrayDeque<int> d2(items, 3);
        assert(d2.GetSize() == 3);
        assert(d2.GetFront() == 1);
        assert(d2.GetBack() == 3);
        
        ArrayDeque<int> d3(d2);
        assert(d3.GetSize() == 3);
        assert(d3.GetFront() == 1);
        assert(d3.GetBack() == 3);
        
        ArrayDeque<int> d4(std::move(d2));
        assert(d4.GetSize() == 3);
        assert(d4.GetFront() == 1);
        assert(d4.GetBack() == 3);
        assert(d2.IsEmpty());
    }

    // Тест операций
    {
        ArrayDeque<int> d;
        
        // Добавление 100 элементов в начало
        for (int i = 0; i < 100; ++i) {
            d.PushFront(i);
            assert(d.GetFront() == i);
        }
        
        // Проверка порядка
        for (int i = 99; i >= 0; --i) {
            assert(d.PopFront() == i);
        }
        assert(d.IsEmpty());
        
        // Добавление 100 элементов в конец
        for (int i = 0; i < 100; ++i) {
            d.PushBack(i);
            assert(d.GetBack() == i);
        }
        
        // Проверка порядка
        for (int i = 0; i < 100; ++i) {
            assert(d.PopFront() == i);
        }
        assert(d.IsEmpty());
    }

    // Тест операторов
    {
        ArrayDeque<int> d1;
        for (int i = 0; i < 10; ++i) {
            d1.PushBack(i);
        }
        
        ArrayDeque<int> d2 = d1;
        assert(d2.GetSize() == 10);
        assert(d2.GetFront() == 0);
        assert(d2.GetBack() == 9);
        
        ArrayDeque<int> d3 = std::move(d1);
        assert(d3.GetSize() == 10);
        assert(d3.GetFront() == 0);
        assert(d3.GetBack() == 9);
        assert(d1.IsEmpty());
    }

    // Тест граничных случаев
    {
        ArrayDeque<int> d;
        bool error = false;
        
        try { d.PopFront(); } 
        catch (...) { error = true; }
        assert(error);
        
        error = false;
        try { d.PopBack(); } 
        catch (...) { error = true; }
        assert(error);
        
        error = false;
        try { d.GetFront(); } 
        catch (...) { error = true; }
        assert(error);
        
        error = false;
        try { d.GetBack(); } 
        catch (...) { error = true; }
        assert(error);
        
        // Тест доступа по индексу
        d.PushBack(1);
        d.PushBack(2);
        assert(d.Get(0) == 1);
        assert(d.Get(1) == 2);
        
        error = false;
        try { d.Get(2); }
        catch (...) { error = true; }
        assert(error);
    }

    std::cout << "All ArrayDeque tests passed!\n";
}
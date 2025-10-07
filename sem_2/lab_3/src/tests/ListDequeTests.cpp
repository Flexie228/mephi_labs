#include <cassert>
#include <iostream>
#include "./include/Deque/ListDeque.hpp"

void testListDeque() {
    // Тест конструкторов
    {
        ListDeque<int> d1;
        assert(d1.IsEmpty());
        
        int items[] = {1, 2, 3};
        ListDeque<int> d2(items, 3);
        assert(d2.GetSize() == 3);
        assert(d2.GetFront() == 1);
        assert(d2.GetBack() == 3);
        
        ListDeque<int> d3(d2);
        assert(d3.GetSize() == 3);
        assert(d3.GetFront() == 1);
        assert(d3.GetBack() == 3);
        
        ListDeque<int> d4(std::move(d2));
        assert(d4.GetSize() == 3);
        assert(d4.GetFront() == 1);
        assert(d4.GetBack() == 3);
        assert(d2.IsEmpty());
    }

    // Тест операций
    {
        ListDeque<int> d;
        
        // Добавление 1000 элементов в начало
        for (int i = 0; i < 1000; ++i) {
            d.PushFront(i);
            assert(d.GetFront() == i);
        }
        
        // Проверка порядка
        for (int i = 999; i >= 0; --i) {
            assert(d.PopFront() == i);
        }
        assert(d.IsEmpty());
        
        // Добавление 1000 элементов в конец
        for (int i = 0; i < 1000; ++i) {
            d.PushBack(i);
            assert(d.GetBack() == i);
        }
        
        // Проверка порядка
        for (int i = 0; i < 1000; ++i) {
            assert(d.PopFront() == i);
        }
        assert(d.IsEmpty());
    }

    // Тест операторов
    {
        ListDeque<int> d1;
        for (int i = 0; i < 100; ++i) {
            d1.PushBack(i);
        }
        
        ListDeque<int> d2 = d1;
        assert(d2.GetSize() == 100);
        assert(d2.GetFront() == 0);
        assert(d2.GetBack() == 99);
        
        ListDeque<int> d3 = std::move(d1);
        assert(d3.GetSize() == 100);
        assert(d3.GetFront() == 0);
        assert(d3.GetBack() == 99);
        assert(d1.IsEmpty());
    }

    // Тест граничных случаев
    {
        ListDeque<int> d;
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

    std::cout << "All ListDeque tests passed!\n";
}
#include <cassert>
#include <iostream>
#include "DynamicArray.hpp"

void TestDynamicArray() {
    // Тест 1: Создание пустого массива
    {
        DynamicArray<int> arr;
        assert(arr.GetSize() == 0);
    }

    // Тест 2: Создание массива заданного размера
    {
        DynamicArray<int> arr(5);
        assert(arr.GetSize() == 5);
    }

    // Тест 3: Копирование из массива
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);

        assert(arr.GetSize() == 3);
        for (int i = 0; i <= 3; i++) {
            assert(arr.Get(i) == src[i]);
        }
    }

    // Тест 4: Копирующий конструктор
    {
        DynamicArray<int> arr1(3);
        arr1.Set(0, 10);
        DynamicArray<int> arr2(arr1);
        assert(arr2.GetSize() == 3);
        assert(arr2.Get(0) == 10);
    }

    // Тест 5: Оператор присваивания
    {
        DynamicArray<int> arr1(2);
        DynamicArray<int> arr2;
        arr2 = arr1;
        assert(arr2.GetSize() == 2);
    }

    // Тест 6: Изменение размера (увеличение)
    {
        DynamicArray<int> arr(2);
        arr.Resize(10);
        assert(arr.GetSize() == 10);
    }

    // Тест 7: Изменение размера (уменьшение)
    {
        DynamicArray<int> arr(10);
        arr.Resize(2);
        assert(arr.GetSize() == 2);
    }

    // Тест 8: Исключения при неверных индексах
    {
        DynamicArray<int> arr(3);
        try {
            arr.Get(5);
            assert(false);
        } catch (...) {}
        
        try {
            arr.Set(5, 10);
            assert(false);
        } catch (...) {}
    }

    std::cout << "8/8 DynamicArray tests passed!\n";
}
#include <iostream>
#include <cassert>
#include "LinkedList.hpp"

void TestLinkedList() {
    // Тест 1: Создание пустого списка
    {
        LinkedList<int> list;
        assert(list.GetLength() == 0);
        try {
            list.GetFirst();
            assert(false);
        } catch (...) {}
    }

    // Тест 2: Создание из массива
    {
        int arr[] = {1, 2, 3};
        LinkedList<int> list(arr, 3);
        assert(list.GetLength() == 3);
        assert(list.GetFirst() == 1);
        assert(list.GetLast() == 3);
        assert(list.Get(1) == 2);

        for (size_t i = 0; i < 3; i++) {
            assert(list.Get(i) == arr[i]);
        }
    }

    // Тест 3: Добавление элементов
    {
        LinkedList<std::string> list;
        list.Append("world");
        list.Prepend("hello");
        assert(list.GetLength() == 2);
        assert(list.GetFirst() == "hello");
        assert(list.GetLast() == "world");
    }

    // Тест 4: Вставка по индексу
    {
        LinkedList<char> list;
        list.InsertAt('a', 0);
        list.InsertAt('c', 1);
        list.InsertAt('b', 1);
        assert(list.GetLength() == 3);
        assert(list.Get(0) == 'a');
        assert(list.Get(1) == 'b');
        assert(list.Get(2) == 'c');
    }

    // Тест 5: Получение подсписка
    {
        int arr[] = {0, 1, 2, 3, 4};
        LinkedList<int> list(arr, 5);
        LinkedList<int>* subList = list.GetSubList(1, 3);
        assert(subList->GetLength() == 3);

        for(size_t i = 1; i < 4; i++) {
            assert(list.Get(i) == subList->Get(i - 1));
        }
        delete subList;
    }

    // Тест 6: Копирующий конструктор
    {
        LinkedList<int> list1;
        list1.Append(10);
        list1.Append(20);
        
        LinkedList<int> list2(list1);
        assert(list2.GetLength() == 2);
        assert(list2.GetFirst() == 10);
        assert(list2.GetLast() == 20);
    }

    // Тест 7: Конкатенация списков
    {
        LinkedList<int> list1;
        list1.Append(1);
        list1.Append(2);
        
        LinkedList<int> list2;
        list2.Append(3);
        list2.Append(4);
        
        LinkedList<int>* result = list1.Concat(&list2);
        assert(result->GetLength() == 4);
        assert(result->Get(0) == 1);
        assert(result->Get(3) == 4);
        delete result;
    }

    // Тест 8: Обработка исключений
    {
        LinkedList<int> list;
        try {
            list.Get(0);
            assert(false);
        } catch (...) {}
        
        try {
            list.InsertAt(1, 1);
            assert(false);
        } catch (...) {}
        
        try {
            LinkedList<int>* empty = new LinkedList<int>();
            empty->GetSubList(0, 1);
            assert(false);
        } catch (...) {}
    }

    std::cout << "8/8 LinkedList tests passed!\n";
}
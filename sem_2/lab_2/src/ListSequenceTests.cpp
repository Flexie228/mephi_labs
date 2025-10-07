#include "ListSequence.hpp"
#include <cassert>
#include <stdexcept>

void testMutableListSequence() {
    // Тест 1.1: Конструкторы
    {
        MutableListSequence<int> seq1;
        assert(seq1.GetLength() == 0);
        
        int items[] = {1, 2, 3};
        MutableListSequence<int> seq2(items, 3);
        assert(seq2.GetLength() == 3);
        assert(seq2.Get(0) == 1);
        assert(seq2.Get(1) == 2);
        assert(seq2.Get(2) == 3);
        
        MutableListSequence<int> seq3(seq2);
        assert(seq3 == &seq2);
    }

    // Тест 1.2: Добавление элементов
    {
        MutableListSequence<int> seq;
        seq.Append(1)->Append(2)->Append(3);
        assert(seq.GetLength() == 3);
        assert(seq.Get(0) == 1);
        assert(seq.Get(1) == 2);
        assert(seq.Get(2) == 3);
        
        seq.Prepend(0);
        assert(seq.GetLength() == 4);
        assert(seq.Get(0) == 0);
        assert(seq.Get(1) == 1);
        
        seq.InsertAt(10, 2);
        assert(seq.GetLength() == 5);
        assert(seq.Get(2) == 10);
    }

    // Тест 1.3: Удаление элементов
    {
        int items[] = {1, 2, 3, 4, 5};
        MutableListSequence<int> seq(items, 5);
        seq.DeleteByIndex(2);
        assert(seq.GetLength() == 4);
        assert(seq.Get(2) == 4);
    }

    // Тест 1.4: Подпоследовательности
    {
        int items[] = {1, 2, 3, 4, 5};
        MutableListSequence<int> seq(items, 5);
        auto subSeq = seq.GetSubsequence(1, 3);
        
        int expected[] = {2, 3, 4};
        MutableListSequence<int> expectedSeq(expected, 3);
        assert(*subSeq == &expectedSeq);
        delete subSeq;
    }

    // Тест 1.5: Конкатенация
    {
        int items1[] = {1, 2, 3};
        int items2[] = {4, 5, 6};
        MutableListSequence<int> seq1(items1, 3);
        MutableListSequence<int> seq2(items2, 3);
        auto concated = seq1.Concat(&seq2);
        
        int expected[] = {1, 2, 3, 4, 5, 6};
        MutableListSequence<int> expectedSeq(expected, 6);
        assert(*concated == &expectedSeq);
        delete concated;
    }

    // Тест  1.6: Оператор сравнения
    {
        int items1[] = {1, 2, 3};
        int items2[] = {1, 2, 3};
        int items3[] = {1, 2, 4};
        
        MutableListSequence<int> seq1(items1, 3);
        MutableListSequence<int> seq2(items2, 3);
        MutableListSequence<int> seq3(items3, 3);
        MutableListSequence<int> seq4(items1, 2);
        
        assert(seq1 == &seq1);  // Сам с собой
        assert(seq1 == &seq2);  // Равные
        assert(!(seq1 == &seq3)); // Разное содержимое
        assert(!(seq1 == &seq4)); // Разная длина
    }

    // Тест 1.7: Оператор присваивания
    {
        int items[] = {1, 2, 3};
        MutableListSequence<int> seq1(items, 3);
        MutableListSequence<int> seq2;
        
        seq2 = seq1;
        assert(seq2 == &seq1);
        
        MutableListSequence<int> seq3;
        seq3 = std::move(seq1);
        assert(seq3 == &seq2);
        assert(seq1.GetLength() == 0);
    }

    // Тест 1.8: Исключения
    {
        MutableListSequence<int> seq;
        bool exceptionThrown = false;
        
        try { seq.GetFirst(); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.GetLast(); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.Get(0); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.InsertAt(1, 1); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.DeleteByIndex(0); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
    }
}
#include "ArraySequence.hpp"
#include <cassert>
#include <stdexcept>

void testMutableArraySequence() {
    // Тест 1.1: Конструкторы
    {
        MutableArraySequence<int> seq1;
        assert(seq1.GetLength() == 0);
        
        int items[] = {1, 2, 3};
        MutableArraySequence<int> seq2(items, 3);
        assert(seq2.GetLength() == 3);
        assert(seq2.Get(0) == 1);
        assert(seq2.Get(1) == 2);
        assert(seq2.Get(2) == 3);
        
        MutableArraySequence<int> seq3(5);
        assert(seq3.GetLength() == 0);
        
        MutableArraySequence<int> seq4(seq2);
        assert(seq4.GetLength() == 3);
        assert(seq4.Get(0) == 1);
        assert(seq4.Get(1) == 2);
        assert(seq4.Get(2) == 3);
    }

    // Тест 1.2: Добавление элементов
    {
        MutableArraySequence<int> seq;
        seq.Append(1);
        seq.Append(2);
        seq.Append(3);

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

    // Тест 1.3:  Удаление элементов
    {
        int items[] = {1, 2, 3, 4, 5};
        MutableArraySequence<int> seq(items, 5);
        seq.DeleteByIndex(2);
        assert(seq.GetLength() == 4);
        assert(seq.Get(2) == 4);
    }

    // Тест 1.4: Подпоследовательности
    {
        int items[] = {1, 2, 3, 4, 5};
        MutableArraySequence<int> seq(items, 5);
        auto subSeq = seq.GetSubsequence(1, 3);
        assert(subSeq->GetLength() == 3);
        assert(subSeq->Get(0) == 2);
        assert(subSeq->Get(1) == 3);
        assert(subSeq->Get(2) == 4);
        delete subSeq;
    }

    // Тест 1.5: Конкатенация
    {
        int items1[] = {1, 2, 3};
        int items2[] = {4, 5, 6};
        MutableArraySequence<int> seq1(items1, 3);
        MutableArraySequence<int> seq2(items2, 3);
        auto concated = seq1.Concat(&seq2);
        
        assert(concated->GetLength() == 6);
        for (int i = 0; i < 6; ++i) {
            assert(concated->Get(i) == i + 1);
        }
        delete concated;
    }

    // Тест 1.6: Оператор сравнения
    {
        int items1[] = {1, 2, 3};
        int items2[] = {1, 2, 3};
        int items3[] = {1, 2, 4};
        
        MutableArraySequence<int> seq1(items1, 3);
        MutableArraySequence<int> seq2(items2, 3);
        MutableArraySequence<int> seq3(items3, 3);
        MutableArraySequence<int> seq4(items1, 2);
        
        assert(seq1 == &seq1);  // Сам с собой
        assert(seq1 == &seq2);  // Равные
        assert(!(seq1 == &seq3)); // Разное содержимое
        assert(!(seq1 == &seq4)); // Разная длина
    }

    // Тест 1.7: Исключения
    {
        MutableArraySequence<int> seq;
        bool exceptionThrown = false;
        
        try { seq.GetFirst(); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.GetLast(); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.Get(3); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.InsertAt(1, 1); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.DeleteByIndex(1); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
        
        exceptionThrown = false;
        try { seq.GetSubsequence(1, 0); } 
        catch (...) { exceptionThrown = true; }
        assert(exceptionThrown);
    }
}
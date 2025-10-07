#ifndef ARRAY_SEQUENCE
#define ARRAY_SEQUENCE

#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include <algorithm>

template <typename T>
class ArraySequence : public Sequence<T> {
    private:
        DynamicArray<T> array;
        size_t elemsCount;
        size_t capacity;

    public:
        //Конструкторы и Деструктор

        ArraySequence() : array(), elemsCount(0), capacity(0) {}
        ArraySequence(const T* items, size_t count) : array(items, count), elemsCount(count), capacity(count) {}
        ArraySequence(size_t size) : array(size), elemsCount(0), capacity(size) {}
        ArraySequence(const ArraySequence<T>& other) : array(other.array), elemsCount(other.elemsCount), capacity(other.capacity) {}
        ~ArraySequence() = default;

        // Операторы
        bool operator == (const Sequence<T>& other) const override
        {
            if (elemsCount != other.GetSize()) return false;
            for (size_t i = 0; i < elemsCount; i++) {
                if (Get(i) != other.Get(i)) return false;
            }
            return true;
        }

        // Методы

        size_t GetSize() const override { return elemsCount; }

        T GetFirst() const override
        {
            if (elemsCount == 0) throwError(SEQUENCE_EMPTY);
            return array.Get(0);
        }

        T Get(size_t index) const override
        {
            if (index >= elemsCount) throwError(INDEX_OUT_OF_RANGE);
            return array.Get(index);
        }

        T GetLast() const override
        {
            if (elemsCount == 0) throwError(SEQUENCE_EMPTY);
            return array.Get(elemsCount - 1);
        }

        Sequence<T>* Append(const T& value) override
        {
            if (elemsCount < capacity) {
                array.Set(value, elemsCount);
            } else {
                capacity = (capacity == 0) ? 1 : capacity * 2;
                array.Resize(capacity);
            }
            ++elemsCount;
            return this;
        }

        virtual Sequence<T>* Prepend(const T& value) override
        {
            if (elemsCount < capacity) {
                for (size_t i = elemsCount; i > 0; i--) {
                    array[i] = array[i - 1];
                }
                array[0] = value;
            } else {
                capacity = (capacity == 0) ? 1 : capacity * 2;
                DynamicArray<T> newArray(capacity);
                newArray[0] = value;
                for (size_t i = 0; i < elemsCount; i++) {
                    newArray[i + 1] = array[i];
                }
                array = std::move(newArray);
            }
            ++elemsCount;
            return this;
        }

        virtual Sequence<T>* InsertAt(const T& value, size_t index) override
        {
            if (index > elemsCount) throwError(INDEX_OUT_OF_RANGE);

            if (index < elemsCount) {
                for (size_t i = elemsCount; i > index; i--) {
                    array[i] = array[i - 1];
                }
                array[index] = value;
                ++elemsCount;
            } else {
                Append(value);
            }
            return this;
        }

        virtual Sequence<T>* RemoveAt(size_t index) override
        {
            if (index >= elemsCount) throwError(INDEX_OUT_OF_RANGE);
            while (index < elemsCount - 1) {
                array[index] = array[index + 1];
                ++index;
            }
            --elemsCount;
            if (capacity > 1 && elemsCount <= capacity / 4) {
                array.Resize(capacity / 2);
                capacity /= 2;
            }
            return this;
        }

        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override
        {
            if (endIndex < startIndex || endIndex >= elemsCount) throwError(INVALID_ARGUMENT);

            ArraySequence<T>* subSeq = new ArraySequence<T>(endIndex - startIndex + 1);
            for (size_t i = startIndex; i <= endIndex; i++) {
                subSeq->Append(Get(i));
            }

            return subSeq;
        }

        Sequence<T>* Concat(const Sequence<T>& other) const override
        {
            if (other.GetSize() == 0) throwError(INVALID_ARGUMENT);
    
            const size_t totalSize = elemsCount + other.GetSize();
            T* newArray = new T[totalSize];
    
            for (size_t i = 0; i < elemsCount; i++) {
                newArray[i] = array[i];
            }
    
            for (size_t i = 0; i < other.GetSize(); i++) {
                newArray[elemsCount + i] = other.Get(i);
            }
    
            Sequence<T>* result = new ArraySequence<T>(newArray, totalSize);
            delete[] newArray;
            return result;
        }

};
#endif
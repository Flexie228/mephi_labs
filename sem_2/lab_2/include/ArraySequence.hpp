#ifndef ARRAY_SEQUENCE
#define ARRAY_SEQUENCE

#include <algorithm>
#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <typename T> class MutableArraySequence;
template <typename T> class ImmutableArraySequence;

template <typename T>
class ArraySequence : public Sequence<T> {
    protected:
        DynamicArray<T> array;
        size_t elemCount;
        size_t capacity;

    public:

        ArraySequence() : array(), elemCount(0), capacity(0) {}
        ArraySequence(const T* items, size_t count) : array(items, count), elemCount(count), capacity(count) {}
        ArraySequence(size_t size) : array(size), elemCount(0), capacity(size) {}
        ArraySequence(const ArraySequence<T>& other) : array(other.array), elemCount(other.elemCount), capacity(other.capacity) {}
        ~ArraySequence() = default;


        // МЕТОДЫ ДОСТУПА

        T GetFirst() const override {
            if (array.GetSize() == 0 || elemCount == 0) THROW_ERROR(SEQUENCE_EMPTY);
            return array.Get(0);
        }

        T GetLast() const override {
            if (array.GetSize() == 0 || elemCount == 0) THROW_ERROR(SEQUENCE_EMPTY);
            return array.Get(elemCount - 1);
        }

        T Get(size_t index) const override {
            if (index >= elemCount) THROW_ERROR(INDEX_OUT_OF_RANGE);
            return array.Get(index);
        }

        size_t GetLength() const override {
            return elemCount;
        }


        // МЕТОДЫ РАБОТЫ

        virtual Sequence<T>* Append(const T& item) = 0;
        virtual Sequence<T>* Prepend(const T& item) = 0;
        virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;
        virtual Sequence<T>* DeleteByIndex(size_t index) = 0;

        // ОПЕРАТОРЫ
        bool operator == (const Sequence<T>* other) const override {
            if (this->GetLength() != other->GetLength()) return false;
            size_t elemCountCommon;
            for (size_t i = 0; i < elemCountCommon; i++) {
                if (this->Get(i) != other->Get(i)) return false;
            }
            return true;
        }


        // МЕТОДЫ СОЗДАНИЯ НОВЫХ ПОСЛЕДОВАТЕЛЬНОСТЕЙ

        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
            if (endIndex < startIndex || endIndex >= elemCount) THROW_ERROR(INVALID_ARGUMENT);

            ArraySequence<T>* subSeq = new MutableArraySequence<T>(endIndex - startIndex + 1);
            for (size_t i = startIndex; i <= endIndex; i++) {
                subSeq->Append(this->Get(i));
            }

            return subSeq;
        }
    
        Sequence<T>* Concat(Sequence<T>* other) const override {
            if (!other) THROW_ERROR(INVALID_ARGUMENT);
    
            const size_t totalSize = elemCount + other->GetLength();
            T* newArray = new T[totalSize];
    
            for (size_t i = 0; i < elemCount; ++i) {
                newArray[i] = array[i];
            }
    
            for (size_t i = 0; i < other->GetLength(); ++i) {
                newArray[elemCount + i] = other->Get(i);
            }
    
            Sequence<T>* result = new MutableArraySequence<T>(newArray, totalSize);
            delete[] newArray;
            return result;
        }
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
    public:

        using ArraySequence<T>::ArraySequence;

        Sequence<T>* Append(const T& item) override {
            if (this->elemCount < this->capacity) {
                this->array.Set(this->elemCount, item);
                ++(this->elemCount);
            } else {
                this->array.Resize(this->capacity*2);
                ++(this->elemCount);
                (this->capacity) *= 2;
            }
            return this;
        }

        Sequence<T>* Prepend(const T& item) override {
            if (this->elemCount < this->capacity) {
                for (size_t i = this->elemCount; i > 0; i--) {
                    this->array[i] = this->array[i - 1];
                }
                this->array[0] = item;
                ++(this->elemCount);
            } else {
                size_t newCapacity = (this->capacity) == 0 ? 1 : (this->capacity) * 2;
                DynamicArray<T> newArray(newCapacity);
                newArray[0] = item;
                for (size_t i = 0; i < (this->elemCount); i++) {
                    newArray[i + 1] = this->array[i];
                }
                this->array = std::move(newArray);
                (this->capacity) = newCapacity;
                ++(this->elemCount);
            }
            return this;
        }

        Sequence<T>* InsertAt(const T& item, size_t index) override {
            if (index > this->elemCount) THROW_ERROR(INDEX_OUT_OF_RANGE);

            if (index < this->capacity) {
                for (size_t i = this->elemCount; i > index; i--) {
                    this->array[i] = this->array[i - 1];
                }
                this->array[index] = item;
                ++(this->elemCount);
            } else {
                size_t newCapacity = this->capacity == 0 ? 1 : this->capacity * 2;
                DynamicArray<T> newArray(newCapacity);
        
                for (size_t i = 0; i < index; i++) {
                    newArray[i] = this->array[i];
                }
                newArray[index] = item;
                for (size_t i = index; i < this->elemCount; i++) {
                    newArray[i + 1] = this->array[i];
                }
        
                this->array = std::move(newArray);
                this->capacity = newCapacity;
                ++(this->elemCount);
            }
            return this;
        }

        Sequence<T>* DeleteByIndex(size_t index) override {
            if (index >= this->elemCount) THROW_ERROR(INDEX_OUT_OF_RANGE);
            while (index < this->elemCount - 1) {
                this->array[index] = this->array[index + 1];
                index++;
            }
            --(this->elemCount);
            if (this->capacity > 1 && this->elemCount <= this->capacity / 4) {
                this->array.Resize(this->capacity / 2);
                this->capacity /= 2;
            }
            return this;
        }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
    public:
    
        using ArraySequence<T>::ArraySequence;

        Sequence<T>* Append(const T& item) override {
            auto* newSeq = new ImmutableArraySequence<T>(*this);
            if (newSeq->elemCount < newSeq->capacity) {
                newSeq->array[newSeq->elemCount++] = item;
            } else {
                newSeq->array.Resize(newSeq->capacity == 0 ? 1 : newSeq->capacity * 2);
                newSeq->capacity = newSeq->array.GetSize();
                newSeq->array[newSeq->elemCount++] = item;
            }
            return newSeq;
        }

        Sequence<T>* Prepend(const T& item) override {
            auto* newSeq = new ImmutableArraySequence<T>(this->elemCount + 1);
            newSeq->array[0] = item;
            for (size_t i = 0; i < this->elemCount; i++) {
                newSeq->array[i + 1] = this->array[i];
            }
            newSeq->elemCount = this->elemCount + 1;
            return newSeq;
        }

        Sequence<T>* InsertAt(const T& item, size_t index) override {
            if (index > this->elemCount) THROW_ERROR(INDEX_OUT_OF_RANGE);

            auto* newSeq = new ImmutableArraySequence<T>(this->elemCount + 1);
            for (size_t i = 0; i < index; i++) {
                newSeq->array[i] = this->array[i];
            }
            newSeq->array[index] = item;
            for (size_t i = index; i < this->elemCount; i++) {
                newSeq->array[i + 1] = this->array[i];
            }
            newSeq->elemCount = this->elemCount + 1;
            return newSeq;
        }

        Sequence<T>* DeleteByIndex(size_t index) override {
            if (index >= this->elemCount) THROW_ERROR(INDEX_OUT_OF_RANGE);

            auto* newSeq = new ImmutableArraySequence<T>(this->elemCount - 1);
            for (size_t i = 0; i < index; i++) {
                newSeq->array[i] = this->array[i];
            }
            for (size_t i = index + 1; i < this->elemCount; i++) {
                newSeq->array[i - 1] = this->array[i];
            }
            newSeq->elemCount = this->elemCount - 1;
            return newSeq;
        }
};    
#endif
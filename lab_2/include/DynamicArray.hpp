#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <stdexcept>
#include <algorithm>
#include "Errors.hpp"

template <typename T>
class DynamicArray {
    private:
        T* data;
        size_t size;

    public:
        DynamicArray() : data(nullptr), size(0) {}

        DynamicArray(const T* other, size_t elementsCount) : data(nullptr), size(elementsCount) {
            if (size > 0 && other == nullptr) THROW_ERROR(INVALID_ARGUMENT);
            if (size == 0) return;

            data = new T[size];
            std::copy(other, other + elementsCount, data);
        }

        explicit DynamicArray(size_t size) : data(nullptr), size(size) {
            if (size > 0) data = new T[size]();
        }

        DynamicArray(const DynamicArray<T>& other) : data(nullptr), size(other.size) {
            if (size > 0) {
                data = new T[size];
                std::copy(other.data, other.data + size, data);
            }
        }

        DynamicArray(DynamicArray<T>&& other) : data(other.data), size(other.size) {
            other.data = nullptr;
            other.size = 0;
        }

        DynamicArray& operator=(DynamicArray<T>&& other) {
            if (this != &other) {
                delete[] data;
                data = other.data;
                size = other.size;
                other.data = nullptr;
                other.size = 0;
            }
            return *this;
        }

        DynamicArray& operator = (const DynamicArray& other) {
            if (this != &other) {
                delete[] data;
                size = other.size;
                data = size > 0 ? new T[size] : nullptr;
                if (data) std::copy(other.data, other.data + size, data);
            }
            return *this;
        }

        ~DynamicArray() {
            delete[] data;
        }

        T Get(size_t index) const {
            return data[index];
        }

        size_t GetSize() const {
            return size;
        }

        void Set(size_t index,const T& value) {
            if (index >= size) THROW_ERROR(INDEX_OUT_OF_RANGE);
            data[index] = value;
        }

        const T& operator[](size_t index) const {
            return data[index];
        }
        
        T& operator[](size_t index) {
            return data[index];
        }

        void Resize(size_t newSize) {
            if (newSize == size) return;
            if (newSize > 0) {
                T* newData = new T[newSize];
            
                std::copy(data, data + std::min(size, newSize), newData);
                delete[] data;

                if (newSize > size) {
                    for (size_t i = size; i < newSize; ++i) {
                        newData[i] = T();
                    }
                }

                data = newData;
                size = newSize;
            } else {
                delete[] data;
                data = nullptr;
                size = 0;
            }
        }
};
#endif
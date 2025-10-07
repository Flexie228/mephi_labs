#ifndef SEQUENCE
#define SEQUENCE

template <typename T>
class Sequence {
    public:
        virtual T GetFirst() const = 0;
        virtual T GetLast() const = 0;
        virtual T Get(size_t index) const = 0;
        virtual size_t GetSize() const = 0;

        virtual Sequence<T>* Append(const T& value) = 0;
        virtual Sequence<T>* Prepend(const T& value) = 0;
        virtual Sequence<T>* InsertAt(const T& value, size_t index) = 0;
        virtual Sequence<T>* RemoveAt(size_t index) = 0;

        virtual bool operator == (const Sequence<T>& other) const = 0;

        virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
        virtual Sequence<T>* Concat(const Sequence<T>& other) const = 0;
    
        virtual ~Sequence() = default;
};
#endif
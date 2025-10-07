#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

template <typename T>
class Sequence {
    public:

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;
    virtual Sequence<T>* DeleteByIndex(size_t index) = 0;

    virtual bool operator == (const Sequence<T>* other) const = 0;

    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual Sequence<T>* Concat(Sequence<T>* other) const = 0;
    
    virtual ~Sequence() = default;

};
#endif
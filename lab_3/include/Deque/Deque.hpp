#ifndef DEQUE
#define DEQUE

template <typename T>
class Deque {
    public:
        virtual ~Deque() = default;
        
        virtual void PushFront(const T& value) = 0;
        virtual void PushBack(const T& value) = 0;

        virtual T PopFront() = 0;
        virtual T PopBack() = 0;

        virtual T GetFront() const = 0;
        virtual T GetBack() const = 0;
        
        virtual size_t GetSize() const = 0;
        virtual bool IsEmpty() const = 0;

        virtual T Get(size_t index) const = 0;
        virtual void Print() const = 0;
};
#endif
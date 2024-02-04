#ifndef CIRCULARBUFFER_CIRCULARBUFFER_H
#define CIRCULARBUFFER_CIRCULARBUFFER_H

#include <cstdint>

template<class T>
class CircularBuffer {
    T* buffer;
    uint64_t _capacity{};
    uint64_t start{};
    uint64_t finish{};
    uint64_t _size{};
public:
    CircularBuffer();
    ~CircularBuffer();
    CircularBuffer(const CircularBuffer& cb);

    explicit CircularBuffer(int capacity);

    CircularBuffer(int capacity, const T& elem);

    T& operator[](int i);
    const T& operator[](int i) const;

    T& at(int i);
    const T& at(int i) const;

    T& front();
    T& back();
    const T& front() const;
    const T& back() const;

    T* linearize();

    bool is_linearized() const;

    void rotate(int new_begin);

    int size() const;
    bool empty() const;

    bool full() const;

    int reserve() const;

    int capacity() const;

    void set_capacity(int new_capacity);

    void resize(int new_size, const T& item = T());

    CircularBuffer& operator=(const CircularBuffer& cb);

    void swap(CircularBuffer& cb);


    void push_back(const T& item = T());

    void push_front(const T& item = T());

    void pop_back();

    void pop_front();

    void insert(int pos, const T& item = T());

    void erase(int first, int last);

    void clear();
};

template<class T>
bool operator==(const CircularBuffer<T>& a, const CircularBuffer<T>& b);

template<class T>
bool operator!=(const CircularBuffer<T>& a, const CircularBuffer<T>& b);

#endif

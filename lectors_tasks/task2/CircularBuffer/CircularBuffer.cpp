#include <cstdlib>
#include <new>
#include <stdexcept>
#include <cassert>
#include "CircularBuffer.h"

/**
 * CircularBuffer - create an empty circular buffer with zero capacity.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 */
template<class T>
CircularBuffer<T>::CircularBuffer() {
    buffer = nullptr;
    _capacity = 0;
    start = 0;
    finish = 0;
    _size = 0;
}

/**
 * ~CircularBuffer - destroy the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 */
template<class T>
CircularBuffer<T>::~CircularBuffer() {
    free(buffer);
    buffer = nullptr;
    _capacity = 0;
    _size = 0;
    start = 0;
    finish = 0;
}

/**
 * CircularBuffer - create a copy of the specified circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param cb - the circular buffer to be copied.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer& cb) {
    buffer = (T*)malloc(cb._capacity * sizeof(T));
    if (not buffer)
        throw std::bad_alloc();
    if (cb._size) {
        int i = cb.start;
        do {
            buffer[i] = cb.buffer[i];
            i = (i + 1) % cb._capacity;
        } while(i != cb.finish);
    }
    _capacity = cb._capacity;
    start = cb.start;
    finish = cb.finish;
    _size = cb._size;
}

/**
 * CircularBuffer - create an empty circular buffer with the specified capacity.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param capacity - the maximum number of elements which can be stored in the circular buffer.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
CircularBuffer<T>::CircularBuffer(int capacity) {
    buffer = (T*)malloc(capacity * sizeof(T));
    if (not buffer)
        throw std::bad_alloc();
    _capacity = capacity;
    start = 0;
    finish = 0;
    _size = 0;
}

/**
 * CircularBuffer - create a full circular buffer with the specified capacity and filled with elem.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param capacity - the maximum number of elements which can be stored in the circular buffer.
 * @param elem - the element the created circular buffer will be filled with.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
CircularBuffer<T>::CircularBuffer(int capacity, const T& elem) {
    buffer = (T*)malloc(capacity * sizeof(T));
    if (not buffer)
        throw std::bad_alloc();
    for (int i = 0; i < capacity; ++i)
        buffer[i] = elem;
    _capacity = capacity;
    start = 0;
    finish = 0;
    _size = capacity;
}

/**
 * operator[] - get the element at the i position.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param i - the position of the element.
 *
 * @attention - i must be less than size() and non-negative.
 *
 * @return a reference to the element at the i position.
 */
template<class T>
T& CircularBuffer<T>::operator[](int i) {
    assert(0 <= i && i < this->_size);
    return this->buffer[(this->start + i) % this->_capacity];
}

/**
 * operator[] - get the element at the i position.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param i - the position of the element.
 *
 * @attention - i must be less than size() and non-negative.
 *
 * @return a const reference to the element at the i position.
 */
template<class T>
const T& CircularBuffer<T>::operator[](int i) const {
    assert(0 <= i && i < this->_size);
    return this->buffer[(this->start + i) % this->_capacity];
}

/**
 * at - get the element at the i position.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param i - the position of the element.
 *
 * @return a reference to the element at the i position.
 *
 * @throw out_of_range - when the index is invalid (when index >= size() or index is negative).
 */
template<class T>
T& CircularBuffer<T>::at(int i) {
    if (0 <= i && i < this->_size)
        return this->buffer[(this->start + i) % this->_capacity];
    throw std::out_of_range("Invalid index.");
}

/**
 * at - get the element at the i position.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param i - the position of the element.
 *
 * @return a const reference to the element at the i position.
 *
 * @throw out_of_range - when the index is invalid (when index >= size() or index is negative).
 */
template<class T>
const T& CircularBuffer<T>::at(int i) const {
    if (0 <= i && i < this->_size)
        return this->buffer[(this->start + i) % this->_capacity];
    throw std::out_of_range("Invalid index.");
}

/**
 * front - get the first element.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @attention - the circular buffer must not be empty.
 *
 * @return a reference to the first element of the circular buffer.
 */
template<class T>
T& CircularBuffer<T>::front() {
    assert(this->_size);
    return this->buffer[this->start];
}

/**
 * back - get the last element.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @attention - the circular buffer must not be empty.
 *
 * @return a reference to the last element of the circular buffer.
 */
template<class T>
T& CircularBuffer<T>::back() {
    assert(this->_size);
    return this->buffer[(this->finish - 1 + this->_capacity) % this->_capacity];
}

/**
 * front - get the first element.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @attention - the circular buffer must not be empty.
 *
 * @return a const reference to the first element of the circular buffer.
 */
template<class T>
const T& CircularBuffer<T>::front() const {
    assert(this->_size);
    return this->buffer[this->start];
}

/**
 * back - get the last element.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @attention - the circular buffer must not be empty.
 *
 * @return a const reference to the last element of the circular buffer.
 */
template<class T>
const T& CircularBuffer<T>::back() const {
    assert(this->_size);
    return this->buffer[(this->finish - 1 + this->_capacity) % this->_capacity];
}

/**
 * linearize - linearize the internal buffer into a continuous array.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return a pointer to the beginning of the array or 0 if empty.
 */
template<class T>
T* CircularBuffer<T>::linearize() {
    if (not this->_size)
        return nullptr;
    auto temp_buff = CircularBuffer(*this);
    int i = temp_buff.start;
    int j = 0;
    do {
        this->buffer[j++] = temp_buff.buffer[i];
        i = (i + 1) % temp_buff._capacity;
    } while(i != temp_buff.finish);
    this->start = 0;
    this->finish = 0;
    return &this->buffer[0];
    // return &this->buffer[0] == this; - check!!!
}

/**
 * is_linearized - is the circular buffer linearized?
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return true if the internal buffer is linearized into a continuous array;
 *         false otherwise.
 */
template<class T>
bool CircularBuffer<T>::is_linearized() const {
    return this->start == 0;
}

/**
 * rotate - rotate elements in the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @param new_begin - the new beginning.
 */
template<class T>
void CircularBuffer<T>::rotate(int new_begin) {
    assert(0 <= new_begin && new_begin < this->_size);
    CircularBuffer<T> temp(new_begin);
    int k = new_begin;
    for (int i = 0; i != new_begin; i = ++i) {
        temp.buffer[i] = this->buffer[i];
        this->buffer[i] = this->buffer[k];
        k++;
    }
    for (int i = new_begin; i != this->_size - new_begin; ++i)
        this->buffer[i] = this->buffer[i + new_begin];
    int j = 0;
    for (int i = this->_size - new_begin; i != this->_size; ++i)
        this->buffer[i] = temp.buffer[j++];
    int new_finish = (this->finish + new_begin) % this->_capacity;
    this->start = (this->start + new_begin) % this->_capacity;
    this->finish = new_finish;
}

/**
 * size - get the number of elements currently stored in the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return the number of elements stored in the circular buffer.
 */
template<class T>
int CircularBuffer<T>::size() const {
    return this->_size;
}

/**
 * empty - is the circular buffer empty?
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return true if there are no elements stored in the circular buffer;
 *         false otherwise.
 */
template<class T>
bool CircularBuffer<T>::empty() const {
    return this->_size;
}

/**
 * full - is the circular buffer full?
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return true if the number of elements stored in the circular buffer equals the capacity of the circular buffer;
 *         false otherwise.
 */
template<class T>
bool CircularBuffer<T>::full() const {
    return this->_size == this->_capacity;
}

/**
 * reserve - get the maximum number of elements which can be inserted into the circular buffer without overwriting
 *           any of already stored elements.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return capacity() - size().
 */
template<class T>
int CircularBuffer<T>::reserve() const {
    return this->_capacity - this->_size;
}

/**
 * capacity - get the capacity of the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 *
 * @return the maximum number of elements which can be stored in the circular buffer.
 */
template<class T>
int CircularBuffer<T>::capacity() const {
    return this->_capacity;
}

/**
 * set_capacity - change the capacity of the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param new_capacity - the new capacity.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
void CircularBuffer<T>::set_capacity(int new_capacity) {
    T* check = (T*)realloc(this->buffer, new_capacity);
    if (check)
        this->buffer = check;
    else
        throw std::bad_alloc();
    if (this->_size > this->_capacity)
        this->_size = new_capacity;
    this->_capacity = new_capacity;
}

/**
 * resize - change the size of the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param new_size - the new size.
 * @param item - the element the circular buffer will be filled with in order to gain the requested size.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
void CircularBuffer<T>::resize(int new_size, const T& item) {
    if (new_size > this->_capacity) {
        T* check = (T*)realloc(this->buffer, new_size);
        if (check)
            this->buffer = check;
        else
            throw std::bad_alloc();
        this->_capacity = new_size;
    }
    if (new_size > this->_size) {
        this->finish = (this->finish + (new_size - this->_size)) % this->_capacity;
        for (int i = (this->start + this->_size) % this->_capacity; i != this->finish; i = (i + 1) % this->_capacity)
            this->buffer[i] = item;
    }
    else
        this->finish = this->finish - (this->_size - new_size);
    this->_size = new_size;
}

/**
 * operator= - the assign operator. Make the circular buffer to become a copy of the specified circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param cb - the circular buffer to be copied.
 *
 * @return a copy of the specified circular buffer.
 *
 * @throw bad_alloc - an allocation error if memory is exhausted.
 */
template<class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer<T>& cb) {
    if (this != &cb) {
        free(this->buffer);
        this->buffer = (T*)malloc(cb._capacity * sizeof(T));
        if (not this->buffer)
            throw std::bad_alloc();
        if (cb._size) {
            int i = cb.start;
            do {
                this->buffer[i] = cb.buffer[i];
                i = (i + 1) % cb._capacity;
            } while(i != cb.finish);
        }
        this->_capacity = cb._capacity;
        this->start = cb.start;
        this->finish = cb.finish;
        this->_size = cb._size;
    }
    return *this;
}

/**
 * swap - swap the contents of two circular buffers.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param cb - the circular buffer whose content will be swapped.
 */
template<class T>
void CircularBuffer<T>::swap(CircularBuffer<T>& cb) {
    auto temp = *this;
    *this = cb;
    cb = temp;
}

/**
 * push_back - insert a new element at the end of the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param item - the element to be inserted.
 */
template<class T>
void CircularBuffer<T>::push_back(const T& item) {
    if (this->_capacity) {
        if (this->_size) {
            this->buffer[(this->finish + 1) % this->_capacity] = item;
            this->finish = (this->finish + 1) % this->_capacity;
        }
        else
            this->buffer[this->finish] = item;
        if (this->_capacity > this->_size)
            this->_size++;
        else
            this->start = this->finish;
    }
}

/**
 * push_front - insert a new element at the beginning of the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param item - the element to be inserted.
 */
template<class T>
void CircularBuffer<T>::push_front(const T& item) {
    if (this->_capacity) {
        if (this->_size) {
            this->buffer[(this->start - 1 + this->_capacity) % this->_capacity] = item;
            this->start = (this->start - 1 + this->_capacity) % this->_capacity;
        }
        else
            this->buffer[this->start] = item;
        if (this->_capacity > this->_size)
            this->_size++;
        else
            this->finish = this->start;
    }
}

/**
 * pop_back - remove the last element from the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 */
template<class T>
void CircularBuffer<T>::pop_back() {
    assert(this->_size);
    this->finish = (this->finish - 1 + this->_capacity) % this->_capacity;
    this->_size--;
}

/**
 * pop_front - remove the first element from the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 */
template<class T>
void CircularBuffer<T>::pop_front() {
    assert(this->_size);
    this->start = (this->start + 1) % this->_capacity;
    this->_size--;
}

/**
 * insert - insert an element at the specified position.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param pos - the position where the item will be inserted.
 * @param item - the element to be inserted.
 */
template<class T>
void CircularBuffer<T>::insert(int pos, const T& item) {
    assert(0 <= pos && pos < this->_size);
    if (this->_capacity) {
        this->finish = (this->finish + 1) % this->_capacity;
        for (int i = (this->start + pos + 1) % this->_capacity; i != this->finish; i = (i + 1) % this->_size)
            this->buffer[i] = this->buffer[(i - 1 + this->_capacity) % this->_capacity];
        this->buffer[this->start + pos] = item;
        if (this->_capacity != this->_size)
            this->_size++;
    }
}

/**
 * erase - Erase the range [first, last).
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param first - the beginning of the range to be removed.
 * @param last - the end of the range to be removed.
 */
template<class T>
void CircularBuffer<T>::erase(int first, int last) {
    assert(0 <= first && first < this->_size);
    assert(0 <= last && last < this->_size);
    assert(first <= last);
    int remove_count = last - first;
    if (first >= this->_size - last) {
        for (int i = (this->start + first) % this->_capacity; i != this->finish; i = (i + 1) % this->_capacity)
            this->buffer[i] = this->buffer[(i + remove_count) % this->_capacity];
        this->finish = (this->finish - remove_count + this->_capacity) % this->_capacity;
    }
    else {
        int temp_finish = (this->start + first) % this->_capacity;
        for (int i = this->start; i != temp_finish; i = (i + 1) % this->_capacity)
            this->buffer[(i + remove_count) % this->_capacity] = this->buffer[i];
        this->start = (this->start + remove_count) % this->_capacity;
    }
    this->_size -= remove_count;
}

/**
 * clear - remove all stored elements from the circular buffer.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 */
template<class T>
void CircularBuffer<T>::clear() {
    this->start = 0;
    this->finish = 0;
    this->_size = 0;
}

/**
 * operator== - compare two circular buffers element-by-element to determine if they are equal.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param a - the circular buffer to compare.
 * @param b - the circular buffer to compare.
 * @return true if the sizes and contents of the circular buffers are equal;
 *         false otherwise.
 */
template<class T>
bool operator ==(const CircularBuffer<T>& a, const CircularBuffer<T>& b) {
    if (a.size() == b.size()) {
        for (int i = 0; i < a.size(); ++i)
            if (a[i] != b[i])
                return false;
        return true;
    }
    return false;
}

/**
 * operator!= - compare two circular buffers element-by-element to determine if they are non-equal.
 *
 * @tparam T - the type of elements stored in the circular buffer.
 * @param a - the circular buffer to compare.
 * @param b - the circular buffer to compare.
 * @return true if the sizes and contents of the circular buffers are equal;
 *         false otherwise.
 */
template<class T>
bool operator!=(const CircularBuffer<T>& a, const CircularBuffer<T>& b) {
    return !(a == b);
}

template class CircularBuffer<int>;
template bool operator==(const CircularBuffer<int>& a, const CircularBuffer<int>& b);

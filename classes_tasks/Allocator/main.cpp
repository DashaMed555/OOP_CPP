#include <algorithm>
#include <iostream>
#include <list>

template <typename T, std::size_t N>
struct smart_allocator {

    using value_type = T;
    template <class U>
    struct rebind {
        typedef smart_allocator<U, N> other;
    };

    smart_allocator() {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << N << "]" << std::endl;
        auto p = std::malloc(N * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        sequence = reinterpret_cast<T *>(p);
        num_stored = 0;
    }

    ~smart_allocator() {
        std::cout << __PRETTY_FUNCTION__  << std::endl;
        std::free(sequence);
    }

    [[nodiscard]]
    T *allocate(std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        num_stored++;
        if (N < num_stored) {
            auto p = std::malloc(N * sizeof(T));
            if (!p)
                throw std::bad_alloc();
            return reinterpret_cast<T *>(p);
        }
        return reinterpret_cast<T *>(&sequence[num_stored - 1]);
    }

    void deallocate(T *p, std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        if (N < num_stored) {
            free(p);
        }
        num_stored -= n;
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }

private:
    T* sequence;
    mutable std::size_t num_stored;
};

int main(int, char *[]) {
    const std::size_t N = 15;
    auto container = std::list<int, smart_allocator<int, N>>{};
    for (size_t i = 0; i < N; ++i) {
        container.emplace_back(i);
        std::cout << "---" << std::endl;
    }

    std::size_t shift = 0;
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (it != container.begin())
            shift = &(*it) - &(*(std::prev(it)));
        std::cout << "Address: "<< &(*it) << "\tValue: " << *it << "\tShift: " << shift << std::endl;
    }

    return 0;
}

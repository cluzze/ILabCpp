#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <iterator>
#include <compare>

namespace containers {
    template <typename T, typename Alloc = std::allocator<T>>
    class vectorBuf {
    protected:
        static_assert(std::is_nothrow_move_constructible<T>::value);
        static_assert(std::is_nothrow_move_assignable<T>::value);
        static_assert(std::is_nothrow_destructible<T>::value);

        vectorBuf(const vectorBuf& rhs) = delete;
        vectorBuf& operator=(const vectorBuf& rhs) = delete;
        
        vectorBuf(vectorBuf&& rhs) noexcept
        : capacity_(std::exchange(rhs.capacity_, 0))
            , size_(std::exchange(rhs.size_, 0))
            , arr(std::exchange(rhs.arr, nullptr)) {}

        vectorBuf& operator=(vectorBuf&& rhs) noexcept {
            std::swap(capacity_, rhs.capacity_);
            std::swap(size_, rhs.size_);
            std::swap(arr, rhs.arr);
        }

        explicit vectorBuf(std::size_t capacity) :
                capacity_{capacity}, arr{capacity_ == 0 ? nullptr 
                                    : Alloc().allocate(capacity_)} {}

        ~vectorBuf() {
            if (arr) {
                for (std::size_t i = 0; i < size_; ++i)
                    arr[i].~T();
                Alloc().deallocate(arr, capacity_);
            }
        }

    protected:
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
        T* arr;
    };

    template <typename T, typename Alloc = std::allocator<T>>
    class vector : private vectorBuf<T, Alloc> {
        static_assert(std::is_nothrow_move_constructible<T>::value);
        static_assert(std::is_nothrow_move_assignable<T>::value);
        static_assert(std::is_nothrow_destructible<T>::value);

        using vectorBuf<T, Alloc>::capacity_;
        using vectorBuf<T, Alloc>::size_;
        using vectorBuf<T, Alloc>::arr;
    public:
        class iterator {
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference_type = T&;
            using pointer_type = T*;
        
        private:
            pointer_type ptr;

        public:
            iterator(pointer_type ptr_ = nullptr) : ptr{ptr_} {}

            pointer_type   operator->() const { return ptr; }
            reference_type operator*() const { return *ptr; }
            reference_type operator[](difference_type dif) const { return *(ptr + dif); }

            iterator& operator++() {
                ptr++;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp(ptr);
                ptr++;
                return tmp;
            }

            iterator& operator+=(difference_type dif) {
                ptr += dif;
                return *this;
            }

            iterator& operator--() {
                ptr--;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp(ptr);
                ptr--;
                return tmp;
            }

            iterator& operator-=(difference_type dif) {
                ptr -= dif;
                return *this;
            }

            iterator operator-(difference_type dif) const {
                return iterator{ptr - dif};
            }

            difference_type operator-(const iterator& other) const {
                return ptr - other.ptr;
            }

            auto operator<=>(const iterator&) const = default;

            friend iterator operator+(const iterator& it, difference_type dif) {
                return iterator{it.ptr + dif};
            }

            friend iterator operator+(difference_type dif, const iterator& it) {
                return iterator{it.ptr + dif};
            }
        };

        class const_iterator {
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T;
            using reference_type = const T&;
            using pointer_type = const T*;
        
        private:
            pointer_type ptr;

        public:
            const_iterator(pointer_type ptr_ = nullptr) : ptr{ptr_} {}

            pointer_type   operator->() const { return ptr; }
            reference_type operator*() const { return *ptr; }
            reference_type operator[](difference_type dif) const { return *(ptr + dif); }

            const_iterator& operator++() {
                ptr++;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp(ptr);
                ptr++;
                return tmp;
            }

            const_iterator& operator+=(difference_type dif) {
                ptr += dif;
                return *this;
            }

            const_iterator& operator--() {
                ptr--;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp(ptr);
                ptr--;
                return tmp;
            }

            const_iterator& operator-=(difference_type dif) {
                ptr -= dif;
                return *this;
            }

            const_iterator operator-(difference_type dif) const {
                return const_iterator{ptr - dif};
            }

            difference_type operator-(const const_iterator& other) const {
                return ptr - other.ptr;
            }

            auto operator<=>(const const_iterator& other) const = default;

            friend const_iterator operator+(const const_iterator& it, difference_type dif) {
                return const_iterator{it.ptr + dif};
            }

            friend const_iterator operator+(difference_type dif, const const_iterator& it) {
                return const_iterator{it.ptr + dif};
            }
        };    

    public:
        vector() : vectorBuf<T, Alloc>{0} {}
        
        vector(size_t n, const T &value) : vectorBuf<T, Alloc>(new_cap(n)) {
            for (std::size_t i = 0; i < n; i++) {
                new (arr + i) T{value};
                size_++;
            }
        }

        explicit vector(std::size_t n) : vectorBuf<T, Alloc>(new_cap(n)) {
            for (std::size_t i = 0; i < n; i++) {
                new (arr + i) T{};
                size_++;
            }
        }

        vector(const vector &rhs) : vectorBuf<T, Alloc>(new_cap(rhs.size_)) {
            for (std::size_t i = 0; i < rhs.size_; i++) {
                new (arr + i) T(rhs.arr[i]);
                size_++;
            }
        }

        vector(vector &&rhs) noexcept : vectorBuf<T, Alloc>(std::move(rhs)) {
        }

        vector &operator=(vector &&other) noexcept {
            if (this == std::addressof(other))
                return *this;

            std::swap(capacity_, other.capacity_);
            arr = std::exchange(other.arr, nullptr);
            size_ = std::exchange(other.size_, 0);
            
            return *this;
        }

        vector &operator=(const vector &rhs) {
            if (this != &rhs) {
                vector tmp(rhs);
                std::swap(*this, tmp);       
            }
            return *this;
        }

        [[nodiscard]] std::size_t size() const noexcept {
            return size_;
        }

        [[nodiscard]] bool empty() const noexcept {
            return size_ == 0;
        }

        [[nodiscard]] std::size_t capacity() const noexcept {
            return capacity_;
        }

        T &operator[](std::size_t n) &noexcept {
            return arr[n];
        }

        T &&operator[](std::size_t n) &&noexcept {
            return std::move(arr[n]);
        }

        const T &operator[](std::size_t n) const &noexcept {
            return arr[n];
        }

        T &at(std::size_t n) & {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return arr[n];
        }

        T &&at(std::size_t n) && {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return std::move(arr[n]);
        }

        const T &at(std::size_t n) const & {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return arr[n];
        }

        void reserve(std::size_t n) & {
            if (capacity_ < n) {
                std::size_t new_capacity = new_cap(n);
                T *buf = All(new_capacity);
                std::size_t i = 0;
                try {
                    for (; i != size_; i++) {
                        new (buf + i) T(std::move(arr[i]));
                    }
                    std::swap(arr, buf);
                    del(0, size_, buf);
                    Alloc().deallocate(buf, capacity_);
                    capacity_ = new_capacity;
                } catch (...) {
                    del(0, i, buf);
                    Alloc().deallocate(buf, new_capacity);
                    throw;
                }
            }
        }

        void push_back(const T &value) & {
            try {
                resize(size_ + 1, value);
            } catch (...) {
                throw;
            }
        }

        void push_back(T &&value) & {
            if (size_ == capacity_) {
                if (capacity_ == 0) {
                    capacity_ = 2;
                    arr = All(capacity_);
                } else {
                    std::size_t new_capacity_ = capacity_ * 2;
                    T *buf = All(new_capacity_);
                    std::size_t i = 0;
                    try {
                        for (; i < size_; i++) {
                            new (buf + i) T(std::move(arr[i]));
                        }
                        Alloc().deallocate(arr, capacity_);
                        arr = buf;
                        capacity_ = new_capacity_;
                    } catch (...) {
                        del(0, i, buf);
                        Alloc().deallocate(buf, new_capacity_);
                        throw;
                    }
                }
            }
            try {
                new (arr + size_) T(std::move(value));
            } catch (...) {
                throw;
            }
            size_++;
        }

        void pop_back() &noexcept {
            size_--;
            arr[size_].~T();
        }

        void resize(std::size_t n) & {
            if (size_ < n) {
                std::size_t new_capacity = new_cap(n);
                T *buf = All(new_capacity);
                std::size_t i = size_;
                try {
                    for (; i != n; i++) {
                        new (buf + i) T();
                    }
                    for (std::size_t j = 0; j < size_; j++) {
                        new (buf + j) T(std::move(arr[j]));
                    }
                    std::swap(arr, buf);
                    del(0, size_, buf);
                    Alloc().deallocate(buf, capacity_);
                    capacity_ = new_capacity;
                    size_ = n;
                } catch (...) {
                    del(size_, i, buf);
                    Alloc().deallocate(buf, new_capacity);
                    throw;
                }
            } else if (size_ > n) {
                del(n, size_, arr);
                size_ = n;
            }
        }

        void resize(std::size_t n, const T &value) & {
            if (size_ < n) {
                std::size_t new_capacity = new_cap(n);
                T *buf = All(new_capacity);
                std::size_t i = size_;
                try {
                    for (; i != n; i++) {
                        new (buf + i) T(value);
                    }
                    for (std::size_t j = 0; j < size_; j++) {
                        new (buf + j) T(std::move(arr[j]));
                    }
                    std::swap(arr, buf);
                    del(0, size_, buf);
                    Alloc().deallocate(buf, capacity_);
                    capacity_ = new_capacity;
                    size_ = n;
                } catch (...) {
                    del(size_, i, buf);
                    Alloc().deallocate(buf, new_capacity);
                    throw;
                }
            } else if (size_ > n) {
                del(n, size_, arr);
                size_ = n;
            }
        }

        void clear() &noexcept {
            del(0, size_, arr);
            size_ = 0;
        }

        iterator begin() const {
            return iterator{arr};
        }

        iterator end() const {
            return iterator{arr + size};
        }

        const_iterator const begin() {
            return iterator{arr};
        }

        const_iterator const end() {
            return iterator{arr + size};
        }

    private:
        std::size_t new_cap(std::size_t x) noexcept {
            if (x == 0)
                return 0;

            std::size_t y = 1;

            while (y < x)
                y *= 2;
            return y;
        }

        void del(std::size_t i, std::size_t j, T *buf) noexcept {
            for (std::size_t k = i; k < j; k++) {
                buf[k].~T();
            }
        }

        T* All(std::size_t n) {
            if (n == 0) {
                return nullptr;
            }
            return Alloc().allocate(n);
        }
    };
}
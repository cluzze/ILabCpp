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
    template <typename T, typename Allocator = std::allocator<T>>
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
                                    : Allocator().allocate(capacity_)} {}

        ~vectorBuf() {
            if (arr) {
                for (std::size_t i = 0; i < size_; ++i)
                    arr[i].~T();
                Allocator().deallocate(arr, capacity_);
            }
        }

    protected:
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
        T* arr;
    };

    template <typename T, typename Allocator = std::allocator<T>>
    class vector : private vectorBuf<T, Allocator> {
        static_assert(std::is_nothrow_move_constructible<T>::value);
        static_assert(std::is_nothrow_move_assignable<T>::value);
        static_assert(std::is_nothrow_destructible<T>::value);

        using vectorBuf<T, Allocator>::capacity_;
        using vectorBuf<T, Allocator>::size_;
        using vectorBuf<T, Allocator>::arr;
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = typename std::allocator_traits<Allocator>::size_type;
        using difference_type = typename std::allocator_traits<Allocator>::difference_type;
    
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
        vector() : vectorBuf<T, Allocator>{0} {}
        
        vector(size_type n, const T &value) : vectorBuf<T, Allocator>(new_cap(n)) {
            for (size_type i = 0; i < n; i++) {
                new (arr + i) T{value};
                size_++;
            }
        }

        explicit vector(size_type n) : vectorBuf<T, Allocator>(new_cap(n)) {
            for (size_type i = 0; i < n; i++) {
                new (arr + i) T{};
                size_++;
            }
        }

        vector(const vector &rhs) : vectorBuf<T, Allocator>(new_cap(rhs.size_)) {
            for (size_type i = 0; i < rhs.size_; i++) {
                new (arr + i) T(rhs.arr[i]);
                size_++;
            }
        }

        vector(vector &&rhs) noexcept : vectorBuf<T, Allocator>(std::move(rhs)) {
        }

        vector &operator=(vector &&rhs) noexcept {
            if (this == std::addressof(rhs))
                return *this;

            std::swap(capacity_, rhs.capacity_);
            std::swap(arr, rhs.arr);
            std::swap(size_, rhs.size_);
            
            return *this;
        }

        vector &operator=(const vector &rhs) {
            if (this == std::addressof(rhs))
                return *this;

            vector tmp(rhs);
            std::swap(*this, tmp);       

            return *this;
        }

        [[nodiscard]] size_type size() const noexcept {
            return size_;
        }

        [[nodiscard]] bool empty() const noexcept {
            return size_ == 0;
        }

        [[nodiscard]] size_type capacity() const noexcept {
            return capacity_;
        }

        T &operator[](size_type n) & {
            return arr[n];
        }

        T &&operator[](size_type n) && {
            return std::move(arr[n]);
        }

        const T &operator[](size_type n) const & {
            return arr[n];
        }

        T &at(size_type n) & {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return arr[n];
        }

        T &&at(size_type n) && {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return std::move(arr[n]);
        }

        const T &at(size_type n) const & {
            if (n >= size_) {
                throw std::out_of_range("out of range");
            }
            return arr[n];
        }

        void reserve(size_type n) & {
            if (capacity_ >= n)
                return;
            
            size_type new_capacity = new_cap(n);
            T *buf = All(new_capacity);
            size_type i = 0;
            try {
                for (; i != size_; i++) {
                    new (buf + i) T(std::move(arr[i]));
                }
                std::swap(arr, buf);
                del(0, size_, buf);
                Allocator().deallocate(buf, capacity_);
                capacity_ = new_capacity;
            } catch (...) {
                del(0, i, buf);
                Allocator().deallocate(buf, new_capacity);
                throw;
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
                size_type new_capacity_ = capacity_ != 0 ? capacity_ * 2 : 2;
                T *buf = All(new_capacity_);
                size_type i = 0;
                try {
                    for (; i < size_; i++) {
                        new (buf + i) T(std::move(arr[i]));
                    }
                    Allocator().deallocate(arr, capacity_);
                    arr = buf;
                    capacity_ = new_capacity_;
                } catch (...) {
                    del(0, i, buf);
                    Allocator().deallocate(buf, new_capacity_);
                    throw;
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

        void resize(size_type n) & {
            if (size_ < n) {
                size_type new_capacity = new_cap(n);
                T *buf = All(new_capacity);
                size_type i = size_;
                try {
                    for (; i != n; i++) {
                        new (buf + i) T{};
                    }
                    for (size_type j = 0; j < size_; j++) {
                        new (buf + j) T(std::move(arr[j]));
                    }
                    std::swap(arr, buf);
                    del(0, size_, buf);
                    Allocator().deallocate(buf, capacity_);
                    capacity_ = new_capacity;
                    size_ = n;
                } catch (...) {
                    del(size_, i, buf);
                    Allocator().deallocate(buf, new_capacity);
                    throw;
                }
            } else if (size_ > n) {
                del(n, size_, arr);
                size_ = n;
            }
        }

        void resize(size_type n, const T &value) & {
            if (size_ < n) {
                size_type new_capacity = new_cap(n);
                T *buf = All(new_capacity);
                size_type i = size_;
                try {
                    for (; i != n; i++) {
                        new (buf + i) T{value};
                    }
                    for (size_type j = 0; j < size_; j++) {
                        new (buf + j) T(std::move(arr[j]));
                    }
                    std::swap(arr, buf);
                    del(0, size_, buf);
                    Allocator().deallocate(buf, capacity_);
                    capacity_ = new_capacity;
                    size_ = n;
                } catch (...) {
                    del(size_, i, buf);
                    Allocator().deallocate(buf, new_capacity);
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

        iterator begin() {
            return iterator(arr);
        }

        iterator end() {
            return iterator(arr + size_);
        }

        const_iterator begin() const {
            return const_iterator(arr);
        }

        const_iterator end() const {
            return const_iterator(arr + size_);
        }

        template <typename It>
        vector(It begin, It end) : vectorBuf<T, Allocator>(1) {
            for (; begin != end; ++begin)
                push_back(*begin);
        }

    private:
        size_type new_cap(size_type x) noexcept {
            if (x == 0)
                return 0;

            size_type y = 1;

            while (y < x)
                y *= 2;
            return y;
        }

        void del(size_type i, size_type j, T *buf) noexcept {
            for (size_type k = i; k < j; k++) {
                buf[k].~T();
            }
        }

        T* All(size_type n) {
            if (n == 0) {
                return nullptr;
            }
            return Allocator().allocate(n);
        }
    };
}
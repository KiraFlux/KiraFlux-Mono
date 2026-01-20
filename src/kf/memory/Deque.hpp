// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/config.hpp"

#include "kf/memory/Allocator.hpp"

#if kf_port_has_deque

#include <deque>

namespace kf {
template<typename T, typename Alloc = kf::Allocator<T>> using Deque = std::deque<T, Alloc>;
}

#else

#include "kf/algorithm.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"
#include "kf/core/eutility.hpp"
#include "kf/core/type_traits.hpp"

namespace kf {

template<typename T, typename Alloc = allocator<T>> class Deque {
private:
    T *buffer_ = nullptr;
    usize capacity_ = 0;
    usize size_ = 0;
    usize front_ = 0;
    Alloc alloc_;

public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = usize;
    using reference = value_type &;
    using const_reference = const value_type &;

    Deque() = default;

    explicit Deque(const allocator_type &alloc) :
        alloc_(alloc) {}

    ~Deque() {
        clear();
        if (buffer_) {
            alloc_.deallocate(buffer_, capacity_);
        }
    }

    // Доступ к элементам
    reference operator[](size_type pos) {
        return buffer_[(front_ + pos) % capacity_];
    }

    const_reference operator[](size_type pos) const {
        return buffer_[(front_ + pos) % capacity_];
    }

    reference front() { return buffer_[front_]; }
    const_reference front() const { return buffer_[front_]; }

    reference back() { return buffer_[(front_ + size_ - 1) % capacity_]; }
    const_reference back() const { return buffer_[(front_ + size_ - 1) % capacity_]; }

    kf_nodiscard bool empty() const { return size_ == 0; }

    kf_nodiscard size_type size() const { return size_; }

    kf_nodiscard size_type capacity() const { return capacity_; }

    void push_back(const T &value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 4);
        }
        usize pos = (front_ + size_) % capacity_;
        allocator_traits<Alloc>::construct(alloc_, buffer_ + pos, value);
        ++size_;
    }

    void push_back(T &&value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 4);
        }
        usize pos = (front_ + size_) % capacity_;
        allocator_traits<Alloc>::construct(alloc_, buffer_ + pos, kf::move(value));
        ++size_;
    }

    void push_front(const T &value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 4);
        }
        front_ = (front_ == 0) ? capacity_ - 1 : front_ - 1;
        allocator_traits<Alloc>::construct(alloc_, buffer_ + front_, value);
        ++size_;
    }

    void push_front(T &&value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 4);
        }
        front_ = (front_ == 0) ? capacity_ - 1 : front_ - 1;
        allocator_traits<Alloc>::construct(alloc_, buffer_ + front_, kf::move(value));
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            usize pos = (front_ + size_ - 1) % capacity_;
            allocator_traits<Alloc>::destroy(alloc_, buffer_ + pos);
            --size_;
        }
    }

    void pop_front() {
        if (size_ > 0) {
            allocator_traits<Alloc>::destroy(alloc_, buffer_ + front_);
            front_ = (front_ + 1) % capacity_;
            --size_;
        }
    }

    void clear() {
        for (usize i = 0; i < size_; ++i) {
            usize pos = (front_ + i) % capacity_;
            allocator_traits<Alloc>::destroy(alloc_, buffer_ + pos);
        }
        size_ = 0;
        front_ = 0;
    }

    void reserve(size_type new_capacity) {
        if (new_capacity <= capacity_) return;

        T *new_buffer = alloc_.allocate(new_capacity);

        for (usize i = 0; i < size_; ++i) {
            usize old_pos = (front_ + i) % capacity_;
            usize new_pos = i;
            allocator_traits<Alloc>::construct(alloc_, new_buffer + new_pos,
                                               kf::move(buffer_[old_pos]));
            allocator_traits<Alloc>::destroy(alloc_, buffer_ + old_pos);
        }

        if (buffer_) {
            alloc_.deallocate(buffer_, capacity_);
        }

        buffer_ = new_buffer;
        capacity_ = new_capacity;
        front_ = 0;
    }

    Deque(const Deque &) = delete;
    Deque &operator=(const Deque &) = delete;
};

}// namespace kf

#endif

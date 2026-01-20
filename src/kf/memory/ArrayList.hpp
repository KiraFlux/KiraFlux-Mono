// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/config.hpp"
#include "kf/memory/Allocator.hpp"

#if kf_port_has_vector
#include <vector>

namespace kf {
template<typename T, typename Alloc = kf::Allocator<T>> using ArrayList = std::vector<T, Alloc>;
}

#else

#include "kf/algorithm.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"
#include "kf/core/type_traits.hpp"
#include "kf/core/utility.hpp"

namespace kf {
template<typename T, typename Alloc = kf::allocator<T>> class ArrayList {
private:
    T *data_ = nullptr;
    usize size_ = 0;
    usize capacity_ = 0;
    Alloc alloc_;

public:
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = usize;

    ArrayList() = default;

    ~ArrayList() {
        clear();
        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }
    }

    // Доступ к элементам
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

    pointer data() { return data_; }
    kf_nodiscard size_type size() const { return size_; }
    kf_nodiscard bool empty() const { return size_ == 0; }

    // Добавление элементов
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 4);
        }
        allocator_traits<Alloc>::construct(alloc_, data_ + size_, value);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            allocator_traits<Alloc>::destroy(alloc_, data_ + size_);
        }
    }

    // Очистка
    void clear() {
        for (usize i = 0; i < size_; ++i) {
            allocator_traits<Alloc>::destroy(alloc_, data_ + i);
        }
        size_ = 0;
    }

    // Резервирование
    void reserve(size_type new_capacity) {
        if (new_capacity <= capacity_) return;

        T *new_data = alloc_.allocate(new_capacity);

        for (usize i = 0; i < size_; ++i) {
            allocator_traits<Alloc>::construct(alloc_, new_data + i, kf::move(data_[i]));
            allocator_traits<Alloc>::destroy(alloc_, data_ + i);
        }

        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_capacity;
    }

    ArrayList(const ArrayList &) = delete;
    ArrayList &operator=(const ArrayList &) = delete;
};

}// namespace kf

#endif
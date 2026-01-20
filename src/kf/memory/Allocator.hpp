// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/config.hpp"

#if kf_port_has_memory
#include <memory>

namespace kf {

template<typename T> using Allocator = std::allocator<T>;
using std::allocator_traits;

}// namespace kf

#else

#include "kf/core/aliases.hpp"
#include "kf/core/type_traits.hpp"
#include "kf/core/utility.hpp"

// Basic placement new for AVR
inline void *operator new(kf::usize, void *ptr) noexcept {
    return ptr;
}

inline void operator delete(void *, void *) noexcept {}

namespace kf {

template<typename T> class Allocator {
public:
    using value_type = T;
    using pointer = T *;
    using size_type = kf::usize;

    pointer allocate(size_type n) {
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }

    template<typename U, typename... Args> void construct(U *p, Args &&...args) {
        ::new (static_cast<void *>(p)) U(forward<Args>(args)...);
    }

    template<typename U> void destroy(U *p) {
        p->~U();
    }
};

template<typename Alloc> struct allocator_traits {
    using allocator_type = Alloc;

    template<typename U> using rebind_alloc = allocator<U>;

    static typename Alloc::pointer allocate(Alloc &a, kf::usize n) {
        return a.allocate(n);
    }

    static void deallocate(Alloc &a, typename Alloc::pointer p, kf::usize n) {
        a.deallocate(p, n);
    }

    template<typename U, typename... Args> static void construct(Alloc &a, U *p, Args &&...args) {
        a.construct(p, forward<Args>(args)...);
    }

    template<typename U> static void destroy(Alloc &a, U *p) {
        a.destroy(p);
    }
};

}// namespace kf

#endif
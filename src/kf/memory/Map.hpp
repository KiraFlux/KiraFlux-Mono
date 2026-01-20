// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <map>

#include "kf/memory/Allocator.hpp"

namespace kf {

template<typename K, typename V, typename C = std::less<K>, typename A = Allocator<std::pair<K, V>>> using Map = std::map<K, V, C, A>;

}

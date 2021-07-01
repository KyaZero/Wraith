#pragma once

// STL includes
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>


// Variant helper
template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

// Windows includes
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::WeakRef;

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// Engine includes
#include "Core/Logger.h"
#include "Core/Math/Mat.h"
#include "Core/Math/Vec.h"
#include "Core/Types.h"
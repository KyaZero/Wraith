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
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

// Windows includes
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifdef INFINITE
#undef INFINITE
#endif

// Thirdparty includes
#include <dubu_serialize/dubu_serialize.hpp>

// Engine includes
#include "Core/Constants.h"
#include "Core/Logger.h"
#include "Core/Math/Mat.h"
#include "Core/Math/Vec.h"
#include "Core/StringID.h"
#include "Core/Types.h"
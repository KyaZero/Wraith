if(NOT TARGET compiler_features)
    add_library(compiler_features INTERFACE)
    # this change happened in CMake 3.20.4
    if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
        target_compile_features(compiler_features INTERFACE cxx_std_23) # /std:c++latest - unlocks the non stable cpp20 features. For new 16.11 versions
    else()
        target_compile_features(compiler_features INTERFACE cxx_std_20)
    endif()
endif()

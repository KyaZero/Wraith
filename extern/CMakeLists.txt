cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(FetchContent)

set(FETCHCONTENT_QUIET ON)

set(target_name Extern)

# ------------------------------------------------------------------------------
# A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and
# input http://www.glfw.org/
message("-- Fetching External Project: glfw")
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)

message("-- Fetching External Project: imgui")
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG cce307a2be4d26c34cce7d4844381e18cc49f656
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(src_imgui
        "${imgui_SOURCE_DIR}/imconfig.h"
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui.h"
        "${imgui_SOURCE_DIR}/imgui_demo.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_internal.h"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/imstb_rectpack.h"
        "${imgui_SOURCE_DIR}/imstb_textedit.h"
        "${imgui_SOURCE_DIR}/imstb_truetype.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
        )
        
    add_library(imgui STATIC ${src_imgui})
    
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
        
    set_target_properties(imgui PROPERTIES FOLDER "thirdparty")

endif()

message("-- Fetching External Project: entt")
FetchContent_Declare(entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
)
FetchContent_MakeAvailable(entt)
set_target_properties(aob PROPERTIES FOLDER "thirdparty")


message("-- Fetching External Project: stb_image")
FetchContent_Declare(stb_image
  GIT_REPOSITORY https://github.com/nothings/stb
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(stb_image)
if (NOT stb_image)
    FetchContent_Populate(stb_image)
endif()

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})

# Simply to have some color in the console, and this library is windows only
if(WIN32)
	message("-- Fetching External Project: color-console")

	FetchContent_Declare(color-console
	  GIT_REPOSITORY https://github.com/imfl/color-console
	  CONFIGURE_COMMAND ""
	  BUILD_COMMAND ""
	)

	FetchContent_GetProperties(color-console)
	if (NOT color-console_POPULATED)
	    FetchContent_Populate(color-console)
	endif()

	add_library(color-console INTERFACE)
	target_include_directories(color-console INTERFACE ${color-console_SOURCE_DIR}/include)
endif()
message("-- Fetching External Project: glfw")

FetchContent_Declare(
  glfw
  GIT_REPOSITORY  https://github.com/glfw/glfw.git
  GIT_TAG         3.3.4
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

set_target_properties(glfw PROPERTIES FOLDER "thirdparty/glfw")
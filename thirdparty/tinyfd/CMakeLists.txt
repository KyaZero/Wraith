set(tinyfd_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(tinyfd_sources
  ${tinyfd_include_dir}/tinyfd/tinyfiledialogs.c
  ${tinyfd_include_dir}/tinyfd/tinyfiledialogs.h)

add_library(tinyfd STATIC ${tinyfd_sources})

target_include_directories(tinyfd PUBLIC ${tinyfd_include_dir})
set_target_properties(tinyfd PROPERTIES FOLDER "thirdparty/tinyfd")
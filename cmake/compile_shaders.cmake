function(compile_shaders TARGET SHADERS_IN_DIR SHADERS_OUT_DIR)
    find_program(GLSLC glslc)

    file(GLOB_RECURSE SHADERS "${SHADERS_IN_DIR}/*.vert" "${SHADERS_IN_DIR}/*.frag" "${SHADERS_IN_DIR}/*.task" "${SHADERS_IN_DIR}/*.mesh" "${SHADERS_IN_DIR}/*.comp")
    file(MAKE_DIRECTORY ${SHADERS_OUT_DIR})

    foreach(SHADER ${SHADERS})
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        set(OUTNAME ${SHADERS_OUT_DIR}/${SHADER_NAME}.spv)
        add_custom_command(
            OUTPUT ${OUTNAME}
            COMMAND ${GLSLC} ${SHADER} -o ${OUTNAME}
            COMMENT "Compiling ${SHADER_NAME}"
            DEPENDS ${SHADER}
            )
        list(APPEND SPV_SHADERS ${OUTNAME})
    endforeach()

    add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})
    add_dependencies(${TARGET} Shaders)

    set_target_properties(Shaders PROPERTIES FOLDER "compile")

endfunction()
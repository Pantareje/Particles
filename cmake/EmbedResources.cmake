function(embed_resources target files)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Embed")

    foreach (bin IN LISTS files)
        get_filename_component(filename ${bin} NAME)
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})

        set(source_file "${CMAKE_CURRENT_BINARY_DIR}/Embed/${filename}.h")

        file(READ ${bin} filedata HEX)

        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        set(resource_code "constexpr unsigned char ${filename}[] = {${filedata}};\n")

        file(WRITE ${source_file} "${resource_code}")

        target_sources(${target} PRIVATE ${source_file})
    endforeach ()
endfunction()

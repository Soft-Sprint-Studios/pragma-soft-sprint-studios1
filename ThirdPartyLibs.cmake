# Luabind
set(LUA_INCLUDE_DIR ${DEPENDENCY_LUAJIT_INCLUDE} CACHE STRING "" FORCE)
set(LUA_LIBRARY ${DEPENDENCY_LUA_LIBRARY} CACHE STRING "" FORCE)
set(LUA_FOUND ON CACHE BOOL ON FORCE)
set(LUA_LIBRARIES ${LUA_LIBRARY} CACHE STRING "" FORCE)
pr_include_third_party_library(luabind INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/luabind")
pr_set_target_folder(luabind third_party_libs)
target_include_directories(luabind PUBLIC ${LUA_INCLUDE_DIR})
target_include_directories(luabind PUBLIC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/luabind")
#

# mpaheader
pr_include_third_party_library(mpaheader)
pr_set_target_folder(mpaheader third_party_libs)
#

# VKVParser
pr_include_third_party_library(
    vkv_parser
    TARGET
    KVParser
    DIR
    vkvparser
    INC
    "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/vkvparser/include")
#

# spdlog
set(SPDLOG_BUILD_SHARED
    ON
    CACHE BOOL ON FORCE)
set(SPDLOG_BUILD_PIC
    ON
    CACHE BOOL ON FORCE)
if(WIN32)
    set(SPDLOG_USE_STD_FORMAT
        ON
        CACHE BOOL ON FORCE)
else()
    set(SPDLOG_USE_STD_FORMAT
        OFF
        CACHE BOOL OFF FORCE)
endif()
set(SPDLOG_CLOCK_COARSE
    ON
    CACHE BOOL ON FORCE)
set(CMAKE_CXX_STANDARD 20) # bump to c++23 after we change scope ops in luabind.
pr_include_third_party_library(spdlog)
pr_set_target_folder(spdlog third_party_libs)
unset(CMAKE_CXX_STANDARD)
#

# fmt
unset(BUILD_SHARED_LIBS CACHE)
set(FMT_TEST
    OFF
    CACHE BOOL OFF FORCE)
set(FMT_DOC
    OFF
    CACHE BOOL OFF FORCE)
pr_include_third_party_library(fmt)
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL ".." FORCE)
#

# 7zip cpp
if(WIN32)
    set(BUILD_SHARED_LIBS
        OFF
        CACHE BOOL OFF FORCE)
    pr_include_third_party_library(7zpp)
    set(BUILD_SHARED_LIBS
        ON
        CACHE BOOL ".." FORCE)
endif()

# muparserx
set(BUILD_EXAMPLES
    OFF
    CACHE BOOL OFF FORCE)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL OFF FORCE)
pr_include_third_party_library(muparserx INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/muparserx/parser")
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL ".." FORCE)
pr_set_target_folder(muparserx third_party_libs/muparserx)
#

# clip2tri
set(M_LIBRARY "")
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL OFF FORCE)
set(BUILD_STATIC_LIBS
    ON
    CACHE BOOL ON FORCE)
pr_include_third_party_library(clip2tri TARGET clip2tri-static INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/clip2tri")
pr_include_third_party_library(NAME clipper TARGET clipper-static)
pr_include_third_party_library(NAME poly2tri TARGET poly2tri-static)
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL ".." FORCE)
pr_set_target_folder(clip2tri third_party_libs/clip2tri)
pr_set_target_folder(clip2tri-static third_party_libs/clip2tri)
pr_set_target_folder(clipper third_party_libs/clip2tri)
pr_set_target_folder(clipper-static third_party_libs/clip2tri)
pr_set_target_folder(poly2tri third_party_libs/clip2tri)
pr_set_target_folder(poly2tri-static third_party_libs/clip2tri)
#
# BUG: M_LIBRARY is not restored to previous value breaking anything using it.

# eigen register_third_party_library(eigen)
#

# glfw
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL OFF FORCE)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL OFF FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL OFF FORCE)
set(GLFW_INSTALL
    OFF
    CACHE BOOL OFF FORCE)
if(WIN32)
    set(GLFW_LIB_NAME
        "glfw3dll"
        CACHE INTERNAL "" FORCE)
else()
    set(GLFW_LIB_NAME
        "glfw"
        CACHE INTERNAL "" FORCE)
endif()
pr_include_third_party_library(glfw)
pr_set_target_folder(glfw third_party_libs/glfw)
#

# libogg
pr_include_third_party_library(ogg)
pr_set_target_folder(ogg third_party_libs)
set(OGG_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/ogg/include" CACHE STRING "" FORCE)
set(OGG_LIBRARIES "$<TARGET_LINKER_FILE:ogg>" CACHE STRING "" FORCE)
target_include_directories(ogg INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/ogg/include>")
#

# libvorbis
pr_include_third_party_library(vorbis TARGET vorbis)
pr_include_third_party_library(TARGET vorbisfile)
pr_set_target_folder(vorbis third_party_libs/vorbis)
pr_set_target_folder(vorbisenc third_party_libs/vorbis)
pr_set_target_folder(vorbisfile third_party_libs/vorbis)

target_include_directories(vorbis PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/vorbis/include>")
target_include_directories(vorbis PUBLIC "$<BUILD_INTERFACE:${OGG_INCLUDE_DIRS}>")
target_include_directories(vorbis PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/ogg/include>")

add_dependencies(vorbis ogg)
#

# zlib
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/third_party_libs/zlib CACHE STRING "" FORCE)
pr_include_third_party_library(zlib INC "${ZLIB_INCLUDE_DIR}")

target_include_directories(zlib PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/zlib>")

target_include_directories(zlibstatic PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/zlib" "${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/zlib")
pr_set_target_folder(example third_party_libs/zlib)
pr_set_target_folder(minigzip third_party_libs/zlib)
pr_set_target_folder(zlib third_party_libs/zlib)
pr_set_target_folder(zlibstatic third_party_libs/zlib)

# if(UNIX) intervene here beacuse libzip uses pkgconfig
set(ZLIB_INCLUDE_DIR
    ${DEPENDENCY_ZLIB_INCLUDE}
    CACHE STRING "" FORCE)
set(ZLIB_LIBRARY
    "$<TARGET_LINKER_FILE:zlib>"
    CACHE STRING "" FORCE)
add_library(ZLIB::ZLIB ALIAS zlib) # for libzip just ahead.
set(ZLIB_LINK_LIBRARY_NAME "ZLIB::ZLIB")
# endif()
#

# libzip
set(LIBZIP_DO_INSTALL
    OFF
    CACHE BOOL ".." FORCE)
set(ENABLE_BZIP2
    OFF
    CACHE BOOL ".." FORCE)
set(ENABLE_LZMA
    OFF
    CACHE BOOL ".." FORCE)
pr_include_third_party_library(libzip TARGET zip INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libzip/lib")
pr_set_include_path(libzip_conf "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libzip/xcode")
target_include_directories(zip PUBLIC ${DEPENDENCY_ZLIB_INCLUDE}) # ew.
pr_set_target_folder(add_from_filep third_party_libs/libzip)
pr_set_target_folder(can_clone_file third_party_libs/libzip)
pr_set_target_folder(check third_party_libs/libzip)
pr_set_target_folder(cleanup third_party_libs/libzip)
pr_set_target_folder(dist third_party_libs/libzip)
pr_set_target_folder(distcheck third_party_libs/libzip)
pr_set_target_folder(fopen_unchanged third_party_libs/libzip)
pr_set_target_folder(fread third_party_libs/libzip)
pr_set_target_folder(fseek third_party_libs/libzip)
pr_set_target_folder(hole third_party_libs/libzip)
# pr_set_target_folder(in-memory third_party_libs/libzip)
pr_set_target_folder(man third_party_libs/libzip)
pr_set_target_folder(nonrandomopen third_party_libs/libzip)
pr_set_target_folder(nonrandomopentest third_party_libs/libzip)
pr_set_target_folder(testinput third_party_libs/libzip)
pr_set_target_folder(tryopen third_party_libs/libzip)
# pr_set_target_folder(update_zip_err_str third_party_libs/libzip)
pr_set_target_folder(update_zip_errors third_party_libs/libzip)
pr_set_target_folder(update-html third_party_libs/libzip)
pr_set_target_folder(update-man third_party_libs/libzip)
pr_set_target_folder(zip third_party_libs/libzip)
pr_set_target_folder(zipcmp third_party_libs/libzip)
pr_set_target_folder(zipmerge third_party_libs/libzip)
pr_set_target_folder(ziptool third_party_libs/libzip)
pr_set_target_folder(ziptool_regress third_party_libs/libzip)
pr_set_target_folder(lzma third_party_libs/libzip)

add_dependencies(zip zlib)
#

# bzip2
pr_include_third_party_library(bzip2 TARGET libz2 INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/bzip2")
pr_set_target_folder(bzip2 third_party_libs/bzip2)
pr_set_target_folder(bzip2recover third_party_libs/bzip2)
pr_set_target_folder(libz2 third_party_libs/bzip2)
#

# libnoise
set(BUILD_LIBNOISE_EXAMPLES
    OFF
    CACHE BOOL ".." FORCE)
set(BUILD_LIBNOISE_UTILS
    ON
    CACHE BOOL ".." FORCE)
unset(BUILD_SHARED_LIBS CACHE)

pr_include_third_party_library(libnoise TARGET noise-static INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libnoise/src")
pr_include_third_party_library(TARGET noiseutils-static INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libnoise/noiseutils")
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL ".." FORCE)
pr_set_target_folder(noise third_party_libs/libnoise)
pr_set_target_folder(noise-static third_party_libs/libnoise)
pr_set_target_folder(noiseutils third_party_libs/libnoise)
pr_set_target_folder(noiseutils-static third_party_libs/libnoise)

target_include_directories(noiseutils-static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libnoise/src)

pr_find_library(libnoise NAMES "noise" PATHS "third_party_libs/libnoise/src/${BINARY_PRAGMA_DIR}")
pr_find_library(libnoiseutils NAMES "noiseutils-static" PATHS "third_party_libs/libnoise/noiseutils/${BINARY_PRAGMA_DIR}")
#

# libpng
pr_include_third_party_library(libpng NAME lpng TARGET png_static INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libpng")
pr_set_include_path(lpng_build "${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/libpng")
add_dependencies(png_static zlib)

target_include_directories(png_static PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/libpng>")
#

pr_set_target_folder(genfiles third_party_libs/libpng)
pr_set_target_folder(png third_party_libs/libpng)
pr_set_target_folder(png_static third_party_libs/libpng)
pr_set_target_folder(pngfix third_party_libs/libpng)
pr_set_target_folder(png-fix-itxt third_party_libs/libpng)
pr_set_target_folder(pngimage third_party_libs/libpng)
pr_set_target_folder(pngstest third_party_libs/libpng)
pr_set_target_folder(pngtest third_party_libs/libpng)
pr_set_target_folder(pngunknown third_party_libs/libpng)
pr_set_target_folder(pngvalid third_party_libs/libpng)

target_include_directories(png PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/zlib)
#

# libsamplerate (required for alsoundsystem with fmod enabled)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL OFF FORCE)
pr_include_third_party_library(libsamplerate TARGET samplerate INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/libsamplerate/src")
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL ON FORCE)
pr_set_target_folder(callback_hang_test third_party_libs/libsamplerate)
pr_set_target_folder(callback_test third_party_libs/libsamplerate)
pr_set_target_folder(clone_test third_party_libs/libsamplerate)
pr_set_target_folder(downsample_test third_party_libs/libsamplerate)
pr_set_target_folder(float_short_test third_party_libs/libsamplerate)
pr_set_target_folder(misc_test third_party_libs/libsamplerate)
pr_set_target_folder(multi_channel_test third_party_libs/libsamplerate)
pr_set_target_folder(multichan_throughput_test third_party_libs/libsamplerate)
pr_set_target_folder(nullptr_test third_party_libs/libsamplerate)
pr_set_target_folder(reset_test third_party_libs/libsamplerate)
pr_set_target_folder(samplerate third_party_libs/libsamplerate)
pr_set_target_folder(simple_test third_party_libs/libsamplerate)
pr_set_target_folder(snr_bw_test third_party_libs/libsamplerate)
pr_set_target_folder(streaming_test third_party_libs/libsamplerate)
pr_set_target_folder(termination_test third_party_libs/libsamplerate)
pr_set_target_folder(throughput_test third_party_libs/libsamplerate)
pr_set_target_folder(timewarp-file third_party_libs/libsamplerate)
pr_set_target_folder(varispeed_test third_party_libs/libsamplerate)
pr_set_target_folder(varispeed-play third_party_libs/libsamplerate)
#

# vtflib
set(USE_LIBTXC_DXTN
    OFF
    CACHE BOOL OFF FORCE)
pr_set_include_path(vtflib_build "${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/vtflib/src")
pr_include_third_party_library(vtflib TARGET VTFLib13 INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/vtflib/src")
pr_set_target_folder(VTFLib13 third_party_libs)

target_include_directories(VTFLib13 PUBLIC "$<BUILD_INTERFACE:${DEPENDENCY_LIBZIP_CONF_INCLUDE}>")
target_include_directories(VTFLib13 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/vtflib/src>")
#

# recastnavigation
set(RECASTNAVIGATION_DEMO
    OFF
    CACHE BOOL OFF FORCE)
set(RECASTNAVIGATION_EXAMPLES
    OFF
    CACHE BOOL OFF FORCE)
set(RECASTNAVIGATION_STATIC
    ON
    CACHE BOOL ON FORCE)
set(RECASTNAVIGATION_TESTS
    OFF
    CACHE BOOL OFF FORCE)
pr_include_third_party_library(recastnavigation TARGET Recast INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/recastnavigation/Recast/Include")
pr_include_third_party_library(TARGET Detour INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/recastnavigation/Detour/Include")
pr_set_target_folder(DebugUtils third_party_libs/recast)
pr_set_target_folder(Detour third_party_libs/recast)
pr_set_target_folder(DetourCrowd third_party_libs/recast)
pr_set_target_folder(DetourTileCache third_party_libs/recast)
pr_set_target_folder(Recast third_party_libs/recast)

pr_find_library(recast NAMES "Recast" PATHS "third_party_libs/recastnavigation/Recast/${BINARY_PRAGMA_DIR}")
pr_find_library(detour NAMES "Detour" PATHS "third_party_libs/recastnavigation/Detour/${BINARY_PRAGMA_DIR}")
#

# rectangle_bin_pack
pr_include_third_party_library(rectangle_bin_pack_cmake TARGET rectangle_bin_pack INC "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/rectangle_bin_pack")
pr_set_target_folder(rectangle_bin_pack third_party_libs)
#

# RapidXml
pr_set_include_path(rapidxml "${CMAKE_CURRENT_LIST_DIR}/third_party_libs/rapidxml")
#

# In Linux there is a cyclic deps between freetype,harfbuzz,pango,cairo and most importantly fontconfig. Fontconfig in linux is reposnsible for discovery of
# fonts. (think C:\Windows\Fonts for linux but customizable via configs) recently I hit a snag in which harfbuzz failed to load due to missing pango symbols.
# The AppImage team hit similar snag too, see https://github.com/AppImageCommunity/pkg2appimage/pull/323 and
# https://github.com/probonopd/linuxdeployqt/issues/261 Even if I would include pango and cairo into our pipeline, CEF would complain (coincidentally hitting
# AppImage's snag too), since that DOES use system fonts. (fontconfig has persistent presence in all graphical managers in linux)
if(WIN32)
    add_library(freetype SHARED IMPORTED)
    set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB "${DEPENDENCY_FREETYPE_LIBRARY}") # pragma_install_lib should pick up the dll file to install, since I
                                                                                            # did not install a target here

    target_include_directories(freetype INTERFACE ${DEPENDENCY_FREETYPE_INCLUDE})
endif()
# in linux the check is done in wgui

if(WIN32)
    set_target_properties(freetype PROPERTIES FOLDER third_party_libs)
endif()
pr_find_library(rectanglebinpack NAMES "rectangle_bin_pack" PATHS "third_party_libs/rectangle_bin_pack/${BINARY_PRAGMA_DIR}")
# target_include_directories(freetype PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/third_party_libs/zlib)
#

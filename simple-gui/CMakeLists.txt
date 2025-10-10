project(SimpleGui)

set(THIRD_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdlib)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# 创建静态库目标
add_library(SimpleGui STATIC ${SOURCES})

# 设置包含目录
target_include_directories(SimpleGui
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/SDL3/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/SDL3_ttf/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/SDL3_image/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/utfcpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/utfcpp/utf8>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置编译属性
target_compile_features(SimpleGui PUBLIC cxx_std_20)


# 安装配置
install(TARGETS SimpleGui
        EXPORT SimpleGuiTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# 安装头文件
install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)

# 导出目标供其他项目使用
install(EXPORT SimpleGuiTargets
        FILE SimpleGuiTargets.cmake
        NAMESPACE SimpleGui::
        DESTINATION lib/cmake/SimpleGui
)

# 创建别名目标
add_library(SimpleGui::SimpleGui ALIAS SimpleGui)

# 项目特定的编译选项
target_compile_definitions(SimpleGui PRIVATE
        LIBRARY_A_BUILD
)

# 依赖项（如果需要）
# find_package(SomeDependency REQUIRED)
# target_link_libraries(LibraryA PUBLIC SomeDependency::SomeDependency)

target_link_libraries(SimpleGui PUBLIC
        ${THIRD_LIB_DIR}/SDL3/lib/x64/SDL3.lib
        ${THIRD_LIB_DIR}/SDL3_ttf/lib/x64/SDL3_ttf.lib
        ${THIRD_LIB_DIR}/SDL3_image/lib/x64/SDL3_image.lib
)


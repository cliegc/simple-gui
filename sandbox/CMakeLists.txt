project(SandBox)

set(THIRD_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdlib)

# 创建可执行文件
add_executable(SandBox
        src/main.cpp
)

# 设置包含目录（如果LibraryA的PUBLIC包含目录不够）
target_include_directories(SandBox
        PRIVATE
        ${PROJECT_SOURCE_DIR}/simple-gui/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${THIRD_LIB_DIR}/SDL3/include
        ${THIRD_LIB_DIR}/SDL3_ttf/include
        ${THIRD_LIB_DIR}/SDL3_image/include
)


# 链接静态库LibraryA
target_link_libraries(SandBox
        PRIVATE
        SimpleGui::SimpleGui
        ${THIRD_LIB_DIR}/SDL3/lib/x64/SDL3.lib
        ${THIRD_LIB_DIR}/SDL3_ttf/lib/x64/SDL3_ttf.lib
        ${THIRD_LIB_DIR}/SDL3_image/lib/x64/SDL3_image.lib
)

target_compile_features(SandBox PUBLIC cxx_std_20)

# 设置可执行文件属性
set_target_properties(SandBox PROPERTIES
        OUTPUT_NAME "sandbox"
        WIN32_EXECUTABLE FALSE
)

# 安装可执行文件
install(TARGETS SandBox
        RUNTIME DESTINATION bin
)

# 项目特定的编译定义
target_compile_definitions(SandBox PRIVATE
        APP_NAME=\"SandBox\"
)


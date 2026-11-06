
cmake_minimum_required(VERSION 3.20)

project(Base64Library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Створення динамічної бібліотеки
add_library(Base64Library SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Base64Library_Lab4/src/base64.cpp"
)

# Налаштування експорту функцій
set_target_properties(Base64Library PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

target_compile_definitions(Base64Library PRIVATE BASE64_EXPORTS)

# Підключення заголовків
target_include_directories(Base64Library PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Base64Library_Lab4/include"
)

# Тестова програма
add_executable(Base64Test
    "${CMAKE_CURRENT_SOURCE_DIR}/Base64Library_Lab4/test/main.cpp"
)

target_link_libraries(Base64Test PRIVATE Base64Library)

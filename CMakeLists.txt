cmake_minimum_required(VERSION 3.14)
project(lab2 VERSION 0.1.0 LANGUAGES C CXX)

# Установка стандарта C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное приложение
add_executable(lab2 
    main.cpp 
    ip_filter.cpp
)

# Тесты
enable_testing()

# Более надежная настройка Google Test
include(FetchContent)

# Используем стабильную версию GTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1  # Стабильная версия
)

# Настройка политик для избежания предупреждений
cmake_policy(SET CMP0135 NEW)  # для DOWNLOAD_EXTRACT_TIMESTAMP
cmake_policy(SET CMP0169 NEW)  # для современного FetchContent

FetchContent_MakeAvailable(googletest)

# Сначала создаем исполняемый файл
add_executable(test_ip test_ip.cpp ip_filter.cpp)

# Затем линкуем
target_link_libraries(test_ip PRIVATE gtest gtest_main)

# Добавляем тесты в систему тестирования CMake
add_test(NAME IPFilterTest COMMAND test_ip)

# Копируем файл с данными в директорию сборки для тестов
configure_file(${CMAKE_SOURCE_DIR}/ip_filter.tsv ${CMAKE_BINARY_DIR}/ip_filter.tsv COPYONLY)
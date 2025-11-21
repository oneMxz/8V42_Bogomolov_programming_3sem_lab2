cmake_minimum_required(VERSION 3.14)
project(lab2 VERSION 0.1.0 LANGUAGES C CXX)

# Установка стандарта 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное приложение
add_executable(lab2 
    main.cpp 
    ip_filter.cpp
)

# Тесты
enable_testing()
include(FetchContent)

# Используем стабильную версию GTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# Настройка политик для избежания предупреждений
cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0169 NEW)

FetchContent_MakeAvailable(googletest)

# Тестовый исполняемый файл
add_executable(test_ip test_ip.cpp ip_filter.cpp)
target_link_libraries(test_ip PRIVATE gtest gtest_main)

# Добавляем тесты в систему тестирования CMake
add_test(NAME IPFilterTest COMMAND test_ip)

# Копируем файл с данными в директорию сборки для тестов
configure_file(${CMAKE_SOURCE_DIR}/ip_filter.tsv ${CMAKE_BINARY_DIR}/ip_filter.tsv COPYONLY)

# Настройка CPack для создания пакетов
set(CPACK_PACKAGE_NAME "lab2")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})  
set(CPACK_PACKAGE_VENDOR "Bogomolov")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "IP Filter Application")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bogomolov")
endif()

# Установка целевых файлов
install(TARGETS lab2 DESTINATION bin)
install(TARGETS test_ip DESTINATION bin)

# Включаем CPack
include(CPack)
cmake_minimum_required(VERSION 3.5)

project(InventoryAppGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Temukan paket Qt. Tambahkan 'Sql' ke daftar COMPONENTS.
# Ini akan menemukan modul Qt SQL.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES InventoryAppGUI_id_ID.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/inventory/ItemInventaris.cpp
        src/utils/Utils.cpp
        src/database/DatabaseManager.cpp
        # Tambahkan header file ke PROJECT_SOURCES jika tidak otomatis diproses oleh CMake/Moc
        src/inventory/ItemInventaris.h
        src/utils/Utils.h
        src/database/DatabaseManager.h
        # ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(InventoryAppGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET InventoryAppGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(InventoryAppGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(InventoryAppGUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# --- BAGIAN KRUSIAL YANG DIUBAH/DITAMBAHKAN ---

# Tambahkan direktori include kustom untuk kode backend kamu.
# Ini memungkinkan file .cpp di folder src/database/ misalnya, untuk menemukan ItemInventaris.h
# tanpa path relatif yang rumit.
target_include_directories(InventoryAppGUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/inventory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database
    # Opsional: Jika libpqxx atau header PostgreSQL tidak ditemukan otomatis,
    # tambahkan jalur include di sini, contoh:
    #/usr/include/postgresql/pqxx
    #/usr/local/include
)

# Tautkan (link) library ke executable
target_link_libraries(InventoryAppGUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets # Library Widgets untuk GUI
    Qt${QT_VERSION_MAJOR}::Sql    # Library SQL untuk fungsionalitas database Qt

    # Library libpqxx dan libpq untuk PostgreSQL.
    # Ada beberapa cara untuk menautkannya:
    # 1. Dengan nama library dasar (CMake mencoba menemukannya di jalur standar)
    pqxx # Library C++ libpqxx
    pq   # Library C dasar PostgreSQL (libpq)

    # 2. Atau, jika cara di atas tidak berhasil, tentukan jalur eksplisit dengan -L dan -l
    #    Contoh untuk Linux:
    #    -L/usr/lib/x86_64-linux-gnu -lpqxx -lpq
    #    Contoh untuk macOS (Homebrew):
    #    -L/usr/local/lib -lpqxx -lpq

    # Pilih salah satu metode di atas. Jika kamu menggunakan distribusi Linux yang standar
    # dan libpqxx diinstal melalui package manager, metode 'pqxx' dan 'pq' biasanya berhasil.
)

# --- AKHIR BAGIAN KRUSIAL ---


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.InventoryAppGUI)
endif()
set_target_properties(InventoryAppGUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS InventoryAppGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(InventoryAppGUI)
endif()
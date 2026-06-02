cmake_minimum_required(VERSION 3.20)
project(RobEngine VERSION 0.010)
#x => Release version
#x.x => Major update 
#x.xx => Minor update 
#x.xxx => Fix 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# ─── Sources du moteur ───────────────────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "src/**/*.cpp"
    "include/*.h"
    "include/**/*.h"
)



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

# ─── Bibliothèque du moteur ──────────────────────────────────────
add_library(RobEngine STATIC ${ENGINE_SOURCES} 
"include/core/GameState.h" 
"src/core/GameState.cpp" 
"include/Utils/FirstPersonCtrl.h"
"src/Utils/FirstPersonCtrl.cpp")

# Les headers sont dans include/, on les expose publiquement
# PUBLIC = disponibles pour RobEngine ET pour quiconque linke RobEngine
target_include_directories(RobEngine 
PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include)

# ─── SFML ────────────────────────────────────────────────────────
# On cherche SFML dans le dossier voisin SFML-3.0.2
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../SFML-3.0.2/lib/cmake/SFML")
set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# PRIVATE = SFML est un détail d'implémentation du moteur,
# le projet consommateur n'a pas besoin de le gérer lui-même
target_link_libraries(RobEngine PUBLIC SFML::Graphics SFML::Window SFML::System)

# ─── Exécutable de test (ton main.cpp actuel) ────────────────────
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(RobEngineApp src/main.cpp
        "include/core/GameState.h"
        "src/core/GameState.cpp"
        "include/Utils/FirstPersonCtrl.h"
        "src/Utils/FirstPersonCtrl.cpp"
    )
    target_link_libraries(RobEngineApp PRIVATE RobEngine)

    add_custom_command(TARGET RobEngineApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_DIR}/../../../bin"
        $<TARGET_FILE_DIR:RobEngineApp>)

    add_custom_command(TARGET RobEngineApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:RobEngineApp>/assets")
endif()

# ─── Installation & packaging ────────────────────────────────────
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Installe la lib et les headers
install(TARGETS RobEngine
    EXPORT RobEngineTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Génère RobEngineTargets.cmake (décrit la target importée)
install(EXPORT RobEngineTargets
    FILE RobEngineTargets.cmake
    NAMESPACE RobEngine::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RobEngine
)

# Génère RobEngineConfigVersion.cmake
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RobEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Copie ton RobEngineConfig.cmake + le fichier de version
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/RobEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RobEngineConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RobEngine
)
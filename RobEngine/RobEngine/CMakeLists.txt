cmake_minimum_required(VERSION 3.20)
project(RobEngine VERSION 0.010)
#x => Release version
#x.x => Major update 
#x.xx => Minor update 
#x.xxx => Fix 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Sources du moteur ───────────────────────────────────────────
# Ajoute ici chaque .cpp que tu crées (sauf main.cpp !)
set(ENGINE_SOURCES
    src/renderer/Renderer.cpp
    # src/mathB/Matrice.cpp   ← décommente quand tu les crées
    # src/mathB/Vector.cpp
    # src/core/...
)

# ─── Bibliothèque du moteur ──────────────────────────────────────
# STATIC = tout est embarqué dans un .lib, plus simple pour débuter
add_library(RobEngine STATIC ${ENGINE_SOURCES})

# Les headers sont dans include/, on les expose publiquement
# PUBLIC = disponibles pour RobEngine ET pour quiconque linke RobEngine
target_include_directories(RobEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ─── SFML ────────────────────────────────────────────────────────
# On cherche SFML dans le dossier voisin SFML-3.0.2
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../SFML-3.0.2/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# PRIVATE = SFML est un détail d'implémentation du moteur,
# le projet consommateur n'a pas besoin de le gérer lui-même
target_link_libraries(RobEngine PRIVATE SFML::Graphics SFML::Window SFML::System)

# ─── Exécutable de test (ton main.cpp actuel) ────────────────────
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(RobEngineApp src/main.cpp)
    target_link_libraries(RobEngineApp PRIVATE RobEngine)
endif()

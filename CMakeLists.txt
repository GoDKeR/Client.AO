# CMakeList.txt: proyecto de CMake para Client.AO, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.12)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Client.AO")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
#set(CMAKE_CXX_EXTENSIONS Off)

add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX -DWIN32_LEAN_AND_MEAN)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
set(INCLUDE_FILES  ${PROJECT_SOURCE_DIR}/include)

add_executable (Client.AO WIN32 "Client.AO.cpp" ${SOURCE_FILES})

target_include_directories(Client.AO PUBLIC "${CMAKE_SOURCE_DIR}/src" ${INCLUDE_FILES})

target_link_libraries(Client.AO PRIVATE d3d11.lib d3dcompiler.lib dxgi.lib Ws2_32.lib) #msimg32.lib 

# TODO: Agregue pruebas y destinos de instalación si es necesario.

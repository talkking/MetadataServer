INCLUDE_DIRECTORIES(
../../include
)

add_library(communication communication.cpp assist.cpp)

add_library(filesystem metadata_server.cpp metadata_system.cpp file_system.cpp file_structure.cpp)
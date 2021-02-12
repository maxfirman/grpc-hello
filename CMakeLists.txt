# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.1)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -static-libstdc++ -static-libgcc")

project ("RGRPC")

find_package(gRPC CONFIG REQUIRED)
find_package( Protobuf REQUIRED )
find_package(Threads REQUIRED)
#find_package(re2 CONFIG REQUIRED)


message(STATUS "CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")

add_library(client STATIC src/client.cpp proto/data.grpc.pb.cc proto/data.pb.cc)
target_link_libraries( client PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite Threads::Threads)

add_executable( server src/server.cpp proto/data.grpc.pb.cc proto/data.pb.cc)
target_link_libraries( server PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_executable( client-bin src/client.cpp proto/data.grpc.pb.cc proto/data.pb.cc)
target_link_libraries( client-bin PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
set_target_properties(client-bin PROPERTIES OUTPUT_NAME client)

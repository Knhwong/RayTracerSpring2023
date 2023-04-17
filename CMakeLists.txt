cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project("Ray Tracer" C CXX)
find_package( OpenCV REQUIRED )
include_directories(
parsers
math
${OpenCV_INCLUDE_DIRS}
)


file (GLOB rapidjson_headers
parsers/rapidjson/*.h
)

file (GLOB source
main/*.cpp
core/*.cpp
cameras/*.cpp
lights/*.cpp
shapes/*.cpp
materials/*.cpp
parsers/fast_obj.h
)


#raytracer executable
add_executable(raytracer ${source} math/geometry.h ${rapidjson_headers})
target_link_libraries( raytracer ${OpenCV_LIBS} )

#json example executable
add_executable(jsonexample examples/jsonExample.cpp ${rapidjson_headers})

#vector example executable
add_executable(vectorexample examples/vecMatrixExample.cpp math/geometry.h)
target_link_libraries( vectorexample ${OpenCV_LIBS} )
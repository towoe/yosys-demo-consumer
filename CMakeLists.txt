cmake_minimum_required(VERSION 4.4)
project(yosys_consumer_example LANGUAGES CXX)

# All that is required to make this project use Yosys
find_package(Yosys REQUIRED)

# Project
add_executable(demo main.cc)
# Link yosys lib
target_link_libraries(demo PRIVATE Yosys::libyosys)

# Adapt Yosys path lookups
target_compile_definitions(demo PRIVATE
	YOSYS_DATADIR="${YOSYS_DATADIR}"
	YOSYS_ABC_EXECUTABLE="${YOSYS_ABC_EXECUTABLE}"
)

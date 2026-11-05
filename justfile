build: build-yosys build-demo

run: run-demo

get-yosys:
	git submodule update --init --recursive

build-yosys:
	cmake -B build-yosys yosys -DYOSYS_INSTALL_LIBRARY=ON
	cmake --build build-yosys --target libyosys yosys-abc -j 6

build-demo:
	cmake -B build-demo . -DYosys_ROOT=build-yosys
	cmake --build build-demo

run-demo:
	./build-demo/demo

clean:
	rm -r build-yosys build-demo

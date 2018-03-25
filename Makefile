CXX := Main.cpp -std=c++11 -stdlib=libc++
O3 := -O3
WASM := -s DEMANGLE_SUPPORT=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "FS_createDataFile", "FS_readFile", "FS_unlink"]' -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s EXPORTED_FUNCTIONS='["_simplify"]'

all:
	g++ ${CXX}
release:
	g++ ${CXX} ${O3}
wasm:
	em++ ${CXX} ${WASM}
wasm_release:
	em++ ${CXX} ${WASM} ${O3}

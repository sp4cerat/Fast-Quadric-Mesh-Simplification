CXX := Main.cpp
O3 := -O3
#WASM := -s DEMANGLE_SUPPORT=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "FS_createDataFile", "FS_readFile", "FS_unlink"]' -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s EXPORTED_FUNCTIONS='["_simplify"]'
WASM := -s DEMANGLE_SUPPORT=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "FS_createDataFile", "FS_readFile", "FS_unlink"]' -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s EXPORTED_FUNCTIONS='["_simplify"]'


all: release

release:
	g++ ${CXX} ${O3} -o simplify

debug:
	g++ ${CXX}

sanitize:
	g++ ${CXX} -O1 -g -fsanitize=address -fno-omit-frame-pointer  -o simplify
#make wasm
wasm:
	em++ ${CXX} ${WASM} ${O3}
wasm_debug:
	em++ ${CXX} ${WASM}

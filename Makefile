all: test.wasm test.dylib glue.dylib Makefile
	env DYLD_INSERT_LIBRARIES=$$(pwd)/glue.dylib lucet-wasi ./test.dylib

test.wasm: test.go Makefile
	tinygo build -panic=trap -no-debug -wasm-abi generic -o test.wasm -target wasm test.go

test.dylib: test.wasm Makefile
	lucetc-wasi -o test.dylib --bindings jsbindings.json test.wasm

glue.dylib:	glue.c Makefile
	cc -O2 -fPIC glue.c -shared -o glue.dylib -lcurl


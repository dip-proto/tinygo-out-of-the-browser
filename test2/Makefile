all: test.wasm test.dylib Makefile
	lucet-wasi ./test.dylib

test.wasm: test.go Makefile
	tinygo build -no-debug -wasm-abi generic -o test.wasm -target wasm test.go

test.dylib: test.wasm Makefile
	lucetc-wasi -o test.dylib test.wasm

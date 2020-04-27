all: test.wasm
	wasm3 --func add test.wasm 1 4

test.wasm: test.go Makefile
	tinygo build -panic=trap -no-debug -wasm-abi generic -o test.wasm -target wasm test.go


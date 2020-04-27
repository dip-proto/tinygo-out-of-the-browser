package main

import (
	"fmt"
)

func gg1() {
	body := make([]uint8, 64)
	handle := HttpGet("https://fastly.com/")
	for len := HttpBodyChunk(handle, body); len > 0; len = HttpBodyChunk(handle, body) {
		fmt.Println(string(body[:len]))
	}
}

func gg2() {
	body := make([]uint8, 32)
	handle := HttpGet("https://www.google.com/")
	for len := HttpBodyChunk(handle, body); len > 0; len = HttpBodyChunk(handle, body) {
		fmt.Println(string(body[:len]))
	}
}

func main() {
	go gg1()
	go gg2()
}

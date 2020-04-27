package main

import (
	"fmt"
)

func main() {
	body := make([]uint8, 4096)
	handle := HttpGet("https://fastly.com/")
	for len := HttpBodyChunk(handle, body); len > 0; len = HttpBodyChunk(handle, body) {
		fmt.Println(string(body[:len]))
	}
}

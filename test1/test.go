package main

//go:export add
func add(a, b int) int {
	return a + b
}

func main() {}

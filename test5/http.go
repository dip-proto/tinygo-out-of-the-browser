package main

//go:export http::get
func HttpGet(uri string) int64

//go:export http::body_chunk
func HttpBodyChunk(handle int64, buf []uint8) int64

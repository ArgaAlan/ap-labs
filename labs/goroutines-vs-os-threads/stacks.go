package main

import (
	"fmt"
	"os"
	"runtime"
	"time"
)

func main() {
	in := make(chan int)
	out := make(chan int)
	var m runtime.MemStats

	file, err := os.Create("stacks-report.txt")
	checkError(err)
	defer file.Close()

	runtime.ReadMemStats(&m)
	_, err1 := file.WriteString(fmt.Sprintf("Before pipe creation: \nSys = %v MiB \n", bToMb(m.Sys)))
	checkError(err1)

	go connect(in, out, 0, 1000000)

	start := time.Now()
	in <- 1

	runtime.ReadMemStats(&m)
	_, err2 := file.WriteString(fmt.Sprintf("After pipe creation: \nSys = %v MiB \n", bToMb(m.Sys)))
	checkError(err2)

	_, err3 := file.WriteString(fmt.Sprintf("Time to get in all pipes: %s\n", time.Since(start)))
	checkError(err3)
	_, err4 := file.WriteString(fmt.Sprintf("The pipe creation is created in 1000000 so you can se a high impact in time and memory, if you do 9999999 pipelines you will get an SO\n"))
	checkError(err4)
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}

func connect(in <-chan int, out chan int, index int, connections int) {
	if index != connections {
		connect(in, out, index+1, connections)
		out <- <-in
	}
}

//https://golangcode.com/print-the-current-memory-usage/
func PrintMemUsage() {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	fmt.Sprintf("Sys = %v MiB \n", bToMb(m.Sys))

}

//https://golangcode.com/print-the-current-memory-usage/
func bToMb(b uint64) uint64 {
	return b / 1024 / 1024
}


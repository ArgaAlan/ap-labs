package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	in := make(chan int)
	out := make(chan int)
	total := 0

	file, err := os.Create("scheduling-report.txt")
	checkError(err)
	defer file.Close()

	go connect(in, out)
	go connect(out, in)

	for i := 0; i < 1; i++ {
		in <- 0
		time.Sleep(time.Duration(1) * time.Second)
		x := <-in
		total += x
	}

	_, err1 := file.WriteString(fmt.Sprintf("Ping pong messages per second: %d", total))
	checkError(err1)
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}

func connect(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}


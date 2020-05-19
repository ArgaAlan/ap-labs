// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"strconv"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

type web struct {
	url   string
	depth int
}

func crawl(url web) []web {
	fmt.Println(url.url)
	if url.depth > 0 {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(url.url)
		sites := make([]web, 0)
		for _, n := range list {
			sites = append(sites, web{url: n, depth: url.depth - 1})
		}
		<-tokens // release the token

		if err != nil {
			log.Print(err)
		}
		return sites
	}
	sites := make([]web, 0)
	return sites
}

func crawlDepth(url string, depth int) {

}

//!-sema

//!+
func main() {
	worklist := make(chan []web)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	input := make([]web, 0)
	s := os.Args[1]
	depthStr := s[len(s)-1:]
	depth, err := strconv.Atoi(depthStr)
	if err != nil {
		panic("Not correct arguments in depth input.")
	}
	input = append(input, web{url: os.Args[2], depth: depth})
	go func() { worklist <- input }()
	// Crawl the web concurrently.
	seen := make(map[web]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				n++
				go func(link web) {
					worklist <- crawl(link)
				}(link)
			}
		}
	}
}

//!-


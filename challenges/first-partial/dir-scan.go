package main

import (
	"fmt"
	"os"
        "path/filepath"
)

func walkFn(root string,info os.FileInfo, e error) error{
    if e != nil{ //Si el error no es diferente de null hubo un problema con el path y se debe eliminar este walk
        fmt.Printf("WRONG PATH")
        return e
    }
    
    switch mode := info.Mode(); true{
	case mode.IsDir():
		dir++
	case mode&os.ModeSymlink != 0:
		sysL++
	case mode&os.ModeDevice != 0:
		dev++
	case mode&os.ModeSocket != 0:
		sock++
	default:
		others++
	}
	
    return nil
}

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	e := filepath.Walk(dir, walkFn)
        if e != nil {
            return e
        }
        return nil
}
var dir, sysL, dev, sock, others int
func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
        e := scanDir(os.Args[1]);
	if e!=nil {
          fmt.Println(e);
          os.Exit(1);
        }

        fmt.Println("Directory Scanning Tool")
	fmt.Println("+-----------------+----------+")
	fmt.Printf("| Path            | %-9s|\n", os.Args[1])
	fmt.Println("+-----------------+----------+")
	fmt.Printf("| Directories     | %-9d|\n", dir)
	fmt.Printf("| Symbolic Links  | %-9d|\n", sysL)
	fmt.Printf("| Devices         | %-9d|\n", dev)
	fmt.Printf("| Sockets         | %-9d|\n", sock)
	fmt.Printf("| Other files     | %-9d|\n", others)
	fmt.Println("+-----------------+----------+")
}

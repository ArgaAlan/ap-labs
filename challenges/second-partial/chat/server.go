// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering      = make(chan client)
	leaving       = make(chan client)
	messages      = make(chan string) // all incoming client messages
	clients       = make(map[client]bool)
	clientList    = make(map[string]bool)
	clientChannel = make(map[string]chan string)
	clientConn    = make(map[string]net.Conn)
	admin         = ""
)

func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	server := "irc-server > "

	user := make([]byte, 100)
	conn.Read(user)
	username := ""
	for i := 0; i < len(user); i++ {
		if user[i] != 0 {
			username = username + string(user[i])
		} else {
			break
		}
	}
	if val, ok := clientList[username]; ok && val {
		ch <- server + "User already connected"
		ch <- ""
		conn.Close()
	} else {
		ch <- server + "Welcome to the Simple IRC Server"
		clientList[username] = true
		clientChannel[username] = ch
		clientConn[username] = conn

		ch <- server + "Your user [" + username + "] is succesfully logged"
		if len(clientList) == 1 {
			ch <- server + "Congrats, you were the first user."
		}
		if admin == "" {
			admin = username
			ch <- server + "You're the new IRC Server Admin"
			fmt.Printf(server + "[" + username + "] was promoted as the channel ADMIN\n")
		}

		messages <- server + "New connected user [" + username + "]"
		fmt.Printf(server + "New connected user [" + username + "]\n")
		entering <- ch
		input := bufio.NewScanner(conn)
		for input.Scan() {
			commands := strings.Split(input.Text(), " ")
			if "/users" == input.Text() {
				usersList := server
				for i := range clientList {
					usersList = usersList + i + ", "
				}
				ch <- usersList[:len(usersList)-2]
			} else if "/time" == input.Text() {
				local, err := time.LoadLocation("Local")
				if err != nil {
					log.Fatal("Error getting TZ")
				}
				if local.String() == "Local" {
					ch <- server + "Local Time: America/Mexico_City" + " " + time.Now().Format("15:04") //strconv.Itoa(time.Now().Hour())
				} else {
					ch <- server + "Local Time: " + local.String() + " " + time.Now().Format("15:04") //strconv.Itoa(time.Now().Hour())
				}

			} else if "" == input.Text() {
				ch <- server + "Please type at least one character"
			} else {
				if len(commands) > 0 {
					if commands[0] == "/msg" {

						if len(commands) > 1 {
							if val, ok := clientList[commands[1]]; ok && val {
								message := ""
								for i := 2; i < len(commands); i++ {
									message = message + commands[i] + " "
								}
								clientChannel[commands[1]] <- "Direct message from [" + username + "] : " + message
							} else {
								ch <- server + "No such user or user disconnected"
							}
						} else {
							ch <- server + "Wrong command use"
						}

					} else if commands[0] == "/user" {

						if len(commands) > 1 {
							if val, ok := clientList[commands[1]]; ok && val {
								userDataName := commands[1]
								userDataIP := clientConn[commands[1]].RemoteAddr().String()
								ch <- server + "username: " + userDataName + ", IP:" + userDataIP
							} else {
								ch <- server + "No such user or user disconnected"
							}
						} else {
							ch <- server + "Wrong command use"
						}

					} else if commands[0] == "/kick" {

						if len(commands) > 1 {
							if username == admin {
								if val, ok := clientList[commands[1]]; ok && val {
									clientChannel[commands[1]] <- server + "You're kicked from this channel"
									clientChannel[commands[1]] <- ""
									clientConn[commands[1]].Close()
									messages <- server + "[" + commands[1] + "] was kicked from this channel"
									fmt.Printf(server + "[" + commands[1] + "] was kicked\n")
								} else {
									ch <- server + "No such user or user disconnected"
								}
							} else {
								ch <- server + "Commnad only allowed to admins"
							}
						} else {
							ch <- server + "Wrong command use"
						}

					} else if string(commands[0][0]) == "/" {
						ch <- server + "No such command"
					} else {
						messages <- username + " > " + input.Text()
					}
				} else {
					messages <- username + " > " + input.Text()
				}
			}
		}
		// NOTE: ignoring potential errors from input.Err()

		leaving <- ch
		messages <- username + " has left"
		fmt.Printf(server + "[" + username + "] left\n")
		conn.Close()
		clientList[username] = false
		if admin == username {
			for i := range clientList {
				if clientList[i] == true {
					admin = i
					clientChannel[i] <- server + "You're the new IRC Server Admin"
				}
			}
		}
	}

}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	if len(os.Args) != 5 {
		log.Fatal("Bad arguments")
	} else {
		fmt.Printf("irc-server > Simple IRC Server started at " + os.Args[2] + ":" + os.Args[4] + "\n")
		fmt.Printf("irc-server > Ready for receiving new clients\n")
		listener, err := net.Listen("tcp", os.Args[2]+":"+os.Args[4])
		if err != nil {
			log.Fatal(err)
		}

		go broadcaster()
		for {
			conn, err := listener.Accept()
			if err != nil {
				log.Print(err)
				continue
			}
			go handleConn(conn)
		}
	}
}

//!-main


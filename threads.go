package main

import "fmt"

// This function runs a for loop that prints its character
// while it receives OKs from the main goroutine.
// Returns a channel which it listens for events on.
func printChar(toPrint string) chan bool {
	listener := make(chan bool)
	go func() {
		ok := <-listener
		for ok {
			// Print string and wait for signals.
			fmt.Print(toPrint + " ")
			ok = <-listener
		}
	}()
	return listener
}

func main() {
	turn := 0

	// Create three goroutines with a different character to print each.
	a := printChar("a")
	b := printChar("b")
	c := printChar("c")
	// Run 1000 times.
	for i := 0; i < 1000; i++ {
		// Alternate between goroutines.
		switch turn {
		case 0:
			a <- true
			break
		case 1:
			b <- true
			break
		case 2:
			c <- true
			break
		}
		turn = (turn + 1) % 3
	}
	fmt.Println()
}

# Ex3

Ex3 is a program that would interpret text code provided, and control the flight simulator 'flightgear' accordingly.

## Running

Use the following command to compile, then run a.out (provide txt file name as argument) :

```bash
g++ -std=c++14 main.cpp ex1.cpp ex1.h Expression.h  -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```

## Usage
You can use the following commands in the txt file :

```cpp
//connect sim as client:
openDataServer(5400)
//connect to sim:
connectControlClient("127.0.0.1",5402)
//print:
Print("Hello")
//sleep:
Sleep(250)
//var:
var throttle -> sim(/path)
var throttle <- sim(/path)
var throttle == aileron
//if:
if throttle > 0.2 {
     h0 = 1
}
//while:
while throttle > 0.2 {
     h0 = h0+1
}
//set value (to existing var):
throttle = 1
```

## GIT
Link : 
```bash
https://github.com/guytiklu/Ex3
```

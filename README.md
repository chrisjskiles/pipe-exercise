# pipe-exercise
A short pipe exercise utilizing Unix system calls in C++
\\This process forks twice resulting in a parent, child, and grandchild process. A pipe is then opened between each of them. In turn, each process receives an integer, M, and applies a formula to it (2 \* M + 3) then identifies itself and displays the new integer, and finally passes it through the pipe to the next process. 

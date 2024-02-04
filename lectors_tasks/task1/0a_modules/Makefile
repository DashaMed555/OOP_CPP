solution: main.o
	/usr/local/Cellar/llvm/17.0.2/bin/clang++ -std=c++20 -fmodules main.o *.pcm -o solution

main.o: main.cpp module1.pcm module2.pcm module3.pcm
	/usr/local/Cellar/llvm/17.0.2/bin/clang++ -std=c++20 -fmodules -c -fprebuilt-module-path=. main.cpp -o main.o

module1.pcm: module1.cpp
	/usr/local/Cellar/llvm/17.0.2/bin/clang++ -std=c++20 -c -Xclang -emit-module-interface module1.cpp -o module1.pcm

module2.pcm: module2.cpp
	/usr/local/Cellar/llvm/17.0.2/bin/clang++ -std=c++20 -c -Xclang -emit-module-interface module2.cpp -o module2.pcm

module3.pcm: module3.cpp
	/usr/local/Cellar/llvm/17.0.2/bin/clang++ -std=c++20 -c -Xclang -emit-module-interface module3.cpp -o module3.pcm

clean:
	rm *.o *.pcm solution

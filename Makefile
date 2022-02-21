
COMPILE = clang++
FLAGS = -std=c++11 -Wall -O

timing : words.hpp words.cpp timing.cpp
	${COMPILE} ${FLAGS} words.cpp timing.cpp -o timing

grade : timing
	./timing vector moby_dick.txt
	./timing list moby_dick.txt
	./timing hash moby_dick.txt
	./timing tree moby_dick.txt
	echo "== words.cpp =="
	cat words.cpp

clean:
		rm -f timing

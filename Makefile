CC=gcc -g
FLAGS = -Wall -Werror -Wextra -std=c11
FLAGS_TESTS = -lcheck -lpthread -fprofile-arcs -ftest-coverage
DIR = Backend/
FUNC = s21_parser.c s21_rotation.c
FUNC_C = $(addprefix $(DIR), $(FUNC))
TESTS_C = tests/tests.c
VAR = $(shell pkg-config --cflags --libs check)


all: install tests

install: install_first open_3DV

install_first:
	mkdir build
	cd build/; qmake ../3D/3D.pro; make

open_3DV:
	chmod 777 ./build/3D.app
	open ./build/3D.app

uninstall:
	rm -rf build

dvi:
#	doxygen
	open ./html/index.html

dist:
	cd ..; tar -cvf archive.tar src

tests: clean
	$(CC) $(FLAGS) $(FUNC_C) $(TESTS_C) $(FLAGS_TESTS) -o test
	./test

gcov_report: tests
	gcov $(TESTS_C)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report
	open report/index.html

clean:
	rm -rf *.o report *.info *.gcda *.gcno *.gcov *.gch *.out *.a *.dSYM test
	rm -rf build-3D*

main:
	$(CC) $(FLAGS) $(DIR)main.c $(FUNC_C) 
	./a.out

valgrind:
	valgrind --leak-check=full -s --track-origins=yes --log-file=valgrind-out.txt ./test

style:
	cp ../materials/linters/.clang-format ./
#	clang-format -i $(FUNC_C) $(TESTS_C) Backend/s21_3DViewer.h 3D/*.cpp 3D/*.h
	clang-format -n $(FUNC_C) $(TESTS_C) Backend/s21_3DViewer.h 3D/*.cpp 3D/*.h



NAME = s21_cat
CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -g -std=c11
CAT=s21_cat.c
COMM=-o s21_cat

.PHONY: s21_cat rebuild clean all

all: $(NAME)

s21_cat:
	$(CC) $(CFLAGS) $(CAT) $(COMM)

clean: 
	rm -rf s21_cat
	rm -rf *.log
	rm -rf *.o s21_cat a.out
	rm -rf *.o *_exe *.dSYM *.out ./.clang-format *.DS_Store


test: s21_cat
	sh test_func_cat.sh

rebuild: clean all

leak: s21_cat
	leaks --atExit -- ./s21_cat -e int s21_cat.c -benst test_file/all_flag_tests/*

check: s21_cat
	cppcheck --enable=all --suppress=missingIncludeSystem . *.c *.h
	cp ../../materials/linters/CPPLINT.cfg ./
	python3 ./../../materials/linters/cpplint.py --extensions=c *.c *.h
	rm -f CPPLINT.cfg

test_valgrind:
	sh ../materials/build/run.sh

clang:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
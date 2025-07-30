CC = gcc
FLAG_C = -c
FLAG_O = -o 
FLAG_ER = -Wall -Werror -Wextra -std=c11
FLAG_TESTS = -lcheck -lm -lsubunit -lpthread
s21_STRING_C = s21_*.c 
s21_STRING_O = s21_*.o

all: clean s21_string.a

s21_string.a:
	$(CC) $(FLAG_C) $(FLAG_ER) $(s21_STRING_C) 
	ar rc s21_string.a $(s21_STRING_O)
	ranlib s21_string.a

clean:
	-rm -rf *.o s21_string.a program* report

main:
	$(CC) main.c s21_string.a -o program

test: clean
	$(CC) $(FLAG_C) $(FLAG_ER) $(s21_STRING_C) --coverage
	ar rc s21_string.a $(s21_STRING_O)
	ranlib s21_string.a
	$(CC) $(FLAG_ER) --coverage $(s21_STRING_C) test.c $(FLAG_TESTS) -o program
	./program

gcov_report: test
	mkdir -p report/coverage_data
	lcov  --directory . --capture --output-file coverage.info
	lcov --remove coverage.info "test.c" "s21_strcpy.c" -o coverage.info
	genhtml coverage.info --output-directory report
	mv *.gcno report/coverage_data
	mv *.gcda report/coverage_data
	mv coverage.info report/coverage_data

open:
	open report/index.html

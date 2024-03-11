CC = gcc
FLAGS = -g -Wall -Wextra -Werror -std=c11

TEST_FLAGS = -lcheck -lpthread -pthread -lm 

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

TEST_SRC = $(wildcard ./tests/*.c)
TEST_OBJ2 = $(patsubst ./tests/%.c, ./tests/%.o, $(TEST_SRC))

ifeq ($(shell uname -s),Linux)
	TEST_FLAGS += -lsubunit -lgcov -lrt
endif

all: clean s21_matrix.a test gcov_report

test.a: $(TEST_OBJ2)
	ar r test.a $^

s21_matrix.a: $(OBJ)
	ar rc s21_matrix.a $^
	ranlib s21_matrix.a
	rm -rf *.o

test: s21_matrix.a test.a
	$(CC) test.a s21_matrix.a $(TEST_FLAGS) -o test.out
	./test.out

test2: s21_matrix.a test.a
	$(CC) $(FLAGS) $(SRC) -o test.out
	./test.out

gcov_report: clean
	$(CC) $(FLAGS) -fprofile-arcs -ftest-coverage $(TEST_SRC) $(SRC) -o test.out $(TEST_FLAGS)
	./test.out
	lcov -t "s21_matrix.a" -o s21_matrix.a.info -c -d .
	lcov --extract s21_matrix.a.info -o filtered.info
	genhtml -o gcov_report filtered.info
	open gcov_report/index.html
	rm -rf *.gcno *.gcda

style_fix:
	clang-format -style=Google -i $(SRC) */*.c */*.h

style:
	clang-format -style=Google -n $(SRC) */*.c */*.h

rebuild: clean all

testforleaks: s21_matrix.a test.a 
	$(CC) $(FLAGS) test.a s21_matrix.a $(TEST_FLAGS) -o test.out

leaks: testforleaks
ifeq ($(shell uname -s), Linux)
	valgrind -s --track-origins=yes --leak-check=yes  ./test.out
else
	leaks --atExit -- ./test.out
endif

clean:
	rm -rf *.o *.a check.c ./tests/*.o ./tests/*.a test.out *.gcno *.gcda gcov_report *.info algebra/*.o comparison/*.o creation/*.o operations/*.o transformations/*.o utils/*.o
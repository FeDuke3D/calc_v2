CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -Werror -Wshadow
LFLAGS = -lgtest

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	LFLAGS += -pthread -lsubunit
endif

TITLE = SmartCalc_v2
TEXI2DVI = texi2dvi

all: test dist install dvi

install : qMakefile
	$(MAKE) -f qMakefile INSTALL_ROOT="~" install

uninstall : qMakefile
	$(MAKE) -f qMakefile INSTALL_ROOT="~" uninstall

# dist : qMakefile
# 	$(MAKE) -f qMakefile DISTNAME="smart_calc_v2" DISTDIR=".tmp/smart_calc_v2" dist

dvi : $(TITLE).dvi

$(TITLE).dvi : $(TITLE).texi
	$(TEXI2DVI) ./$(TITLE).texi

build: qMakefile
	$(MAKE) -f qMakefile CXX="g++ -std=c++17"

tests : test

test: test.o calc_core.o
	$(CC) test.o calc_core.o -o test.out $(LFLAGS)
	./test.out

test.o: test.cpp controller.h
	$(CC) -c $(CFLAGS) test.cpp -o test.o

calc_core.o: calc_core.cpp calc_core.h
	$(CC) -c $(CFLAGS) calc_core.cpp -o calc_core.o

gcov : gcov_report

gcov_report: clean
	$(MAKE) CFLAGS="$(CFLAGS) --coverage" LFLAGS="$(LFLAGS) --coverage" calc_core.o
	$(MAKE) LFLAGS="$(LFLAGS) --coverage" test
	lcov -t test.out -o test.info -c -d . --no-external
	genhtml -o report test.info
	-xdg_open report/index.html

report_clean:
	$(RM) -rf ./*.gcda ./*.gcno ./*.info ./*.gch
	$(RM) -rf ./report

qMakefile:
	qmake MAKEFILE=qMakefile smart_calc_v2.pro
# 'sudo apt install qtchooser' or 'sudo apt install qt5-default' may be needed

checks : style cppcheck

style :
	@clang-format -n --Werror *.h --style=Google --ferror-limit=3
	@clang-format -n --Werror *.cpp --style=Google --ferror-limit=3

cppcheck :
	cppcheck --std=c++17 --enable=all --check-config --suppress=missingIncludeSystem --suppress=missingInclude --suppress=unmatchedSuppression calc_core.cpp mainwindow.cpp calc_core.h controller.h mainwindow.h

leaks: test
	CK_FORK=no leaks --atExit -- ./test.out

mem : test
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./test.out

mem2 : test
	valgrind --undef-value-errors=no --log-file=valgrind-out.txt ./test.out
	grep ERROR valgrind-out.txt

clean: report_clean qMakefile
	$(RM) -rf *.o *.out smart_calc_v2 .tmp *.dvi *.log *.txt
	$(MAKE) -f qMakefile clean
	$(RM) -rf qMakefile .qmake.stash

rebuild :
	$(MAKE) clean
	$(MAKE) build

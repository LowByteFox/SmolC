# SPDX-License-Identifier: BSD-2-Clause
.POSIX:
CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -Iinclude
BIN = smolc
YFLAGS = -d -Wcounterexamples

OBJS = src/main.o src/ast.o src/parser.o src/lexer.o

all: $(BIN)

$(BIN): $(OBJS)
	@echo "  LD   $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

.c.o:
	@echo "  CC   $<"
	@$(CC) $(CFLAGS) -c $< -o $@

.l.o:
	@echo "  LEX  $<"
	@$(LEX) $(LFLAGS) $<
	@$(CC) -c lex.yy.c
	@rm -f lex.yy.c
	@rm -f y.tab.h
	@mv lex.yy.o $@

.y.o:
	@echo "  YACC $<"
	@$(YACC) $(YFLAGS) $<
	@$(CC) $(CFLAGS) -c y.tab.c
	@rm -f y.tab.c
	@mv y.tab.o $@

clean:
	rm -f lex.yy.c $(BIN) $(OBJS)

.PHONY: all clean install uninstall
.SUFFIXES: .c .l .y

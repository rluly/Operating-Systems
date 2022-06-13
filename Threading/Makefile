TARGETS = threadarray numbers cfwin

all:$(TARGETS)

threadarray: threadarray.c
numbers: numbers.c
cfwin: cfwin.c

$(TARGETS):
	gcc -o $@ $^ -lpthread


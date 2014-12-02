
test: bbuffer.c test.c
	@$(CC) $^ -std=c99 -o $@
	@./test

.PHONY: test

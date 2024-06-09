CFLAGS = -g -O0 -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -fsanitize=address -fno-omit-frame-pointer

# default rule. Ignores header file links
%.o: %.c %.h error.h Makefile
	gcc ${CFLAGS} -c -o $@ $<

model.o: model.c Makefile model.h character.h error.h
	gcc ${CFLAGS} -c -o model.o model.c

test_model.o: test_model.c Makefile model.h character.h error.h
	gcc ${CFLAGS} -c -o test_model.o test_model.c

test_model: test_model.o model.o character.o
	gcc ${CFLAGS} -o test_model -lm $^

clean:
	rm *.o test_model

format:
	clang-format -i *.[ch]

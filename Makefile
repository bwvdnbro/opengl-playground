CFLAGS = -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Werror -Wpedantic
LDFLAGS = -fsanitize=address -fno-omit-frame-pointer

# default rule. Ignores header file links
%.o: %.c %.h error.h log.h Makefile
	gcc ${CFLAGS} -c -o $@ $<

model.o: model.c Makefile model.h character.h error.h
	gcc ${CFLAGS} -c -o model.o model.c

test_model.o: test_model.c Makefile model.h character.h error.h
	gcc ${CFLAGS} -c -o test_model.o test_model.c

test_view.o: test_view.c Makefile model.h character.h error.h log.h
	gcc ${CFLAGS} -c -o test_view.o test_view.c

test_texture.o: test_texture.c Makefile texture.h
	gcc ${CFLAGS} -c -o test_texture.o test_texture.c

test_model: test_model.o model.o character.o
	gcc ${CFLAGS} -o test_model -lm $^

test_view: test_view.o model.o character.o
	gcc ${CFLAGS} -o test_view -lm -lGL -lGLU -lglut $^

test_texture: test_texture.o texture.o
	gcc ${CFLAGS} -o test_texture -lm $^

clean:
	rm *.o test_model

format:
	clang-format -i *.[ch]

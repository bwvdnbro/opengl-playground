.DEFAULT_GOAL := all

CFLAGS = -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Werror -Wpedantic
LDFLAGS = -fsanitize=address -fno-omit-frame-pointer

# default rule. Ignores header file links
%.o: %.c %.h error.h log.h Makefile
	gcc ${CFLAGS} -c -o $@ $<

character.o: character.c Makefile character.h error.h log.h character_sprite.h
	gcc ${CFLAGS} -c -o character.o character.c

character_sprite.o: character_sprite.c Makefile character_sprite.h error.h log.h texture.h
	gcc ${CFLAGS} -c -o character_sprite.o character_sprite.c

model.o: model.c Makefile model.h character.h error.h log.h
	gcc ${CFLAGS} -c -o model.o model.c

texture.o: texture.c Makefile texture.h error.h
	gcc ${CFLAGS} -c -o texture.o texture.c

test_model.o: test_model.c Makefile model.h character.h error.h log.h
	gcc ${CFLAGS} -c -o test_model.o test_model.c

test_view.o: test_view.c Makefile model.h character.h error.h log.h character_sprite.h texture.h
	gcc ${CFLAGS} -c -o test_view.o test_view.c

test_texture.o: test_texture.c Makefile texture.h error.h
	gcc ${CFLAGS} -c -o test_texture.o test_texture.c

test_model: test_model.o model.o character.o character_sprite.o texture.o
	gcc ${LDFLAGS} -o test_model -lm -lGL -lGLU -lglut $^

test_view: test_view.o model.o character.o character_sprite.o texture.o
	gcc ${LDFLAGS} -o test_view -lm -lGL -lGLU -lglut $^

test_texture: test_texture.o texture.o character_sprite.o
	gcc ${LDFLAGS} -o test_texture -lm -lGL -lGLU -lglut $^

clean:
	rm *.o test_model

format:
	clang-format -i *.[ch]

all: test_model test_view test_texture

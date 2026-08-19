gl-exp: main.c glad/src/gl.c shader.c stb_image.c texture.c
	clang -lglfw -lGL -lm -Iglad/include $(CFLAGS) -o $@ $^

clean:
	rm gl-exp

.PHONY:clean
hash:hash.c
	gcc -o $@ $^
clean:
	rm -f hash

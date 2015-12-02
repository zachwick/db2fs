.PHONY: clean build

build: db2fs

clean:
	rm -f db2fs db2fs.o

db2fs: db2fs.o
	gcc -std=gnu99 -Wall -I/usr/local/include -L/usr/local/lib/mysql -lmysqlclient $< -o $@

db2fs.o: main.c
	gcc -c $< -std=gnu99 -Wall -lmysqlclient -I/usr/local/include -L/usr/local/lib/mysql -o $@

test: build
	valgrind --leak-check=full --show-leak-kinds=all ./db2fs

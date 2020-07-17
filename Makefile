all: 8080

.PHONY: 8080
8080:
	./build.sh

.PHONY: clean
clean:
	-rm -f 8080

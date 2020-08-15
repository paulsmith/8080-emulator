all: 8080

.PHONY: 8080
8080:
	./build.sh

example.rom: example.asm
	zasm --asm8080 $<

.PHONY: clean
clean:
	-rm -f 8080

SOURCES=$(shell find . -type f -iname '*.c')
TARGET=ku_psort

all: $(TARGET)

$(TARGET): $(SOURCES)
	gcc $(SOURCES) -o $(TARGET) -W -Wall -Wno-unused-parameter

clean:
	rm -f $(TARGET)

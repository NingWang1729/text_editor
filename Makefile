CC = g++
LIBS = -lncurses
STD = -std=c++17

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = wurd

all: $(PRODUCT)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $< -o $@

$(PRODUCT): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o *~ *# *.zip *.tar *.tgz
	rm -f $(PRODUCT)

git: add commit push

add: clean
	git add -A

commit:
	git commit -m "$(m)"

push:
	git push -u origin master

pull:
	git pull

submit: clean
	zip p4.zip Student*

view:
	zipinfo *.zip


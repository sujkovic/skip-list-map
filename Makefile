all: assignment2

SOURCE = extraTest

assignment2: $(SOURCE).o
	g++ -o assignment2 $(SOURCE).o

$(SOURCE).o:
	g++ -c -g -ldl $(SOURCE).cpp

clean:
	rm -f assignment2 *.o
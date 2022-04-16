CXX = g++
CXXFLAGS = -Wall -g

EXEC = compfs
DEPS = scanner/token.h scanner/scanner.h scanner/fsaState.h scanner/fsaTable.h scanner/testscanner.h parser/parser.h parser/node.h parser/productions.h parser/parsetreeprocessing.h parser/tokenstack.h codeGenerator/codegenerator.h
SRC = ./scanner/token.cpp ./scanner/scanner.cpp ./scanner/fsaState.cpp ./scanner/fsaTable.cpp ./scanner/testscanner.cpp ./parser/parser.cpp ./parser/node.cpp ./parser/productions.cpp ./parser/parsetreeprocessing.cpp ./parser/tokenstack.cpp ./codeGenerator/codegenerator.cpp

%.o: %.cpp
	$(CXX) -std=c++11 -c -o $@ $< $(CXXFLAGS)
P0: main.o
	$(CXX) -std=c++11 $(SRC) -o $(EXEC) $^ $(CXXFLAGS)

clean:
	 rm $(EXEC) *.o 

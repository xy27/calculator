.PHONY:clean all

CXX=g++
CXXFLAGS=-Wall -g
BIN=calc
OBJECTS=main.o Calc.o CommandParser.o Exception.o \
		FunctionTable.o Node.o Parser.o Scanner.o \
		Serial.o Storage.o SymbolTable.o Tracer.o

all:$(BIN)

$(BIN):$(OBJECTS)
		 $(CXX) $(CXXFLAGS) -o $@ $^ 

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(BIN)


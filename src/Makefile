CXX=g++
CXXFLAGS=-g -std=c++14
OBJECTS=
DEPENDS=${OBJECTS:.o=.d}
EXEC=a5q1

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
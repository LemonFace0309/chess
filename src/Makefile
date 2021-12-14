CXX=g++
CXXFLAGS=-g -std=c++14
OBJECTS=main.o bishop.o board.o game.o king.o knight.o observer.o pawn.o piece.o pieceEnum.o playerEnum.o queen.o rook.o square.o subject.o textdisplay.o window.o graphicsDisplay.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=chess

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
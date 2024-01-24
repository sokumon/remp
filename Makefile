cxx := g++
win_cxx := x86_64-w64-mingw32-g++
target_program := remp
src_dirs := ./src/*.cpp
LIBS := -lcurl -lssl -lcrypto -lz
INC := -Iinclude/


linux:
	${cxx} ${CXXFLAGS} ${src_dirs} -o ${target_program} ${LIBS} ${INC} -g

clean:
	rm -f ${target_program}

test:
	g++ test.cpp -o test -lcurl -lssl -lz -lcrypto -Iinclude/ 

win-test:
	${win_cxx} test.cpp -o test -lcurl  -Iinclude/  -static
	
windows:
	${win_cxx} ${CXXFLAGS} ${src_dirs} -o ${target_program} ${LIBS} ${INC} -g
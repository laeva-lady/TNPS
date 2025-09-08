r:
	clear
	cmake . -B build
	make -C build -j12
	mv build/tnps .
	# ./tnps ./source/main.cc ./source/Utils.cc ./header/Utils.hpp ./header/Todo.hpp ./filename.txt
	./tnps ./CMakeLists.txt ./Makefile ./source/main.cc ./source/todo.cc ./source/Utils.cc ./.clangd ./filename.txt ./todos.txt ./.gitignore ./header/Utils.hpp ./header/Todo.hpp ./.clang-format ./tnps

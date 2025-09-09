r:
	clear
	cmake . -B build
	make -C build -j12
	mv build/tnps .
	# ./tnps ./source/main.cc ./source/Utils.cc ./header/Utils.hpp ./header/Todo.hpp ./filename.txt
	# ./tnps TODO filename.txt
	./tnps -p TODO -f source/main.cc filename.txt -- some other stuff here too

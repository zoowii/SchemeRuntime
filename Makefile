ALL: ss
ss: *.h *.cpp
	g++ -o ss *.h *.cpp

clear:
	rm ss.exe || rm ss

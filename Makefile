All: ss
ss: *.h *.cpp
	g++ -o ss *.h *.cpp -Wall -std=c++0x -fpermissive

clean:
	rm ss.exe || rm ss

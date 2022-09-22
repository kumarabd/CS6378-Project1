all:
	g++ main.cpp types.cpp output.cpp -o build/app 
	./build/app

clean:
	rm -f build/*
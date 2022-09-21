all:
	g++ main.cpp types.cpp -o build/app 
	./build/app

clean:
	rm -f build/*
all:
	g++ main.cpp network.cpp node.cpp output.cpp -o build/app 
	./build/app

clean:
	rm -f build/*
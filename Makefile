all:
	gcc hw4.c -std=gnu99 -lm -o hw4
	./hw4 -input trace1.txt -output mytrace1out
	./hw4 -input trace2.txt -output mytrace2out
	./hw4 -input trace3.txt -output mytrace3out
clean:
	rm -f hw4


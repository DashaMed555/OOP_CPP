sound_processor: main.o wav_processing.o converters.o parser.o
	g++ main.o wav_processing.o converters.o parser.o -o sound_processor

main.o: main.cpp
	g++ main.cpp -c

wav_processing.o: wav_processing.cpp
	g++ wav_processing.cpp -c

converters.o: converters.cpp
	g++ converters.cpp -c

parser.o: parser.cpp
	g++ parser.cpp -c

clean:
	rm *.o sound_processor

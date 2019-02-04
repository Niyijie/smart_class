OPENCVLIB	= `pkg-config --cflags --libs opencv`
HIKLIBPATH	= ./video/sdk
HIKLIBS		= -Wl,-rpath=$(HIKLIBPATH):$(HIKLIBPATH)/HCNetSDKCom -lhcnetsdk

all : pisender tserver
.PHONY: clean

pisender: sender_main.o detector.o mtcnn.o sender.o sender_socket.o util.o video_capture.o video_frame.o
	g++ -std=c++11 sender_main.o detector.o mtcnn.o sender.o sender_socket.o util.o video_capture.o video_frame.o -o pisender ./libncnn.a -L$(HIKLIBPATH) $(HIKLIBS) $(OPENCVLIB) -fopenmp
tserver: server_main.o server.o server_socket.o util.o
	g++ -std=c++11 server_main.o server.o server_socket.o util.o -o tserver $(OPENCVLIB)
sender_main.o: sender_main.cpp
	g++ sender_main.cpp -c
detector.o: ./detect/detector.cpp
	g++ ./detect/detector.cpp -c
mtcnn.o: ./detect/mtcnn.cpp
	g++ ./detect/mtcnn.cpp -c
sender.o: ./sender/sender.cpp
	g++ ./sender/sender.cpp -c
sender_socket.o: ./sender/sender_socket.cpp
	g++ ./sender/sender_socket.cpp -c
util.o: ./util/util.cpp
	g++ ./util/util.cpp -c
video_capture.o: ./video/video_capture.cpp
	g++ ./video/video_capture.cpp -c
video_frame.o: ./video/video_frame.cpp
	g++ ./video/video_frame.cpp -c
server_main.o: server_main.cpp
	g++ server_main.cpp -c
server_socket.o: ./server/server_socket.cpp
	g++ ./server/server_socket.cpp -c
server.o: ./server/server.cpp
	g++ ./server/server.cpp -c
clean:
	-rm -rf *.o

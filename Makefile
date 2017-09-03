CXX=g++
CFLAGS=-Wall -g3 -O3 -I. -Isorting -Iuser_interface `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`
CMD_LINE=dbus-launch ./pixel_sorter photo.jpg

all:
	$(CXX) $(CFLAGS) $(LIBS) *.cpp sorting/*.cpp user_interface/*.cpp -o pixel_sorter

run: all
	$(CMD_LINE)


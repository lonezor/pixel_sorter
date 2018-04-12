CXX=g++
CFLAGS=-Wall -g3 -O3 -I. -Isorting -Iuser_interface `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`
CMD_LINE=dbus-launch ./pixel_sorter photo1.jpg

all:
	$(CXX) $(CFLAGS) $(LIBS) *.cpp sorting/*.cpp user_interface/*.cpp -o pixel_sorter

run: all
	$(CMD_LINE)

ut:
	$(CXX) $(CFLAGS) -DUNIT_TEST $(LIBS) *.cpp sorting/*.cpp user_interface/*.cpp -o unit_test

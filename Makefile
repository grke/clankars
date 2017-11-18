OBJS= \
	clankars.o \
	ckmisc.o \
	ckweapon.o \
	caimagemanipulation.o \
	ckcar.o \
	cktrack.o \
	ckobj.o \
	ckmenu.o \

all: $(OBJS)
	g++ -Wall -o \
	clankars -lclanCore -lclanDisplay -lclanApp -lclanPNG $(OBJS)

clean:
	-rm -rf *.o
	-rm -f clankars

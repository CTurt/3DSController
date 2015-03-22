CC		:=	gcc
CP		:=	g++
LN		:=	g++
ODIR	:=	build
SDIR	:=	source
IDIR	:=	include
LDIR	:=	lib
CFLAGS	:=	-I$(IDIR) -fms-extensions -O2 -Wall
LFLAGS	:=	-L$(LDIR) -lvJoyInterface -lws2_32 -lGdi32 -lgdiplus -static-libgcc -static-libstdc++
CFILES	:=	$(wildcard $(SDIR)/*.c)
CPPFILES	:=	$(wildcard $(SDIR)/*.cpp)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))
OBJS	+=	$(patsubst $(SDIR)/%.cpp, build/%.o, $(wildcard $(SDIR)/*.cpp))

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	TARGET=3DSController
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	TARGET=3DSController
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	TARGET=3DSController.exe
endif

$(TARGET): $(ODIR) $(OBJS)
	$(LN) $(ODIR)/*.o -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CP) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o

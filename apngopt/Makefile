PACKAGE    = apngopt
CC         = g++
SRC_DIRS   = . 7z zopfli libpng zlib
CFLAGS     = -Wall -pedantic
CFLAGS_OPT = -O3
CFLAGS_7Z  = -Wno-sign-compare -Wno-reorder -Wno-maybe-uninitialized -Wno-parentheses

INCLUDE_DIRS := $(addprefix -I./, $(SRC_DIRS))
INCLUDE_DIRS += -I./imagequant
OBJ_DIRS := $(addprefix obj/, $(SRC_DIRS))
OBJECTS := $(addprefix obj/, $(wildcard $(addsuffix /*.c*, $(SRC_DIRS))))
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.cc=.o)
OBJECTS := $(OBJECTS:.cpp=.o)

all : $(PACKAGE)

$(PACKAGE) : imagequant objdirs $(OBJECTS)
	$(CC) -o $@ -c $(OBJECTS) $(LIBS)

imagequant : imagequant/target/release/libimagequant_sys.a
imagequant/target/release/libimagequant_sys.a:
	cd imagequant; cargo build --release

objdirs :
	mkdir -p $(OBJ_DIRS)

obj/%.o : %.cpp
	$(CC) -o $@ -c $< $(INCLUDE_DIRS) $(CFLAGS) $(CFLAGS_OPT)

obj/%.o : %.c
	gcc -o $@ -c $< $(INCLUDE_DIRS) $(CFLAGS) $(CFLAGS_OPT)

obj/%.o : %.cc
	cc -o $@ -c $< $(INCLUDE_DIRS) $(CFLAGS) $(CFLAGS_OPT) $(CFLAGS_7Z)

.PHONY : clean

clean : 
	rm -rf $(PACKAGE) obj

clean_all: clean
	rm -rf imagequant/target
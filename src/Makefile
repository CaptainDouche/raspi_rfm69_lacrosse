USE_MOSQUITTO = 1
# change these to set the proper directories where each files should be
TARGET     = rfm69_lacrosse
SRCDIR     = .
OBJDIR     = /tmp/obj_$(TARGET)
BINDIR     = /tmp
INSTALLDIR = /bin

CC       = gcc
DEFS     = -D_BSD_SOURCE -D_LINUX -DLINUX -D__RASPI__ -DRFM69=1
# compiling flags here
#CFLAGS   = -x c -std=c99 -Wall -D_LINUX


# http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_SPI.html
# $ modprobe spi_bcm2708
# $ modprobe: ERROR: could not insert 'spi_bcm2708': No such device
# $ modprobe spi_bcm2835
# $ sudo modprobe spidev

LINKER   = gcc -o
# linking flags here
#LFLAGS   = -lrt -lpthread -lm
LFLAGS   = -lrt

ifeq ($(USE_MOSQUITTO), 1)
	DEFS += -DUSE_MOSQUITTO=1
	MOSQ_LFLAGS = -lmosquitto -DUSE_MOSQUITTO=1
	LFLAGS += $(MOSQ_LFLAGS)
endif

CFLAGS   = -std=c99 $(DEFS)

GLOBCFGS := 
SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

all: $(BINDIR)/$(TARGET)
	@echo "Building $(BINDIR)/$(TARGET)"
	
dump:
	@echo SOURCES=$(SOURCES)
	@echo
	@echo OBJECTS=$(OBJECTS)
	
#objs:
#	@$(foreach I,$(OBJECTS), echo $(I);)
	
$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(GLOBCFGS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"
	
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking "$<" complete!"
	
dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)	
	

	
install: $(BINDIR)/$(TARGET)
	cp $(BINDIR)/$(TARGET) $(INSTALLDIR)
	#enable letting it run as root for normal users:
	chmod a+s $(INSTALLDIR)/$(TARGET)
	
clean:
	@$(rm) $(OBJECTS)

	@echo "Cleanup complete!"

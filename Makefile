CONTIKI_PROJECT = server
all: $(CONTIKI_PROJECT)

CONTIKI = ../..

MAKE_NET = MAKE_NET_NULLNET

include $(CONTIKI)/Makefile.include

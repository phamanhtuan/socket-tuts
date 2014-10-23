ODIR = obj
SDIR = src
BDIR = bin
CC = g++
_OBJS = client.o server.o

OBJS = $(patsubst %, $(ODIR)/%, %(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $@ $< $(CFLAGS)
$(BDIR)/%: $(ODIR)/%.o
	$(CC) -0 $@ $< 
clean:
	rm -f $(ODIR)/*.o $(OUT)
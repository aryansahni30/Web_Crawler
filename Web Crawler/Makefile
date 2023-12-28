start = gcc
end = -lcurl
content = bag.o crawler.o curl.o pagedir.o hashtable.o set.o url.o
middle = -Wall -Wpedantic -ggdb -Werror
execute = crawler

.PHONY: all clean

all: $(execute)

$(execute): $(content)
	$(start) $(middle) $^ $(end) -o $@

%.o: %.c
	$(start) $(middle) -c $<

crawler.o: crawler.c crawler.h pagedir.h hashtable.h set.h bag.h curl.h url.h
pagedir.o: pagedir.c pagedir.h crawler.h
hashtable.o: hashtable.c hashtable.h set.h
set.o: set.c set.h
bag.o: bag.c bag.h
curl.o: curl.c curl.h
url.o: url.c url.h

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(execute)
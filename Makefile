CC=g++-4.9
CFLAGS= -I`g++-4.9 -print-file-name=plugin`/include -fPIC

./lib/print_filename_plugin.so: ./src/print_filename_plugin.cxx
	$(CC) $(CFLAGS) -dynamiclib -undefined dynamic_lookup $^ -o $@
	$(CC) -S -x c++ -fplugin=./lib/print_filename_plugin.so test/test.cxx

clean:
	rm -f lib/*o *~ *out

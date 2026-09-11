cooke_parser: front.o parser.o
	$(CC) -Wall front.o parser.o -o cooke_parser

.c.o:
	$(CC) -Wall -c $<


test: cooke_parser front.in
	./cooke_analyzer front.in

clean:
	$(RM) parser.o front.o cooke_parser

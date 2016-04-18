all: brinkhuis.c brinkhuis2t1.c brinkhuis2t2.c
	clang -O3 brinkhuis.c -o brinkhuis
	clang -O3 brinkhuis2t1.c -o brinkhuis2t1
	clang -O3 brinkhuis2t2.c -o brinkhuis2t2
	clang++ DHCS.cpp -O3 -o dhcs
	clang++ RHCS.cpp -O3 -o rhcs

clean:
	$(RM) brinkhuis brinkhuis2t1 brinkhuis2t2 rhcs dhcs

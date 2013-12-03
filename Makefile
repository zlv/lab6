SOURCEOBJS = polynom.cpp PolStr.o 
main :
	g++ -o lab4 $(SOURCEOBJS) 
main-debug :
	g++ -g -O1 -o lab4 $(SOURCEOBJS)

SOURCEOBJS = spline.cpp PolStr.o 
main :
	g++ -o lab6 $(SOURCEOBJS) 
main-32 :
	g++ -o lab6 spline.cpp PolStr32.o
main-debug :
	g++ -g -O1 -o lab6 $(SOURCEOBJS)

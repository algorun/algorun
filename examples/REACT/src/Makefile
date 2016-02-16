#
# Makefile for Project EA_Two
#   VBI  18-July-2007
#
#  primary target goes first
#
React: BitStringNew.o DynSysModel.o NTuple.o Monomial.o Polynomial.o GAParams.o FileManager.o GAModel.o React.o
	$(CXX) BitStringNew.o DynSysModel.o NTuple.o Monomial.o Polynomial.o GAParams.o FileManager.o GAModel.o React.o -O3 -o React

BitStringNew.o: BitString.cpp
	$(CXX) BitStringNew.cpp -O3 -c

DynSysModel.o: DynSysModel.cpp
	$(CXX) DynSysModel.cpp -O3 -c -g

NTuple.o: NTuple.cpp
	$(CXX) NTuple.cpp -O3 -c -g

Monomial.o: Monomial.cpp
	$(CXX) Monomial.cpp -O3 -c -g

Polynomial.o: Polynomial.cpp
	$(CXX) Polynomial.cpp -O3 -c -g

GAParams.o: GAParams.cpp
	$(CXX) GAParams.cpp -O3 -c -g

FileManager.o: FileManager.cpp
	$(CXX) FileManager.cpp -O3 -c -g

GAModel.o: GAModel.cpp
	$(CXX) GAModel.cpp -O3 -c -g

React.o: React.cpp
	$(CXX) React.cpp -O3 -c -g


clean:
	rm -rf *.o React


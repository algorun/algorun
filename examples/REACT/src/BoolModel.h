// BoolModel.h
//
//  This is the main header file for the project:
//			Evolutionary Algorithm for Discrete Dynamical System Optimization
//
//	contains global definitions and pulls in include files in a specific order
//
#ifndef _VBI_POLYMATH_BOOL_MODEL_H_
#define _VBI_POLYMATH_BOOL_MODEL_H_

// Include the basic stuff
#include <cstdlib>
#include <cmath>

typedef unsigned short USHORT;
typedef unsigned int ULONG;

// Standard Template Library Containers
//  dynamically sized array, fast random access, no insertion/deletion
#include <vector>

//  doubly linked list, fast insertion/deletion, no random access
#include <list>

//  sorted set where key=data, no duplicates, lookup by value
#include <set>

//  sorted set of key, value pairs
#include <map>

//	arbitrary length binary string with boolean operators
#include <bitset>

//	a character string
#include <string>

// include time functions for randomization, timing
#include <ctime>

// include stdlib algorithms including min, max
#include <algorithm>

// include support for console input / output
#include <iostream>

// include file input / output definitions
#include <fstream>

// using std::set;
// using std::bitset;

#define MAX_VARIABLES 8
#define MAX_TERMS     80

// typedef std::bitset<MAX_VARIABLES> TermType;
// typedef std::bitset<MAX_VARIABLES> TupleType;

typedef std::vector<unsigned short> UShortTable;
typedef std::vector<unsigned short>::iterator UShortTableIter;

typedef std::vector<ULONG> WordArray;
typedef std::vector<ULONG>::iterator WordArrayIter;
typedef std::vector<ULONG>::const_iterator WordArrayConstIter;

typedef std::vector<double> DoubleVector;
typedef DoubleVector::iterator DoubleVectorIter;

// A simple (possibly sparse) matrix of doubles
typedef std::vector<double> WeightMatrixRow;
typedef WeightMatrixRow::iterator WeightMatrixRowIter;

typedef std::vector<WeightMatrixRow> WeightMatrix;
typedef WeightMatrix::iterator WeightMatrixIter;

struct ComplexityQuotient
{
	int	p;
	int	q;
};

typedef std::vector<ComplexityQuotient> ComplexityMatrixEntry;
typedef ComplexityMatrixEntry::iterator ComplexityMatrixEntryIter;

typedef std::vector<ComplexityMatrixEntry> ComplexityMatrixRow;
typedef ComplexityMatrixRow::iterator ComplexityMatrixRowIter;

typedef std::vector<ComplexityMatrixRow> ComplexityMatrix;
typedef ComplexityMatrix::iterator ComplexityMatrixIter;

// A handy definition for the STL string type
typedef std::string String;

typedef std::vector<String> StringVector;
typedef StringVector::iterator StringVectorIter;

//Comment out the next line to eliminate debug file
//#define GA_DEBUG_FILE

#include "PolyMathRand.h"

#include "BitStringNew.h"

typedef BitString TermType;
typedef BitString TupleType;

#include "PolyMathRand.h"
#include "GAParams.h"

#include "NTuple.h"
#include "Monomial.h"	
#include "Polynomial.h"
#include "DynSysModel.h"
#include "FileManager.h"
#include "GAModel.h"


//#include "BoolDynSysGA.h"

#endif //_VBI_POLYMATH_BOOL_MODEL_H_

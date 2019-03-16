#pragma once
#ifndef CLASSES_H_
#define CLASSES_H_

#include "includes.h"
#include "stormscript.h"
#include "variables.h"
#include "functions.h"

class stsclass {
public:
	void declare(int *y, sts *inst);
	void decmethod(sts *inst, int *ln);
	std::vector<stsvars> variables;
	std::vector<int> constructors; // this is a vector that tells us the indexes of the constructors in the variables vector
	std::vector<stsfunc> methods;
	string name;
};

class stsclasstype:public stsvars{
public:
	stsclass tpe;
	std::vector<int> indexes;
};

#endif
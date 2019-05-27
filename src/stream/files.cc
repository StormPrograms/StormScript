#include "../include/stormscript.h"

stsvars sts::readfile(int *y, std::vector<stsvars> *vars, std::vector<stsfunc> functions) {
    stsvars v;
    v.type = 's';

    *y+= 1;

    std::ifstream file;
    string contents;
    string name = getval(vars, functions, y).val;

    file.open(name);

    if (file.fail()) 
		error(11, name);

    char c = file.get();

    while (file.good()) {
        contents += c;
        c = file.get();
    }

    file.close();

    v.val = contents;

    return v;
}

void sts::writefile(int *y, std::vector<stsvars> *vars, std::vector<stsfunc> functions) {
    *y += 1;
    std::ofstream file;
    string name = getval(vars, functions, y).val;

    file.open(name);

    *y += 1;
    string contents = getval(vars, functions, y).val;

    file.write(contents.c_str(), contents.size());
    file.close();

    *y += 1;
}
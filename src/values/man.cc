#include "../include/stormscript.h"

/* 
This file deals with non-keyword commands like:
    variable manipulation
    functions
    class type declarations
*/


bool sts::valchange(std::vector<stsvars> * pvars, std::vector<stsclasstype> *classtypes, int *ln){ //changes the value of the stsvars list
    std::vector<stsvars> vars = *pvars;
    int y = *ln;
    std::vector<stsclasstype> ct = *classtypes;

    if (prs[y].back() == ':') {
        int *index = new int(0);
        
        string *name = new string(prs[y]);
        name->pop_back();

        if (!isvar(pvars, *name, index))
            pvars->push_back(declare(ln, pvars));
        else {
            pvars->at(*index).val = getval(vars, new int(y+1)).val;
            if ((pvars->at(*index).type == 's') || (pvars->at(*index).type == 'l')) {
                int *ind = new int(0);

                // change length value of variable
                if (isvar(pvars, pvars->at(*index).name + "|length", ind)) {
                    pvars->at(*index).length = ((pvars->at(*index).type == 's') ? 
                        pvars->at(*index).val.size() : pvars->at(*index).vals.size());
                    pvars->at(*ind).val = std::to_string(pvars->at(*index).length);
                    
                    if (pvars->at(*index).glob)
                        globvars[*ind] = pvars->at(*ind);
                }
            
            }
            if (pvars->at(*index).glob)
                globvars[*index] = pvars->at(*index);
        }

        while (prs[y]!=";")
            y++;

        *ln = y;

        return true;
    }

    if ((prs[y+1] == "[") && (prs[y+4] == ":")) {
        int *index = new int(0);

        string name = prs[y];
        
        if (isvar(pvars, name, index)) {
            if (pvars->at(*index).type == 'l')
                pvars->at(*index).vals[std::stoi(getval(vars, new int(y+2)).val)].val = getval(vars, new int(y+5)).val;
            else
                pvars->at(*index).val[std::stoi(getval(vars, new int(y+2)).val)] = getval(vars, new int(y+5)).val[0];

            if (pvars->at(*index).glob)
                globvars[*index] = pvars->at(*index);
        }
        else
            error(12, prs[y]);

        while (prs[y]!=";")
            y++;
        
        *ln = y;
        return true;
    }

    if (prs[y+2].back() == ':') {
        int *num = new int(0);

        if (isvar(pvars, prs[y], num)) {
            if (prs[y+1] == "+") {
                if (pvars->at(*num).type == 's')
                    pvars->at(*num).val += getval(vars, new int(y+3)).val;
                else if (pvars->at(*num).type == 'i') {
                    if (isint(prs[y+3]))
                        pvars->at(*num).val = std::to_string(
                            std::stoi(pvars->at(*num).val) + std::stoi(prs[y+3])
                        );
                    else
                        error(9, prs[y+3]);
                }
                else
                    error(9, prs[y]);
            }
            else if (prs[y+1] == "-") {
                if (pvars->at(*num).type == 'i') {
                    if (isint(prs[y+3]))
                        pvars->at(*num).val = std::to_string(
                            std::stoi(pvars->at(*num).val) - std::stoi(prs[y+3])
                        );
                    else
                        error(9, prs[y+3]);
                }
                else
                    error(9, prs[y]);
            }
            if (pvars->at(*num).glob) {
                int *size_num = new int(-1);

                globvars[*num] = pvars->at(*num);

                if (isvar(pvars, pvars->at(*num).name + "|length", size_num))
                    globvars[*size_num] = pvars->at(*size_num);
            }

            y += 3;
            *ln = y;
            return true;
        }
    }

    if (functions.size()>0){
        bool isfunc = false;

        for (int i = 0; i < functions.size() && !isfunc; i++)
            isfunc = (functions[i].name == prs[y]);

        if (isfunc) {
            runfunc(&vars, &ct, &y);
            y++;
            *ln = y;
            *classtypes = ct;
            *pvars = vars;
            return true;
        }
    }

    if (classes.size()!=0) {
        for (int i = 0; i<classes.size() && classes[i-1].name!=prs[y]; i++){
            if (classes[i].name==prs[y]) {
                ct.resize(ct.size()+1);
                ct.back().tpe = classes[i];
                y++;
                ct.back().name = prs[y];

                if (prs[y].back() == ':')
                    ct.back().name.pop_back();

                for (int b = 0; b<ct[ct.size()-1].tpe.variables.size(); b++) {
                    vars.resize(vars.size()+1);
                    vars.back().name = ct[i].name + "|" + ct[i].tpe.variables[b].name;
                    vars.back().type = ct[i].tpe.variables[b].type;

                    for (int c = 0; c < ct[i].tpe.constructors.size(); c++) {
                        int currentind = ct[i].tpe.constructors[c];

                        if (b == currentind) {
                            y++;
                            vars.back().val = getval(vars, new int(y)).val;
                        }
                    } 

                    ct[i].indexes.push_back(vars.size()-1); // dump index for reading during execution of method
                }

                for (int b = 0; b<ct[ct.size()-1].tpe.methods.size(); b++) {
                    functions.push_back(ct[i].tpe.methods[b]);
                    functions.back().name = ct[i].name + "|" + ct[i].tpe.methods[b].name;
                    functions.back().cof = ct[i].name;
                }
                y++;

                *ln = y;
                *classtypes = ct;
                *pvars = vars;
                
                return true;
            }

        }
    }

    return false;
}
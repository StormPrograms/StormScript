#include "../core/stsclasses.h"

bool condition(sts *program, int *ln, std::vector<stsvars> vars) {
    std::vector<string> prs = program->prs;
    sts prg = *program;
    int y = *ln;
    bool v;

    prg.prs = { program->prs[y] };


    stsvars val1 = prg.getval(vars, new int(0));

    prg.prs = { program->prs[y+2] };
    stsvars val2 = prg.getval(vars, new int(0));

    if (val1.type == val2.type) {
        // compare based on conditional operator
        switch (val1.type) {
            case 's':
                v = (val1.valstring == val2.valstring);
                break;
            case 'i':
                v = (val1.valint == val2.valint);
                break;
            case 'b':
                v = (val1.val == val2.val);
                break;
        }
    }
    else {
        // give error with value based on type
        program->error(9, ((val2.type == 'i') ? std::to_string(val2.valint) : 
            ((val2.type=='s') ? val2.valstring : 
                ((val2.val == true) ? "true" : "false")
            ))
        );
    }

    return v;
}
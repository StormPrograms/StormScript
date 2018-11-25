#include "../core/stsclasses.h"

void sts::interp(string fname,int psize, char *argv[], int argc){
    prs = parse(prg);

    globvars.resize(globvars.size()+1);
    for (int x = 1; x<=argc-1; x++){
        globvars[globvars.size()-1].type='l';
        globvars[globvars.size()-1].vals.resize(globvars[globvars.size()-1].vals.size()+1);
        globvars[globvars.size()-1].vals[globvars[globvars.size()-1].vals.size()-1].type = 's';
        globvars[globvars.size()-1].vals[globvars[globvars.size()-1].vals.size()-1].valstring=argv[x];
        globvars[globvars.size()-1].name="arg";
        globvars[globvars.size()-1].glob=1;
    }
    
    for (int x = 0; x<prs.size(); x++){
        if (prs[x]=="lib"){
            names.resize(names.size()+1);
            x++;
            names[names.size()-1]=prs[x];
        }
        else if ((prs[x]=="int") || (prs[x]=="str") || (prs[x]=="bool") || (prs[x]=="list")){
            x++;
            if (prs[x-1]=="int")
                globvars.push_back(declare('i', x, &globvars));
            else if (prs[x-1]=="str") {
                globvars.push_back(declare('s', x, &globvars));
                globvars[globvars.size()-2].glob = true;
            }
            else if (prs[x-1]=="bool")
                globvars.push_back(declare('b', x, &globvars));
            else if (prs[x-1]=="list") {

                globvars.push_back(declare('l', x, &globvars));
                globvars[globvars.size()-2].glob = true;
            }
            while (prs[x]!=";") 
                x++;
            globvars.back().glob=1;
        }

        else if (prs[x]=="type") { // declares a class
            classes.resize(classes.size()+1);
            classes[classes.size()-1].declare(&x, this);
        }

        else if (prs[x]=="mod") {
            x++;
            addmodule(&x);
        }

        else if (prs[x]=="set") {
            x++;
            set(prs[x], prs[x+2], x);
            x+=2;
        }
        else if (prs[x]=="func"){
            functions.resize(functions.size()+1);
            x++;
            functions.back().name=prs[x];
            x++;

            if (prs[x]=="=>") {
                x++;
                std::vector<stsvars> args;

                while (prs[x]!="{") {
                    args.resize(args.size()+1);

                    if (prs[x] == "bool") {
                        args.back().type='b';
                    }
                    else if (prs[x] == "str") {
                        args.back().type='s';
                    }
                    else if (prs[x] == "int") {
                        args.back().type='i';
                    }
                    else {
                        error(2, prs[x]);
                    }
                    x++;
                    args[args.size()-1].name = prs[x];
                    x++;
                }
                x++;
                functions.back().args=args;
            }
            
            functions[functions.size()-1].linestarted=x;
            int endreq = 1;
            while (endreq != 0) {
                if ((prs[x]=="}") || (prs[x]=="loop"))
                    endreq--;
                else if (((prs[x]=="if") && (prs[x-1]!="else")) || (prs[x]=="else"))
                    endreq++;
                x++;
            }
            x--;
            if (prs[x]=="loop")
                x+=2;
        }
        else if (prs[x]=="do"){
            exec(&x, ((psize==-1) ? -2 : -1), {}, {});
        }
        else if ((prs[x]!=";") && (prs[x][0]!='\0')){
            error(1, prs[x]);
        }
    }
}   
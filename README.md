# AI2_SecondAssignment

### Per compilare dobbiamo eseguire questo comandi
```
DENTRO visit_domain
../popf3-clp -x dom1.pddl prob1.pddl ../visits_module/build/libVisits.so region poses

DENTRO visit_module/build
cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_BUILD_TYPE=Release ../src/

make
```
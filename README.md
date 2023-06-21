# AI2_SecondAssignment

### Per compilare dobbiamo eseguire questo comandi
```
DENTRO visit_domain
../popf3-clp -x dom1.pddl prob1.pddl ../visits_module/build/libVisits.so region poses

DENTRO visit_module/build
cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_BUILD_TYPE=Release ../src/

make
```
### Informazioni e commenti utili per il progetto
1) all'interno del domain e problem vengono usate le regioni come luoghi che possono essere visitati dal robot. Poi nel file region_poses viene specificato per ogni regione il waypoint corrispondente.
2) r2d2 è semplicemente il nome del robot all'interno del file problem.pddl

### FILE DA MODIFICARE RISPETTO A IL PROGETTO TROVATO SU INTERNET
CMakelist.txt (ho già aggiunto le reighe ma commentate)
VisitSolver.cpp
VisitSolver.h
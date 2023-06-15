# AI2_SecondAssignment

### Per compilare dobbiamo eseguire questo comando dentro la cartella visits_domain
```
../popf3-clp -x dom1.pddl prob1.pddl ../visits_module/build/libVisits.so region poses
```
### Informazioni e commenti utili per il progetto
1) all'interno del domain e problem vengono usate le regioni come luoghi che possono essere visitati dal robot. Poi nel file region_poses viene specificato per ogni regione il waypoint corrispondente.
2) r2d2 è semplicemente il nome del robot all'interno del file problem.pddl
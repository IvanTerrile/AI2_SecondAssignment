# AI2 Second Assignment
This repository contains the code for the second assignment of the Artificial Intelligence 2 course at the course of Robotic Engineering at the University of Genoa.

---------------------------------------
## Installation requirements
To run the code you need to install the popf-tif planner, the details of which can be found here:

[https://github.com/popftif/popf-tif](https://github.com/popftif/popf-tif) 

Anyway, the executable file is already present in the repository and it is called **popf3-clp**.

----------------------------------
## How to build the external module
Once installed, download or clone this repository and follow the instruction inside the **buildInstruction.txt** to build the external module.
The external module are some cpp files, inside the **visit_module/src/** folder, that are used to make the motion planning of the robot according to the instructions given by the assignemnt.

-----------------------------------
## How to run the code
You can run the code by executing the following command, inside the **visit_domain/** folder:

```
../popf3-clp -x dom1.pddl prob1.pddl ../visits_module/build/libVisits.so region poses
```
where:
- **/popf3-clp** is the executable file of the planner
- **-x** is the flag to specify the execution of the external module
- **dom1.pddl** is the domain file
- **prob1.pddl** is the problem file
- **../visits_module/build/libVisits.so** is the external module 
- **region** is the name of the region 
- **poses** is the name of the poses

------------------------------------
## Authors

* **Alessio Mura** - [alemuraa](https://github.com/alemuraa)
* **Pisano Davide** - [DavidePisano](https://github.com/DavidePisano)
* **Miriam Anna Ruggero** - [Miryru](https://github.com/Miryru)
* **Terrile Ivan** - [IvanTerrile](https://github.com/IvanTerrile)

# Aulavik
#### Tiny hobby OS written in C

### README WIP

## Workspace setup

### Compiler
You should have a cross-compiler for the architecture you wish to build for. 
A good guide on how to do this can be found on the OS Dev wiki. At the moment, 
the build system assumes it to be a fork of GCC, and thus looks for (ARCH)-gcc
in your path. 

### Note for IDEA IDE users
If you are using CLion (or any other IDEA IDE), you should go to *Build, 
Execution, and Deployment* -> *Toolchains*, create one for this project, and 
select your cross-compiler for your target architecture. This will stop CLion
from always assuming a hosted environment.
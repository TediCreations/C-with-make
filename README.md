# C project development using both GNU make and Eclipse IDE

[![Build Status](https://travis-ci.org/TediCreations/C-with-make.svg?branch=master)](https://travis-ci.org/TediCreations/C-with-make)

------

# Instructions:

## When using Eclipse:

1. Open Eclipse with **proj/eclipse_workspace** as the workspace.
2. Import existing project in the solution.
3. Start developing...

## When using GNU make:

1. Run **make**

------
# Info:

## Tools:

- **IDE:**              Eclipse (Neon 3) with CDT
- **Build tools:**      gcc, make
- **Documentation:**    doxygen, graphviz
- **Analysers:**        cppcheck

## Folder structure:

- **bin:**              Output of the executable
- **conf:**             Configuration files from various tools. e.g: doxygen, cppcheck etc.
- **doc:**              Documentation. (output of doxygen).
- **obj:**              Object files during make.
- **proj:**             Eclipse IDE config files.
- **src:**              Projects source files.

------

# TODO:

- Project is working in GNU/Linux, fix Travis CI errors...

- Add script to install dependencies in Debian based GNU/Linux distros.
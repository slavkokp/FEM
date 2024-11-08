# Finite element method implementation

This repository contains static library FEM with implementation of finite element method which is built using visual studio 2022 and user interface written on Qt framework which utilizes [matplot++](https://github.com/alandefreitas/matplotplusplus) library for plotting 3d model.<br />

This finite element method implementation allows to perform automatic triangulation after setting only the coordinates of 20 inital nodes of an object, received mesh can also be adjusted manually if necessary. The UI allows to specify coordiantes of 20 inital nodes, constants (Young's modulus and Poisson's coefficient), fixated and powered sides (with power value) of finite elements and the quantity of elements along each of the three axes.<br />

![UI demo](https://github.com/user-attachments/assets/11892e4f-8770-4e72-bb73-b8101122e8b3)

Running the application<br /> 
The static library is compiled using visual studio 2022 and C++17 or higher. The UI is written using  Qt 6.8 and is compiled with qmake. To run UI you also have to install the matplot++ library and specify the path to your installation in [FEMInterfaceQt.pro](https://github.com/slavkokp/FEM/master/FEMInterfaceQt) file.



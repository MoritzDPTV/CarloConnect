# CarloConnect
*CarloConnect* is a Monte Carlo Tree Search (MCTS) based game artificial intelligence (AI) to play Connect Four - implemented in C++. The game of Connect Four using CarloConnect can be played online on [https://carloconnect.eu/](https://carloconnect.eu/).

<p align="center">
  <img src="https://carloconnect.eu/icons/overview-600.png" width="350"/>
</p>

The provided code of this repository can be compiled to a console application to play Connect Four against CarloConnect offline. Besides the MCTS approach utilized for CarloConnect, the code further contains a Monte Carlo Search (MCS) approach.

For further information about the game of Connect Four, the MCS, the MCTS, the implementation and the strength of CarloConnect, see my [thesis](https://carloconnect.eu/pinheiro-torres_vogt_thesis_20.pdf).


## Configuration
The **main.cpp** file is the main file of the program. All configurations must be done there. By default, the computer's moves are calculated with the MCTS having a computing time limit of 1000 ms. To switch between the applied AI (MCS and MCTS), see *Line 17*, to modify the time limit, see *Line 20*.


## Compilation
In order to compile the code, the main file must be compiled. Therefore, solely a C++ compiler suffice. With the Windows PowerShell/Command Prompt or a Linux based Terminals the **main.cpp** can be compiled using g++ ([Windows](http://www.mingw.org/wiki/getting_started), [Linux](https://linuxconfig.org/how-to-install-g-the-c-compiler-on-ubuntu-18-04-bionic-beaver-linux)):

```sh
$ g++ main.cpp -std=c++14
```

Alternatively, the files of this repository can be added to project folders of C++ IDEs like VisualStudio or CLion and be run by compiling the **main.cpp** file inside the IDE.

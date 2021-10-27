# Caissa.
Caissa is a chess engine created specifically for chess variations. Unlike most chess engines, Caissa aims to make a lot of variables commonly used in chess variants easily configurable, this includes things like board size, win conditions, piece count and so on. Caissa is NOT meant to be the highest rated nor the fastest chess engine, it is build entirely for the purpose of creating AI opponents for chess variants.

# Board representation.
Caissa uses a "Square Centric" board representation, meaning that we store data about non-occupied squares. I chose to go with a square centric board representation, instead of a piece centric board representation, since it makes it easier to add extra functionality to empty squares.

# Developer resources.
Versioning system: https://semver.org/

# Building.
1. `cd Caissa/build/"`
2. `cmake ../ -G"MinGW Makefiles"`
3. `make`
4. `./Caissa.exe`
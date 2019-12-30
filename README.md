
## Project Name: 
_Poker AI_

## About:
C++ Poker Implementation that uses multithreading to train the AI against itself. 
The AI uses a decision tree like structure to store weights. These weights are used to calculate what decision the AI should make in a particular situation._


## Instructions:
The C++ code stores players or computer players in a list. Main creates many games in a loop so when it is run it will play many games simultaneously against itself._


1. ### How to compile:
	This code was written and compiled on Ubuntu. There is a shell script called make.sh that compiles it using g++._

2. ### How to execute:
	Run poker.exe then follow menu instructions._

3. ### Expected output:
	If the game is set to include a human player they will see a text based poker game. If not they will see an output of many games 	 happening at once_

4. ### Known issues:
	The decision tree stucture could be improved. The methods for the AI to evaluate it's position could be improved. Updating the decision nodes is slow. There is 73,200 decision nodes currently but if I used more details to make decisions the number would increase multiplicatively. Occasionaly it crashes for large numbers of games played in a single run, but it has not crashed for smaller numbers like ~100 games._
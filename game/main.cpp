/*
 * @file    main.cpp
 * @brief   poker
 * @date    10/24/2019
 */

#include "game.h"
#include <list>
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
#include <ctime>
#include <random>
#include <string>

int main ()
{
    srand(time(0));
    cout << "\nWelcome\n";
    int numPlayers = 0;
    int choice = 0;
    cout << "Machine Learning: [1]\n"
         << "Player Mode:      [2]\n";
    while(!choice)
    {
        cout << "Choice: ";
        cin >> choice;
        if (choice == 1){
            int gamesToPlay = 0;
            while(!gamesToPlay){
                cout << "How many games? [1-1000]\n";
                cin >> gamesToPlay;
                if(gamesToPlay > 0 && gamesToPlay < 1000){
                    #pragma omp parallel
                    #pragma omp for
                    for(int i = 0; i < gamesToPlay; i++){   //begin parallel machine learning
                        std::cerr << "Game " << i << endl;
                        Game g;
                    }
                }
                else{
                    gamesToPlay = 0;
                    cin.clear();
                    std::string trash;
                    getline(cin, trash);
                    cout << "Invalid input!\n";
                }
            }
           
        }
        else if(choice == 2){
            while(!numPlayers){
                cout << "How many computer players? [1-5]\n";
                cin >> numPlayers;
                if(numPlayers > 0 && numPlayers < 6)    //begin player game
                    Game g{false, numPlayers+1};
                else{
                    numPlayers = 0;
                    cin.clear();
                    std::string trash;
                    getline(cin, trash);
                    cout << "Invalid input!\n";
                }
            }
        }
        else{
            choice = 0;
            cin.clear();
            std::string trash;
            getline(cin, trash);
            cout << "Invalid input!\n";
        }
    }
    return 0;
}

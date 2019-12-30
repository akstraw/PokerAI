#include "decisionTree.h"
#include <chrono>
#include <random>
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
#include <unistd.h>
#include <utility>
#include <fstream>
    using std::ofstream;
    using std::ifstream;
    //handChance

void DecisionTree::read()
{
    //indexed by [round(eg. deal or turn)] [# of opponents - 1] [then last 3 represent foldCallRaise/foldOrCall/checkOrRaise]
    {
        ifstream fin{"ai/dealTreeWeights.txt"};
        for(int highCard = 0; highCard < TOTALRANKS; highCard++){
            for(int pot = 0; pot < POTPERCENTSECTIONS; pot++)
            {
                for(int opponents = 0; opponents < MAXOPPONENTS; opponents++)
                {
                    for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                        dealTree[highCard][pot][opponents][optionSet] = std::move(std::unique_ptr<DecisionTreeNode>{new DecisionTreeNode{fin}});
                    }
                }
            }
        }
        fin.close();
        fin.open("ai/flopTreeWeights.txt");
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            flopTree[hand][highCard][pot][opponents][optionSet] = std::move(std::unique_ptr<DecisionTreeNode>{new DecisionTreeNode{fin}});
                        }
                    }
                }
            }
        }
        fin.close();
        fin.open("ai/turnTreeWeights.txt");
        for(int hole = 0; hole < HOLETURN; hole++){
            for(int hand = 0; hand < NUMHANDS; hand++){
                for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                    for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                        for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                            for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                                turnTree[hole][hand][highCard][pot][opponents][optionSet] = std::move(std::unique_ptr<DecisionTreeNode>{new DecisionTreeNode{fin}});
                            }
                        }
                    }
                }
            }
        }
        fin.close();
        fin.open("ai/riverTreeWeights.txt");
        for(int hole = 0; hole < HOLERIVER; hole++){
            for(int hand = 0; hand < NUMHANDS; hand++){
                for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                    for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                        for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                            for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                                riverTree[hole][hand][highCard][pot][opponents][optionSet] = std::move(std::unique_ptr<DecisionTreeNode>{new DecisionTreeNode{fin}});
                            }
                        }
                    }
                }
            }
        }
        fin.close();
    }
}
//adds other trees weights to this one. Used at the end of a game to add to the stored tree
void DecisionTree::sum(DecisionTree& decTree)
{
    for(int highCard = 0; highCard < TOTALRANKS; highCard++){
        for(int pot = 0; pot < POTPERCENTSECTIONS; pot++)
        {
            for(int opponents = 0; opponents < MAXOPPONENTS; opponents++)
            {
                for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                    dealTree[highCard][pot][opponents][optionSet]->add(*decTree.dealTree[highCard][pot][opponents][optionSet]);
                }
            }
        }
    }
    for(int hand = 0; hand < NUMHANDS; hand++){
        for(int highCard = 0; highCard < TOTALRANKS; highCard++){
            for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                    for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                        flopTree[hand][highCard][pot][opponents][optionSet]->add(*decTree.flopTree[hand][highCard][pot][opponents][optionSet] );
                    }
                }
            }
        }
    }
    for(int hole = 0; hole < HOLETURN; hole++){
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            turnTree[hole][hand][highCard][pot][opponents][optionSet]->add(*decTree.turnTree[hole][hand][highCard][pot][opponents][optionSet] );
                        }
                    }
                }
            }
        }
    }
    for(int hole = 0; hole < HOLERIVER; hole++){
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            riverTree[hole][hand][highCard][pot][opponents][optionSet]->add(*decTree.riverTree[hole][hand][highCard][pot][opponents][optionSet] );
                        }
                    }
                }
            }
        }
    }
}
//resets weights to 0. Uses for keeping track of decisions made throughout a game
void DecisionTree::resetWeights()
{
    for(int highCard = 0; highCard < TOTALRANKS; highCard++){
        for(int pot = 0; pot < POTPERCENTSECTIONS; pot++)
        {
            for(int opponents = 0; opponents < MAXOPPONENTS; opponents++)
            {
                for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                    dealTree[highCard][pot][opponents][optionSet]->resetWeights();
                }
            }
        }
    }
    for(int hand = 0; hand < NUMHANDS; hand++){
        for(int highCard = 0; highCard < TOTALRANKS; highCard++){
            for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                    for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                        flopTree[hand][highCard][pot][opponents][optionSet]->resetWeights();
                    }
                }
            }
        }
    }
    for(int hole = 0; hole < HOLETURN; hole++){
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            turnTree[hole][hand][highCard][pot][opponents][optionSet]->resetWeights();
                        }
                    }
                }
            }
        }
    }
    for(int hole = 0; hole < HOLERIVER; hole++){
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            riverTree[hole][hand][highCard][pot][opponents][optionSet]->resetWeights();
                        }
                    }
                }
            }
        }
    }
}
//overwrites this decision tree to the files
void DecisionTree::write()
{
        ofstream fout{"ai/dealTreeWeights.txt"};
        for(int highCard = 0; highCard < TOTALRANKS; highCard++){
            for(int pot = 0; pot < POTPERCENTSECTIONS; pot++)
            {
                for(int opponents = 0; opponents < MAXOPPONENTS; opponents++)
                {
                    for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                        dealTree[highCard][pot][opponents][optionSet]->write(fout);
                    }
                }
            }
        }
        fout.close();
        fout.open("ai/flopTreeWeights.txt");
        for(int hand = 0; hand < NUMHANDS; hand++){
            for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                    for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                        for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                            flopTree[hand][highCard][pot][opponents][optionSet]->write(fout);
                        }
                    }
                }
            }
        }
        fout.close();
        fout.open("ai/turnTreeWeights.txt");
        for(int hole = 0; hole < HOLETURN; hole++){
            for(int hand = 0; hand < NUMHANDS; hand++){
                for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                    for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                        for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                            for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                                turnTree[hole][hand][highCard][pot][opponents][optionSet]->write(fout);
                            }
                        }
                    }
                }
            }
        }
        fout.close();
        fout.open("ai/riverTreeWeights.txt");
        for(int hole = 0; hole < HOLERIVER; hole++){
            for(int hand = 0; hand < NUMHANDS; hand++){
                for(int highCard = 0; highCard < TOTALRANKS; highCard++){
                    for(int pot = 0; pot < POTPERCENTSECTIONS; pot++){
                        for(int opponents = 0; opponents < MAXOPPONENTS; opponents++){
                            for(int optionSet = 0; optionSet < POSSIBLEOPTIONSETS; optionSet++){
                                riverTree[hole][hand][highCard][pot][opponents][optionSet]->write(fout);
                            }
                        }
                    }
                }
            }
        }
        fout.close();
}

#include "decisionTreeNode.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

DecisionTreeNode::DecisionTreeNode(std::istream& fin)
{
    std::string line;
    getline(fin, line);
    std::stringstream linestream{line};
    char buf;
    linestream >> buf;
    if (buf != '('){
        std::cerr << "error: expecting '('\n";
        exit(1);
    }
    //gather decision type and weights
    linestream >> buf;
    decisionType = static_cast<DecisionType>(static_cast<int> (buf - 48));
    for(int i = 0; i < NUMWEIGHTS; i++)
        linestream >> weights[i];
    //calculate proportions
    long long leftAndRight = weights[LEFT] + weights[RIGHT];
    for(int i = 0; i <= RIGHT; i++){
        proportions[i] = weights[i] * 100.0 / leftAndRight;
        if (proportions[i] < 1) proportions[i] = 1;
        if (proportions[i] > 99) proportions[i] = 99;
    }
    //end this node and check for more nodes
    linestream >> buf;
    if (buf != ')'){
        std::cerr << "error: expecting ')'\n";
        exit(1);
    }
    if(decisionType == FOLDORNOT){   //if not a purely terminal decision
        nextDecision = {std::move(std::unique_ptr<DecisionTreeNode>{new DecisionTreeNode{linestream}})};
    }
}
bool DecisionTreeNode::write(std::ofstream& fout)
{
    
    fout << "( ";
    fout << decisionType << " ";
    for(long w : weights){
        fout << w << " ";
    }
    fout << ") ";
    if(nextDecision.get())
        nextDecision->write(fout);
    else
        fout << std::endl;
    fout.flush();
    return true;
}
//set weights to 0
void DecisionTreeNode::resetWeights()
{
    for(int i = 0; i < NUMWEIGHTS; i++)
        weights[i] = 0;
    if(nextDecision)
        nextDecision->resetWeights();
}
//returns list of new weight totals
void DecisionTreeNode::add (DecisionTreeNode &n2)
{
    for(int i = 0; i < NUMWEIGHTS; i++){
        weights[i] = (weights[i] + n2.weights[i]);
        if (weights[i] <= 0)
            weights[i] = 100;
    }
    bool oversized = false;
    for(long& weight : weights){
        if (weight > 1500000) oversized = true;
    }
    if(oversized){
        for(int i = 0; i < NUMWEIGHTS; i++){
            weights[i] /= 2;
            if (weights[i] <= 0)
                weights[i] = 100;
        }
    }
}
DecisionTreeNode::~DecisionTreeNode()
{
    if(nextDecision){
        if(nextDecision->nextDecision)
        {
            nextDecision->nextDecision.reset();
        }
        nextDecision.reset();
    }
}
//
// Created by spl211 on 31/10/2020.
//
#include <fstream>
#include <iostream>
#include <vector>
#include "Graph.h"
#include <Session.h>
#include "json.hpp"


using json = nlohmann::json;
using namespace std;
using json = nlohmann::json;


//need to finish initialization list
Session::Session(const std::string &path): g({}), treeType(), agents(){
    std::ifstream jsonRead(path);
    json jsonParser = json::parse(jsonRead);
    vector <pair<string, int>> agents1 = jsonParser["agents"];
    vector<vector<int>> graph1 = jsonParser["graph"];
    string tree1 = jsonParser["tree"];

    g= graph1;
    if(tree1 == "M"){

    }
    else{}

    std::queue<int> infectionQueue;

    for (auto& elem: jsonParser["agents"]) {
        if(elem[0]=="C"){
            Agent* agent= new ContactTracer();
            agents.push_back(agent);
        } else{
            Agent* agent=new Virus(this,elem[1]);
            agents.push_back(agent);
        }

    }


}

Session &Session::operator=(const Session &oth) {
    if (this == &oth) {
        return *this;
    } else {
        g = oth.g;
        treeType=oth.treeType;
        infectionQueue=oth.infectionQueue;
        clearAgents();
        agents.clear();
        for (auto agent : oth.agents) {
            agents.push_back(agent->clone());
        }
        return *this;
    }
}

Session::Session(Session &&oth): g(oth.g),treeType(oth.treeType),infectionQueue(oth.infectionQueue),agents(move(oth.agents))  {}

Session::~Session() {
    clearAgents();
}

void Session::simulate() {
    while (!g.isChainBreak()){
        int cycleSize=agents.size();
        for(int i=0; i < cycleSize;i++){
            agents[i]->act(this);
        }
    }
}

void Session::addAgent(const Agent &agent) {
   Agent* clone = agent.clone();
   agents.push_back(clone); //*** todo check if we need to create new agents vector
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

void Session::enqueueInfected(int i) {
    infectionQueue.push(i);
}

int Session::dequeueInfected() {
    if (!infectionQueue.empty()) {
        return infectionQueue.back();
    }
    return -1;
}

TreeType Session::getTreeType() {
    json jsonFile;
    string jsonTreeType = jsonFile.at("tree");

    if (jsonTreeType == "C") {
        return Cycle;
    }
    else if (jsonTreeType == "M") {
        return MaxRank;
    }
    else if (jsonTreeType == "R") {
        return Root;
    }
}


Graph Session::getGraph() {
    return g;
}

void Session::clearAgents() {
    for (auto & agent : agents) {
        delete agent;
    }
}





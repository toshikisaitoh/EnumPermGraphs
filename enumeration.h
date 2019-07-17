#pragma once

#include "dm_english.h"
#include "permutation.h"

#include <iostream>
#include <fstream>
#include <string>

extern std::ofstream ofs;
extern std::ofstream Log;
extern std::string fname;

extern bool flag_connected;

void enumPermutationGraphs(const int n);
graphe removeEdge(graphe G, int u, int v);
Permutation makeParent(Permutation p);
void makeChildren(Permutation p);

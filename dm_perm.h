#pragma once

#include <tuple>

#include "./dm_english.h"
#include "./permutation.h"

std::pair<Permutation, int> createPermutation(noeud *v);
Permutation createPermutation(noeud *v, graphe GG);

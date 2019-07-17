#include <set>

#include "./enumeration.h"

#include "./dm_perm.h"

bool bflag = false;


graphe removeEdge(const graphe G, const int u, const int v){
  graphe G_e;
  G_e.n = G.n;
  G_e.G=(adj **)malloc(G.n*sizeof(adj *));
  for(int i = 0 ; i<G.n ; i++){
    G_e.G[i] = NULL;
    for(adj *a = G.G[i] ; a != NULL ; a = a->suiv){
      if((i == u && a->s == v) || (i==v && a->s == u)) continue;
      adj *e = (adj *) malloc(sizeof(adj));
      e->s = a->s;
      e->suiv = G_e.G[i];
      G_e.G[i] = e;
    }
  }
  return G_e;
}


Permutation makeParent(const Permutation p){
  Permutation pp = p.parentPermutation();
  //pp.print();
  
  //Creategraphe from p_Ge
  graphe Gp = pp.makeGraphe();
  noeud *root = decomposition_modulaire(Gp);
  //print(Gp);
  //printarbre(root);
  
  pp = createPermutation(root, Gp);
  
  return pp;
}



void enumPermutationGraphs(const int n){
  Permutation p(n,SERIE);
  p.print(ofs);
  makeChildren(p);
}


void makeChildren(const Permutation p){
  std::set<Permutation> st;

  graphe G = p.makeGraphe();
  for(int i=0 ; i<G.n ; i++){
    for(adj *a = G.G[i] ; a != NULL ; a = a->suiv){
      if(a->s < i) continue; // the edge has already checked.
      
      graphe G_e = removeEdge(G, i, a->s);
      noeud *root_e = decomposition_modulaire(G_e);
      
      if (flag_connected && root_e->type == PARALLELE) continue;   // 連結なものだけがほしければ，ここでチェック．
      
      Permutation p_Ge = createPermutation(root_e, G_e);
      if (!p_Ge.is_permutation()) continue; // G_e is not a permutation graph.
      
      auto itr = st.find(p_Ge);
      if(itr != st.end()) continue; // G_e has already stored in the set.

      Permutation pp = makeParent(p_Ge);

      if(p == pp){
	p_Ge.print(ofs);
	st.insert(p_Ge);
	makeChildren(p_Ge);
      }
    }
  }
  /*
  if(!st.empty()){
    Log << "Parent: ";
    p.print(Log);
    print(p.makeGraphe());
    for(auto itr = st.begin(); itr != st.end(); ++itr) {
      Log << "Output: ";
      itr->print(Log);
      print(itr->makeGraphe());
    }
    Log << std::endl;
  }
  */
}

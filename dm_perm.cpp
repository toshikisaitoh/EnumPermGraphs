#include <algorithm>
#include <utility>
#include <map>
#include <fstream>

#include "./dm_perm.h"

extern std::ofstream Log;
extern bool bflag;

using namespace std;


graphe G;


void print(graphe G){
  //std::cout << "Number of vertices: " << std::endl;
  for(int i=0 ; i<G.n ; i++){
    Log << i << ": ";
    for(adj *a = G.G[i] ; a != NULL ; a = a->suiv){
      Log << a->s << ", ";
    }
    Log << std::endl;
  }
}

Permutation createPermutation(noeud *v, graphe GG){
  G = GG;
  std::pair<Permutation, int> p = createPermutation(v);
  return p.first;
}


vector< Permutation > vsymmetry(vector<Permutation> &vec )
{
  if(bflag){ Log << "Symmetry!" << std::endl;}
  // 反転すべきかを以下で判別
  //for(int i=0 ; i<vec.size() ; i++){
  for(int i=0 ; i<vec.size()/2 ; i++){
    if(bflag){
      Log << "vec[" << i << "]: ";
      vec[i].print(Log);
      Log << "vec[" << vec.size()-i-1 << "]: ";
      vec[vec.size()-i-1].print(Log);
    }
// 	cerr << "com : " << i << " <->" << vec.size() - 1 - i << endl;
    if(vec[i] == vec[ vec.size() - 1 - i ] ){
      if(bflag) Log << "Equal ---- continue" << std::endl;
      continue; // 対称位置にある順列が等しい
    }
    
	// 以下は等しくないので，確定．
	if(vec[ vec.size() - 1 - i ] < vec[i]){ // 反転する場合
		if(bflag) Log << "Reverse!" << std::endl;
		// 反転
		for(int j=0 ; j<vec.size()/2 ; j++){
// 			cerr << j << " <->" << vec.size() - 1 - j << endl;
			std::swap(vec[j], vec[ vec.size() - 1 - j ]);
// 			std::swap(r[j], r[ vec.size() - 1 - j ]);
		}
	}
	if(bflag) Log << "Done!" << std::endl;
	break;
	// 反転する場合も，そうでない場合もループを抜ける．（反転しない場合は何もしない．）
  }

  return vec;
}

vector< Permutation > hsymmetry( vector< Permutation > &vec, const Permutation &p )
{
	auto vec2( vec );

	for ( int i = 0; i < p.length(); ++i )
	{
		vec2[ p.perm[i] - 1 ] = vec[i];
	}

	return min( vec, vec2 );
}

vector< Permutation > rsymmetry(vector<Permutation> &vec, const Permutation &p )
{
	vector< Permutation > rot( vec );
	for ( int i = 0; i < p.length(); ++i )
	{
		rot[ p.length() - p.pi( i ) ] = vec[i];
	}

	return min( vec, rot );
}


graphe makeGraph(map<int, int> mp, vector<int> represent){
  graphe qG;
  qG.n = represent.size();
  qG.G=(adj **)malloc(qG.n*sizeof(adj *));
  for(int i=0 ; i < qG.n ; i++) qG.G[i] = NULL;
  
  for(int i=0 ; i < qG.n ; i++){
    qG.G[i] = NULL;
    for(adj *a = G.G[represent[i]] ; a != NULL ; a = a->suiv){
      auto itr = mp.find(a->s);
      if( itr != mp.end() ) {
	//設定されている場合の処理
	adj *edge = (adj *) malloc(sizeof(adj));
	edge->s = itr->second;
	edge->suiv=qG.G[i];
	qG.G[i] = edge;
      }
    }
  }
  return qG;
}


// The input is (the root of) modular decomposition 
std::pair<Permutation, int> createPermutation(noeud *v){
// #define D_CREATE_PERMUTATION

  if (v->type == FEUILLE) return std::make_pair(Permutation(1), v->nom);

  vector<Permutation> vec;
  vector<int> represent;
  
  fils *ffils = v->fils;
  //noeud *nfils;
  int count = 0;

  // 子の permutation を再帰的に計算
  // vec にそれらの permutation を子の順序どおりに格納
  while(ffils != NULL){
    std::pair<Permutation, int> pp = createPermutation(ffils->pointe);
    if(!pp.first.is_permutation()) return pp;
    vec.push_back(pp.first);
    represent.push_back(pp.second);
    v->nom = pp.second;
    count++;
    ffils = ffils->suiv;
  }

  Permutation p;
  switch(v->type){
  case PARALLELE:
//     if(bflag) Log << "Parallel" << std::endl;
//     cerr << "Parallel" << std::endl;
    p.identify(count);
	sort( begin( vec ), end( vec ) );
    break;
  case SERIE:
//     if(bflag) Log << "Series" << std::endl;
//     cerr << "Series" << std::endl;
    p.reverse(count);
	sort( begin( vec ), end( vec ) );
    break;
  case PREMIER:  // for a prime module.
//     if(bflag) Log << "Prime" << std::endl;
// 	cerr << "Prime" << std::endl;
    // represent[];（represent な頂点集合）


    std::map<int, int> mp;
    for(int i=0  ; i<represent.size() ; i++){
      mp[represent[i]] = i;
    }

    // construct the quotient graph using representation.
    graphe qG = makeGraph(mp, represent);

    //Log << count << std::endl;
    //print(qG);
    p.prime(qG);
    if(!p.is_permutation()) return std::make_pair(p, -1);
    //p.print(Log);
	
    // vec に格納されている permutation の順序を対応されたものに入れ替える．
    std::vector<Permutation> tmp;
//     for(auto itr = p.label.begin() ; itr != p.label.end() ; ++itr){
//       tmp.push_back( vec[ p.inverse[ *itr-1] - 1 ] );
//     }
	for ( int i = 0; i < vec.size(); ++i )
	{
		const int kth_repr = p.label[ p.perm[i] - 1 ] - 1;
		tmp.push_back( vec[ kth_repr ] );
	}
    vec = tmp;

    // symmetricity check
	if ( p.is_v_symmetry() && p.is_h_symmetry() )
	{
		vec = min( { vec, vsymmetry( vec ), hsymmetry( vec, p ), rsymmetry( vec, p ) } );
	}
	else if (p.is_v_symmetry() )
	{
		vec = vsymmetry(vec );
	}
	else if ( p.is_h_symmetry() )
	{
		vec = hsymmetry( vec, p );
	}
	else if (p.is_rotational_symmetry() )
	{
		vec = rsymmetry(vec, p);
	}

    
    break;
  } // prime 処理はここまで．

  // 子の permutation (vec) をマージ
  if(bflag){
    Log << "Before merge: ";
    p.print(Log);
    for(auto itr = vec.begin() ; itr != vec.end() ; ++itr){
      itr->print(Log);
    }
  }

  p.computeInverse();
  p.merge(vec);
  
  if(bflag){
    Log << "After merge: ";
    p.print(Log);
  }

  return std::make_pair(p, v->nom);
}


#ifdef DM
int main(){
  int n = 6;
  /*
  int edgesU[] = {0,0,1,1,1,2,2,3};
  int edgesV[] = {1,2,2,3,4,3,4,4};
  int M = 8;
  */
  int edgesU[] = {0,0,0,0,0,1,2,2,3,4};
  int edgesV[] = {1,2,3,4,5,2,3,4,4,5};
  int M = 10;

  G.n=n;
  G.G=(adj **)malloc(n*sizeof(adj *));
  for(int i=0 ; i<n ; i++) G.G[i]=NULL;

  for(int k=0 ; k<M ; k++){
    int i = edgesU[k];
    int j = edgesV[k];
    // ajoute j a l'adjacence de i
    adj *a=(adj *)malloc(sizeof(adj));
    a->s=j;
    a->suiv=G.G[i];
    G.G[i]=a;
    // et reciproquement
    a=(adj *)malloc(sizeof(adj));
    a->s=i;
    a->suiv=G.G[j];
    G.G[j]=a;
  }

  noeud *root = decomposition_modulaire(G);
  printarbre(root);

  std::pair<Permutation, int> pp = createPermutation(root);
  pp.first.print();
  
  //cout << node->type << endl;
}
#endif

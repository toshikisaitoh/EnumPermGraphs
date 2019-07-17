#include <assert.h>
#include <unistd.h>

#include <sstream>
//#include <boost/algorithm/string.hpp>
#include <algorithm> // for is_permutation
#include <tuple>
#include <functional>
#include <iterator>

#include "./permutation.h"

#include "./dm_english.h"
#include "./dm_perm.h"
#include "./sagemath_alter.h"

Permutation::Permutation(){}
  
Permutation::Permutation(const int n)
{
	if(n==-1) non_perm();
	else identify(n);
}
  
Permutation::Permutation(const std::vector<int> vec)
{
	std::vector<int>::const_iterator it;
	for(it = vec.begin() ; it != vec.end() ; ++it){
		perm.push_back(*it);
	}
	label.resize( length() );
	computeInverse();
// 	canonization();
}

// construct a canonical permutation for G (induced by modules)
// for parallel, series, prime modules. 
Permutation::Permutation(const int n, const ModuleType type)
{
	switch(type){
	case FEUILLE:
		identify(1);
		break;
		      /*
		    case PREMIER:
			// This case will not be reached
		      prime(G);
		      break;
		      */
	case PARALLELE:
		identify(n);
		break;
	case SERIE:
		reverse(n);
		break;
	default:
		std::cerr << "Error! Strange the module type.\n\n";
	}
}

  
  // For a prime module.
  // ここで sagemath を使っている．
  // sagemath を使わなければ，1000倍は早くなるはず．
void Permutation::prime(const graphe G)
{
	std::vector< std::vector< int > > adj_matrix( G.n, std::vector< int >( G.n ) );
	for ( int i = 0; i < G.n; ++i )
	{
		for ( adj* adj_list = G.G[i]; adj_list != NULL; adj_list = adj_list->suiv )
		{
			const int j = adj_list->s;
			adj_matrix[i][j] = adj_matrix[j][i] = 1;
		}
	}

	std::vector< int > vec1, vec2;
	std::tie( vec1, vec2 ) = is_permutation_certificate( adj_matrix );
	if ( vec1.empty() )
	{
		non_perm();
		return;
	}

// #define D_PRIME

	label = vec1;
	// 正規化（一方の permutation を(1, 2, 3, ..., n)に変換）し，perm に格納
	std::vector<int> v1(vec1.size());
	for(int i=0 ; i<vec1.size() ; i++){
		v1[vec1[i]-1] = i+1;
	}
	for(int i=0 ; i<vec2.size() ; i++){
		perm.push_back(v1[vec2[i]-1]);
	}
	// -----（ここまで）出力データから二つの permutation を読み込んで正規化 -----

	canonization(); // 標準形に変換

}

  
void Permutation::identify(const int n)
{
	for(int i=0 ; i<n ; i++){
		perm.push_back(i+1);
	}
	computeInverse();
}

  
void Permutation::reverse(const int n)
{
	for(int i=n ; i>0 ; i--){
		perm.push_back(i);
	}
	computeInverse();
}

  
void Permutation::non_perm(){
	std::vector<int>().swap(perm);
	perm.push_back(-1);
}

  /******************************************/

void Permutation::canonization()
{
	std::vector<int> vecH = computeHorizontalFlip();
	std::vector<int> vecV = computeVerticalFlip();
	std::vector<int> vecR = computeRotation();

	computeInverse();

	if(is_SmallerEqual(vecH) && is_SmallerEqual(vecV) && is_SmallerEqual(vecR)){
		return;
	}

	if(is_small(vecH, vecV)){ // checked vecH < vecV
		if(is_small(vecH, vecR)){ // vecH < vecR
			perm = vecH;
			computeHorizontalFlip_label();
		}
		else{ // vecR < vecH
			perm = vecR;
			computeRotation_label();
		}
	}
	else{ // vecV < vecH
		if(is_small(vecV, vecR)){ // vecV < vecR
			perm = vecV;
			computeVerticalFlip_label();
		}
		else{ // vecR < vecV
			perm = vecR;
			computeRotation_label();
		}
	}

	computeInverse();
}

void Permutation::computeInverse()
{
	inverse.resize( length() );
	for ( int i = 0; i < length(); ++i )
	{
		inverse[ perm[i] - 1 ] = i + 1;
	}
	return;
}

void Permutation::computeHorizontalFlip_label()
{
// 	cerr << "H-flip" << endl;
	std::vector<int> vec(length());
	for(int i=0 ; i<length() ; i++){
		vec[pi(i)-1] = label[i];
// 		cerr << i << " -> " << perm[i] << endl;
	}
	label = vec;
}
  
std::vector<int> Permutation::computeHorizontalFlip() const
{
	std::vector<int> vec(length());
	for(int i=0 ; i<length() ; i++){
		vec[pi(i)-1] = i+1;
	}
	//perm = vec;
	return vec;
}

void Permutation::computeVerticalFlip_label()
{
	std::vector<int> vec(length());
	for(int i=0 ; i<length() ; i++){
		vec[length()-i-1] = label[i];
	}
	label = vec;
}

std::vector<int> Permutation::computeVerticalFlip() const
{
	std::vector<int> vec(length());
	for(int i=0 ; i<length() ; i++){
		vec[length()-i-1] = length() - pi(i) + 1;
	}
	//perm = vec;
	return vec;
}

std::vector<int> Permutation::computeVerticalFlip(std::vector<int> p) const
{
	assert(p.size()==length());
	std::vector<int> vec(length());
	for(int i=0 ; i<length() ; i++){
		vec[length()-i-1] = length() - p[i] + 1;
	}
	//perm = vec;
	return vec;
}

void Permutation::computeRotation_label()
{
	vector< int > vec( length() );
	for ( int i = 0; i < length(); ++i )
	{
		vec[ length() - inverse[i] ] = label[i];
	}
	label = vec;

// 	computeHorizontalFlip_label();
// 	computeVerticalFlip_label();
}
    
std::vector<int> Permutation::computeRotation() const
{
	std::vector<int> vecH = computeHorizontalFlip();
	std::vector<int> vec = computeVerticalFlip(vecH);
	return vec;
}

bool Permutation::is_canonical()
{
	Permutation p(perm);
	p.canonization();
	if(is_Smaller(p.perm)) return true;
	else return false;
}

bool Permutation::is_v_symmetry() const
{
	for(int i=0 ; i<length()/2 ; i++){
		if (perm[i] != length() - perm[length()-i-1] + 1){
// 			cerr << "i : " << i << endl;
// 			cerr << "perm[i] : " << perm[i] << endl;
// 			cerr << "length() - perm[length()-i-1] + 1) : " << length() - perm[length()-i-1] + 1 << endl;
			return false;
		}
	}
	return true;
}

bool Permutation::is_h_symmetry() const
{
	for ( int i = 0; i < length(); ++i )
	{
		if ( i != perm[ perm[i] - 1 ] - 1 )
		{
			return false;
		}
	}
	return true;
}

bool Permutation::is_rotational_symmetry() const
{
	std::vector<int> vecR = computeRotation();
	return is_equal(vecR);
}
  
bool Permutation::is_permutation()
{
	//std::cout << perm[0] << std::endl;
	if(perm[0] == -1) return false;
	else return true;
}

int Permutation::length() const
{
	return perm.size();
}

int Permutation::pi(int k) const
{
	return perm[k];
}

bool Permutation::operator==(const Permutation& p) const
{
	if(length() != p.length()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] != p.pi(i)) return false;
	}
	return true;
}

bool Permutation::operator==(const std::vector<int>& vec) const
{
	if(length() != vec.size()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] != vec[i]) return false;
	}
	return true;
}

bool Permutation::is_equal(const std::vector<int>& vec) const
{
	if(length() != vec.size()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] != vec[i]) return false;
	}
	return true;
}

bool Permutation::operator<(const Permutation& p) const {

	if(length() < p.length()) return true;
	if(length() > p.length()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] < p.pi(i)) return true;
		if(perm[i] > p.pi(i)) return false;
	}
	return false;
}

bool Permutation::is_Smaller(const std::vector<int>& vec) const
{
	if(length() < vec.size()) return true;
	if(length() > vec.size()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] < vec[i]) return true;
		if(perm[i] > vec[i]) return false;
	}
	return false;
}

bool Permutation::is_SmallerEqual(const std::vector<int>& vec) const
{
	if(length() < vec.size()) return true;
	if(length() > vec.size()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] < vec[i]) return true;
		if(perm[i] > vec[i]) return false;
	}
	return true;
}


bool Permutation::is_Smaller(const Permutation& p) const
{
	if(length() < p.length()) return true;
	if(length() > p.length()) return false;

	for(int i=0 ; i<length() ; i++){
		if(perm[i] < p.pi(i)) return true;
		if(perm[i] > p.pi(i)) return false;
	}
	return false;
}

bool Permutation::is_small(const std::vector<int> v1, const std::vector<int> v2) const
{
	if(v1.size() < v2.size()) return true;
	if(v1.size() > v2.size()) return false;

	for(int i=0 ; i<v1.size() ; i++){
		if(v1[i] < v2[i]) return true;
		if(v1[i] > v2[i]) return false;
	}
	return false;
}

bool Permutation::is_smallequal(const std::vector<int> v1, const std::vector<int> v2) const
{
	if(v1.size() < v2.size()) return true;
	if(v1.size() > v2.size()) return false;

	for(int i=0 ; i<v1.size() ; i++){
		if(v1[i] < v2[i]) return true;
		if(v1[i] > v2[i]) return false;
	}
	return true;
}

// for Prime module
void Permutation::merge(const std::vector<Permutation> p)
{

	vector< vector< int > > perms;
	for ( int i = 0; i < p.size(); ++i )
	{
		perms.push_back( p[i].perm );
	}

	vector< int > psum( length() + 1 );
	for ( int i = 0; i < length(); ++i )
	{
		psum[ i + 1 ] = perms[i].size();
	}
	//partial_sum( begin( psum ), end( psum ), begin( psum ) );

	for ( int i = 0, s = 0; i < length(); s += perms[ inverse[ i++ ] - 1 ].size() )
	{
		// 		cerr << "perms[ " << inverse[i] - 1 << " ]" << endl;
		// 		cerr << s << endl;
		transform( begin( perms[ inverse[i] - 1 ] ), end( perms[ inverse[i] - 1 ] ), begin( perms[ inverse[i] - 1 ] ), bind( plus< int >(), placeholders::_1, s ) );
	}

	vector< int > vec;
	for ( const auto &pp : perms )
	{

		copy( begin( pp ), end( pp ), back_inserter( vec ) );
// 				copy( begin( pp ), end( pp ), ostream_iterator< int >( cerr, " " ) );
// 				cerr << endl;
	}

	perm = vec;
	label.resize( perm.size() );
}

Permutation Permutation::parentPermutation() const
{
	std::vector< int > vec( perm );
	for ( int i = 0; i + 1 < length(); ++i )
	{
		if ( vec[i] < vec[ i + 1 ] )
		{
			std::swap( vec[i], vec[ i + 1 ] );
			break;
		}
	}

	Permutation p(vec);
// 	cerr << "parent permutation from : ";
// 	p.print();
	graphe G = p.makeGraphe();
	noeud *root = decomposition_modulaire( G );
	Permutation pp = createPermutation( root, G );
	return pp;
}

graphe Permutation::makeGraphe() const
{
	graphe G;
	G.n = length();
	G.G=(adj **)malloc(G.n*sizeof(adj *));
	for(int i=0 ; i<G.n ; i++) G.G[i] = NULL;

	for(int i=0 ; i<length() ; i++){
		for(int j=i+1 ; j<length() ; j++){
			if(perm[i] > perm[j]){
				// add an edge perm[i] to perm[j] (and its reversal).
				adj *a = (adj *)malloc(sizeof(adj));
				a->s = perm[j]-1;
				a->suiv = G.G[perm[i]-1];
				G.G[perm[i]-1] = a;

				a = (adj *)malloc(sizeof(adj)); a->s = perm[i]-1;
				a->suiv = G.G[perm[j]-1];
				G.G[perm[j]-1] = a;

			}
		}
	}
	return G;
}
  
void Permutation::print() const
{
	if(length()<1){
		std::cout << "length 0" << std::endl;
		return;
	}

	std::cout << "( ";
	std::vector<int>::const_iterator it;
	for(it = perm.begin() ; it != perm.end() ; ++it){
		std::cout << *it << " ";
	}
	std::cout << ")" << std::endl;
}

    
void Permutation::print(std::ofstream &ofs) const
{
	if(length()<1){
		ofs << "length 0" << std::endl;
		return;
	}

	ofs << "( ";
	std::vector<int>::const_iterator it;
	for(it = perm.begin() ; it != perm.end() ; ++it){
		ofs << *it << " ";
	}
	ofs << ")" << std::endl;
}

void Permutation::print(std::ofstream &ofs, std::vector<int> vec) const
{
	if(vec.size()<1){
		ofs << "length 0" << std::endl;
		return;
	}

	ofs << "( ";
	std::vector<int>::const_iterator it;
	for(it = vec.begin() ; it != vec.end() ; ++it){
		ofs << *it << " ";
	}
	ofs << ")" << std::endl;
}


void Permutation::print(std::vector<int> vec)
{
	std::cout << "( ";
	for(int i=0 ; i<vec.size() ; i++){
		std::cout << vec[i] << " ";
	}
	std::cout << ")" << std::endl;
}

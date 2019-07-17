#include <bits/stdc++.h>

#include "./sagemath_alter.h"

using namespace std;
using namespace placeholders;

using LL = long long;
using ULL = unsigned long long;
using VI = vector< int >;
using VVI = vector< vector< int > >;
using VS = vector< string >;
using ISS = istringstream;
using OSS = ostringstream;
using PII = pair< int, int >;
using VPII = vector< pair< int, int > >;
template < typename T = int > using VT = vector< T >;
template < typename T = int > using VVT = vector< vector< T > >;
template < typename T = int > using LIM = numeric_limits< T >;

template < typename T > inline istream& operator>>( istream &s, vector< T > &v ){ for ( T &t : v ) { s >> t; } return s; }
template < typename T > inline ostream& operator<<( ostream &s, const vector< T > &v ){ for ( int i = 0; i < int( v.size() ); ++i ){ s << ( " " + !i ) << v[i]; } return s; }

void in_impl(){};
template < typename T, typename... TS > void in_impl( T &head, TS &... tail ){ cin >> head; in_impl( tail ... ); }
#define IN( T, ... ) T __VA_ARGS__; in_impl( __VA_ARGS__ );

template < typename T > struct getv_fmt;
template <> struct getv_fmt<       int >{ static constexpr const char *fmt = "%d"; };
template <> struct getv_fmt< long long >{ static constexpr const char *fmt = "%lld"; };
template < typename T > void getv( std::vector< T > &v ){ for_each( begin( v ), end( v ), []( T &a ){ scanf( getv_fmt< T >::fmt, &a ); } ); };

template < typename T > inline T fromString( const string &s ) { T res; istringstream iss( s ); iss >> res; return res; }
template < typename T > inline string toString( const T &a ) { ostringstream oss; oss << a; return oss.str(); }

#define NUMBERED( name, number ) NUMBERED2( name, number )
#define NUMBERED2( name, number ) name ## _ ## number
#define REP1( n ) REP2( NUMBERED( REP_COUNTER, __LINE__ ), n )
#define REP2( i, n ) REP3( i, 0, n )
#define REP3( i, m, n ) for ( int i = ( int )( m ); i < ( int )( n ); ++i )
#define GET_REP( a, b, c, F, ... ) F
#define REP( ... ) GET_REP( __VA_ARGS__, REP3, REP2, REP1 )( __VA_ARGS__ )
#define FOR( e, c ) for ( auto &&e : c )
#define ALL( c ) begin( c ), end( c )
#define AALL( a ) ( remove_all_extents< decltype( a ) >::type * )a, ( remove_all_extents< decltype( a ) >::type * )a + sizeof( a ) / sizeof( remove_all_extents< decltype( a ) >::type )
#define DRANGE( c, p ) begin( c ), begin( c ) + ( p ), end( c )

#define SZ( v ) ( (int)( v ).size() )
#define EXIST( c, e ) ( ( c ).find( e ) != ( c ).end() )

template < typename T > inline bool chmin( T &a, const T &b ){ if ( b < a ) { a = b; return true; } return false; }
template < typename T > inline bool chmax( T &a, const T &b ){ if ( a < b ) { a = b; return true; } return false; }

#define PB push_back
#define EM emplace
#define EB emplace_back
#define BI back_inserter

#define MP make_pair
#define fst first
#define snd second

#define DUMP( x ) cerr << #x << " = " << ( x ) << endl

class UnionFind
{
private:
	const int N;

	vector<int> parent;
	vector<int> rank;

	int groups_;
	std::vector< int > sizes;

public:
	UnionFind( int n ) : N( n ), parent( N ), rank( N, 0 ), groups_( N ), sizes( N, 1 )
	{
		std::iota( parent.begin(), parent.end(), 0 );

		return;
	}

	int find( int x )
	{
		if ( parent[x] == x )
		{
			return x;
		}
		return parent[x] = find( parent[x] );
	}

	bool same( int x, int y )
	{
		return find( x ) == find( y );
	}

	bool unite( int x, int y )
	{
		x = find( x );
		y = find( y );

		if ( x == y )
		{
			return false;
		}

		if ( rank[x] < rank[y] )
		{
			parent[x] = y;
			sizes[y] += sizes[x];
		}
		else
		{
			parent[y] = x;
			sizes[x] += sizes[y];

			if ( rank[x] == rank[y] )
			{
				++rank[x];
			}
		}

		--groups_;
		return true;
	}

	int groups() const
	{
		return groups_;
	}

	int groupSize( const int x )
	{
		return sizes[ find( x ) ];
	}
};
// UnionFind( N )
// find( x )
// same( x, y )
// unite( x, y )
// groups()
// groupSize( x )

// 二部グラフ判定
// O( |V| + |E| )
class IsBipartite
{
private:
	const int N;

	vector< vector<int> > G;
	vector<int> colors;

public:
	IsBipartite( const int n ) : N( n ), G( n ), colors( N ) {};

	void connect( const int u, const int v )
	{
		if ( u == v )
		{
			return;
		}

		G[u].push_back( v );
		G[v].push_back( u );

		return;
	}

	bool solve()
	{
		fill( colors.begin(), colors.end(), -1 );

		for ( int u = 0; u < N; ++u )
		{
			if ( colors[u] == -1 && !paint( u, 1 ) )
			{
				return false;
			}
		}

		return true;
	}

	bool color( const int v ) const
	{
		return colors[v];
	}

private:
	bool paint( const int u, const int c )
	{
		if ( colors[u] != -1 )
		{
			return colors[u] == c;
		}

		colors[u] = c;

		for ( const int v : G[u] )
		{
			if ( !paint( v, 1 - c ) )
			{
				return false;
			}
		}

		return true;
	}
};
// IsBipartite( N )
// connect( a, b )
// solve()
// color( v ) // required to be solved

bool is_connected( const vector< vector< int > > &G )
{
	const int N = G.size();

	UnionFind uf( N );
	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < i; ++j )
		{
			if ( G[i][j] )
			{
				uf.unite( i, j );
			}
		}
	}

	return uf.groups() == 1;
}

bool is_comparability( const vector< vector< int > > &G )
{
	const int N = G.size();

	UnionFind uf( N * N );
	const auto unite = [&]( const int a, const int b, const int x, const int y )
	{
		return uf.unite( a * N + b, x * N + y );
	};
	const auto same = [&]( const int a, const int b, const int x, const int y )
	{
		return uf.same( a * N + b, x * N + y );
	};

	for ( int u = 0; u < N; ++u )
	{
		for ( int v1 = 0; v1 < N; ++v1 )
		{
			for ( int v2 = v1 + 1; v2 < N; ++v2 )
			{
				if ( G[u][ v1 ] && G[u][ v2 ] && !G[ v1 ][ v2 ] )
				{
					unite( u, v1, u, v2 );
					unite( v1, u, v2, u );
				}
			}
		}
	}

	for ( int u = 0; u < N; ++u )
	{
		for ( int v = u + 1; v < N; ++v )
		{
			if ( same( u, v, v, u ) )
			{
				return false;
			}
		}
	}
	return true;
}

bool is_permutation( const vector< vector< int > > &G )
{
	const int N = G.size();

	auto G_c = G;
	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < i; ++j )
		{
			G_c[i][j] = G_c[j][i] = !G[i][j];
		}
	}

	return is_comparability( G ) && is_comparability( G_c );
}

vector< int > vertical_flip( const vector< int > &permutation )
{
	const int N = permutation.size();

	vector< int > result( N );
	for ( int i = 0; i < N; ++i )
	{
		result[ permutation[i] ] = i;
	}
	return result;
}

vector< int > horizontal_flip( const vector< int > &permutation )
{
	const int N = permutation.size();

	vector< int > result( N );
	for ( int i = 0; i < N; ++i )
	{
		result[ N - 1 - i ] = N - 1 - permutation[i];
	}
	return result;
}

vector< int > rotate( const vector< int > &permutation )
{
	return vertical_flip( horizontal_flip( permutation ) );
}

vector< int > normalize_permutation( const vector< int > &permutation )
{
	return min( { permutation,
			vertical_flip( permutation ),
			horizontal_flip( permutation ),
			rotate( permutation ) } );
}

bool is_total_ordering( const VVI &G )
{
	const int N = SZ( G );
	REP( i, N )
	{
		REP( j, i )
		{
			if ( !( G[i][j] || G[j][i] ) )
			{
				cerr << "!( G[i][j] || G[j][i] ) )" << endl;
				cerr << i << ' ' << j << endl;
				FOR( row, G )
				{
					DUMP( row );
				}
				return false;
			}
		}
	}
	REP( k, N )
	{
		REP( i, N )
		{
			REP( j, N )
			{
				if ( i == k || j == k )
				{
					continue;
				}

				if ( G[i][k] * G[k][j] )
				{
					if ( !G[i][j] )
					{
						DUMP( G[i][k] );
						DUMP( G[k][j] );
						DUMP( G[i][j] );
						cerr << i << ' ' << j << " via " << k << endl;
						FOR( row, G )
						{
							DUMP( row );
						}
						return false;
					}
				}
			}
		}
	}
	return true;
}

vector< vector< int > > is_comparability_certificate( vector< vector< int > > G )
{
	const int N = G.size();

	UnionFind uf( N * N );
	const auto unite = [&]( const int a, const int b, const int x, const int y )
	{
		return uf.unite( a * N + b, x * N + y );
	};
	const auto find = [&]( const int a, const int b )
	{
		return uf.find( a * N + b );
	};
	const auto same = [&]( const int a, const int b, const int x, const int y )
	{
		return find( a, b ) == find( x, y );
	};

	vector< pair< int, int > > edges;
	while ( true )
	{
		vector< vector< int > > E( N );
		for ( int i = 0; i < N; ++i )
		{
			for ( int j = 0; j < N; ++j )
			{
				if ( G[i][j] )
				{
					E[i].push_back( j );
				}
			}
		}

// 		FOR( row, G )
// 		{
// 			DUMP( row );
// 		}
// 		cout << endl;

		const int u = find_if( begin( E ), end( E ), []( const vector< int > &es ){ return !es.empty(); } ) - begin( E );
		if ( u == N )
		{
			break;
		}
		const int v = E[u][0];

		vector< pair< int, int > > to_remove;

// 		cerr << "equilvarent class with ( " << u << ", " << v << " )" << endl;
		// TODO: THe following line should be replaced (use of function< T > is not recommended
		const function< void( int, int ) > dfs = [&]( const int x, const int y )
		{
			to_remove.emplace_back( x, y );

			vector< int > vertices;
			merge( begin( E[x] ), end( E[x] ), begin( E[y] ), end( E[y] ),
					back_inserter( vertices ) );

			to_remove.emplace_back( x, y );
			for ( const int &w : vertices )
			{
				if ( w == x || w == y )
				{
					continue;
				}

				if ( G[x][w] && !G[y][w] && !same( x, y, x, w ) )
				{
					unite( x, y, x, w );
// 					unite( y, x, w, x );
// 					cerr << x << ' ' << y << " : " << x << ' ' << w << endl;
					to_remove.emplace_back( x, w );
					dfs( x, w );
				}
				if ( G[w][y] && !G[x][w] && !same( x, y, w, y ) )
				{
					unite( x, y, w, y );
// 					unite( y, x, y, w );
// 					cerr << y << ' ' << x << " : " << y << ' ' << w << endl;
					to_remove.emplace_back( w, y );
					dfs( w, y );
				}
			}
			return;
		};
		dfs( u, v );
		dfs( v, u );

		for ( const auto &e : to_remove )
		{
			const int u = e.first;
			const int v = e.second;
			G[u][v] = G[v][u] = 0;
			edges.emplace_back( u, v );
		}
	}

	IsBipartite coloring( N * N );
// 	cerr << "construct bipartite graph" << endl;
	for ( int u = 0; u < N; ++u )
	{
		for ( int v = u + 1; v < N; ++v )
		{
			if ( find( u, v ) == find( v, u ) )
			{
				return {};
			}
// 			cerr << find( u, v ) << ' ' << find( v, u ) << endl;
			coloring.connect( find( u, v ), find( v, u ) );
		}
	}

	if ( !coloring.solve() )
	{
// 		cerr << "not a comparability" << endl;
		return {};
	}

	vector< vector< int > > ordering( N, vector< int >( N ) );
// 	sort( begin( edges ), end( edges ) );
// 	edges.erase( unique( begin( edges ), end( edges ) ), end( edges ) );
// 	cout << "construct partial ordering" << endl;
	for ( const auto &e : edges )
	{
		const int u = e.first;
		const int v = e.second;

		ordering[u][v] = coloring.color( find( u, v ) );
		ordering[v][u] = coloring.color( find( v, u ) );
// 		if ( coloring.color( find( u, v ) ) )
// 		{
// 			cout << u << ' ' << v << endl;
// 		}
// 		if ( coloring.color( find( v, u ) ) )
// 		{
// 			cout << v << ' ' << u << endl;
// 		}
	}
// 	cout << endl;

	return move( ordering );
}

pair< vector< int >, vector< int > > is_permutation_certificate( const vector< vector< int > > G )
{
	const int N = G.size();

	auto G_c = G;
	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < i; ++j )
		{
			G_c[i][j] = G_c[j][i] = !G[i][j];
		}
	}

	if ( !is_connected( G ) || !is_connected( G_c ) )
	{
// 		cerr << "given grpah is not a parime" << endl;
		assert( false );
		return {};
	}

// 	cout << "TEST" << endl;
	const auto porder_G = is_comparability_certificate( G );
// 	cout << "TEST" << endl;
	const auto porder_G_c = is_comparability_certificate( G_c );

// 	cout << "TEST" << endl;
// 	FOR( row, porder_G )
// 	{
// 		DUMP( row );
// 	}
// 	cout << endl;
// 	FOR( row, porder_G_c )
// 	{
// 		DUMP( row );
// 	}
// 	cout << endl;

	if ( porder_G.empty() || porder_G_c.empty() )
	{
// 		cerr << "not permutation" << endl;
// 		assert( false );
		return {};
	}

// 	cerr << "partial orderings" << endl;
// 	FOR( row, porder_G )
// 	{
// 		DUMP( row );
// 	}
// 	cerr << endl;
// 	FOR( row, porder_G_c )
// 	{
// 		DUMP( row );
// 	}

	vector< vector< int > > ordering1( porder_G_c ), ordering2( porder_G_c );
	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < N; ++j )
		{
			if ( porder_G[i][j] )
			{
				ordering1[i][j] = 1;
				ordering2[j][i] = 1;
			}
		}
	}

	assert( is_total_ordering( ordering1 ) );
	assert( is_total_ordering( ordering2 ) );

// 	cerr << "oderings" << endl;
// 	FOR( row, ordering1 )
// 	{
// 		cerr << row << endl;
// 	}
// 	cerr << endl;
// 	FOR( row, ordering2 )
// 	{
// 		cerr << row << endl;
// 	}
// 	cerr << endl;

	const auto create_permutation = [&]( const auto &ordering )
	{
		vector< int > p( N );
		iota( begin( p ), end( p ), 0 );
		sort( begin( p ), end( p ),
				[&]( const int i, const int j ){ return ordering[i][j]; } );
		return move( p );
	};

	auto p1 = create_permutation( ordering1 );
	auto p2 = create_permutation( ordering2 );
	reverse( ALL( p1 ) );
	reverse( ALL( p2 ) );

// 	DUMP( p1 );
// 	DUMP( p2 );

// 	vector< int > maps_to( N );
// 	for ( int i = 0; i < N; ++i )
// 	{
// 		maps_to[ p1[i] ] = i;
// 	}
// 	DUMP( maps_to );
// 	transform( begin( p2 ), end( p2 ), begin( p2 ), [&]( const int a ){ return maps_to[a]; } );
	transform( begin( p1 ), end( p1 ), begin( p1 ), bind( plus< int >(), placeholders::_1, 1 ) );
	transform( begin( p2 ), end( p2 ), begin( p2 ), bind( plus< int >(), placeholders::_1, 1 ) );
	return { move( p1 ), move( p2 ) };

// 	vector< int > maps_to( N );
// 	for ( int i = 0; i < N; ++i )
// 	{
// 		maps_to[ p1[i] ] = i;
// 	}
// 	DUMP( maps_to );
// 	transform( begin( p2 ), end( p2 ), begin( p2 ), [&]( const int a ){ return maps_to[a]; } );
// 	p2 = normalize_permutation( p2 );
// 	transform( begin( p2 ), end( p2 ), begin( p2 ), bind( plus< int >(), placeholders::_1, 1 ) );
// 	return move( p2 );
}

// void test_permutations()
// {
// 	mt19937 rng;
//
// 	ifstream file( "permutations.txt" );
// 	for ( string s; getline( file, s ) && !s.empty(); )
// 	{
// 		const int length = s.size();
// 		s = s.substr( 2, length - 4 );
// 		istringstream iss( s );
//
// 		const int N = ( s.size() + 1 ) / 2;
// 		vector< int > p2( N );
// 		for ( auto &a : p2 )
// 		{
// 			iss >> a;
// 		}
//
// 		auto op2 = p2;
// 		cout << "original p2 : " << op2 << endl;
//
// 		vector< int > p1( N );
// 		iota( begin( p1 ), end( p1 ), 1 );
// 		{
// 			vector< int > maps_to( N );
// 			iota( begin( maps_to ), end( maps_to ), 1 );
// 			shuffle( begin( maps_to ), end( maps_to ), rng );
//
// 			for ( int i = 0; i < N; ++i )
// 			{
// 				p1[i] = maps_to[ p1[i] - 1 ];
// 				p2[i] = maps_to[ p2[i] - 1 ];
// 			}
// 		}
//
// 		cout << "shuffled p1 : " << p1 << endl;
// 		cout << "shuffled p2 : " << p2 << endl;
//
// 		vector< vector< int > > G( N, vector< int >( N ) );
// 		for ( int i = 0; i < N; ++i )
// 		{
// 			for ( int j = i + 1; j < N; ++j )
// 			{
// 				const int p1i = find( begin( p1 ), end( p1 ), i + 1 ) - begin( p1 );
// 				const int p1j = find( begin( p1 ), end( p1 ), j + 1 ) - begin( p1 );
// 				const int p2i = find( begin( p2 ), end( p2 ), i + 1 ) - begin( p2 );
// 				const int p2j = find( begin( p2 ), end( p2 ), j + 1 ) - begin( p2 );
// 				if ( ( p1i - p1j ) * ( p2i - p2j ) < 0 )
// 				{
// 					G[i][j] = G[j][i] = 1;
// 					cout << i + 1 << ' ' << j + 1 << endl;
// 				}
// // 				if ( ( p1[i] - p1[j] ) * ( p2[i] - p2[j] ) < 0 )
// // 				{
// // 					G[i][j] = G[j][i] = 1;
// // 					cout << i + 1 << ' ' << j + 1 << endl;
// // 				}
// 			}
// 		}
//
// 		if ( !is_permutation( G ) )
// 		{
// 			cerr << "False negative for a permutation graph" << endl;
// 			exit( -1 );
// 		}
//
// 		const auto res = is_permutation_certificate( G );
// 		if ( res.fst.empty() )
// 		{
// 			cerr << endl;
// 			continue;
// 		}
//
// 		cout << "obtained p2 : " << res.snd << endl;
//
// 		transform( begin( op2 ), end( op2 ), begin( op2 ), bind( minus< int >(), placeholders::_1, 1 ) );
// 		op2 = normalize_permutation( op2 );
// 		transform( begin( op2 ), end( op2 ), begin( op2 ), bind( plus< int >(), placeholders::_1, 1 ) );
// 		DUMP( op2 );
//
// 		assert( op2 == res );
// 		cerr << endl;
//
// 		return;
// 	}
//
// 	return;
// }
//
// void test_random_graphs( const int N )
// {
// 	mt19937 rng( 711 );
// 	uniform_int_distribution< int > rng_NN( N * ( N / 10 ), N * ( N - 1 ) / 2 );
//
// 	REP( 100 )
// 	{
// 		VT< PII > edges;
// 		REP( i, N )
// 		{
// 			REP( j, i )
// 			{
// 				edges.EB( i, j );
// 			}
// 		}
//
// 		shuffle( ALL( edges ), rng );
// 		edges.resize( rng_NN( rng ) );
//
// 		VVI G( N, VI( N ) );
// 		FOR( e, edges )
// 		{
// 			const int u = e.fst;
// 			const int v = e.snd;
// 			G[u][v] = G[v][u] = 1;
// 		}
//
// // 		cout << "is_permutation() : " << is_permutation( G ) << endl;
// // 		if ( is_permutation( G ) )
// // 		{
// // 			FOR( e, edges )
// // 			{
// // 				cout << e.fst << ' ' << e.snd << endl;
// // 			}
// // 		}
// 	}
//
// 	return;
// }

// void print_permutations( const pair< VI, VI > &ps )
// {
// 	cout << ps.fst << endl;
// 	cout << ps.snd << endl;
// }
//
// int main()
// {
// 	VVI G( {
// 			{ 0, 0, 0 },
// 			{ 0, 0, 1 },
// 			{ 0, 1, 0 }
// 			} );
// 	print_permutations( is_permutation_certificate( G ) );
//
// 	return 0;
// }

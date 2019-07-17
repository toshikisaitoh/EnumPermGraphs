#include "enumeration.h"

#include <ctime>

using namespace std;

std::ofstream ofs;
std::ofstream Log;

std::string fname;

bool flag_connected = false;

int main(int argc, char** argv){
// 	test();

  if(argc < 2){
    cout << "Please input the number of vertices. " << endl;
    exit(-1);
  }

  std::string filename;
  std::string logfilename;

  if(argc>2 && argv[2][0]=='c'){
    filename = string("connected_");
    logfilename = string("connected_");
    fname = string("c");
    flag_connected = true;
  }
  filename += string("perm") + string(argv[1]) + string(".dat");
  logfilename += string("perm") + string(argv[1]) + string("_log.dat");
  fname += string(argv[1]) + string("prime.dat");

  ofs.open(filename);
  //Log.open(logfilename);
  
  //std::cout << "from sage.graphs.comparability import is_permutation" << std::endl;

  clock_t clock_start = clock();
  
  enumPermutationGraphs(atoi(argv[1]));

  std::cout << "Enumeration is completed." << std::endl;
  std::cout << "Elapsed time : " <<  static_cast< double >( clock() - clock_start ) / CLOCKS_PER_SEC << std::endl;

  return 0;
}

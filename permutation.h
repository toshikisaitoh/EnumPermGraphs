#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dm_english.h"
#include "./permutation.h"

extern std::ofstream Log;
extern std::string fname;
extern bool bflag;

static long prev_time = 0;

/*
#ifndef DEBUG_PERM
#define DEBUG_PERM
enum ModuleType{
  prime, parallel, series,
};
#endif
*/

typedef int ModuleType;

class Permutation{
// private:
	public:
  std::vector<int> perm;
  std::vector<int> label, inverse;
  // label[i] means number of corresponding vertex


public:
  Permutation();
  Permutation(const int n);
  Permutation(const std::vector<int> vec);

  // construct a canonical permutation for G (induced by modules)
  // for parallel, series, prime modules. 
  Permutation(const int n, const ModuleType type);

  // For a prime module.
  void prime(const graphe G);
  
  void identify(const int n);
  void reverse(const int n);
  
  void non_perm();

  /******************************************/

  void canonization();
  void computeInverse();

  void computeHorizontalFlip_label();
  
  std::vector<int> computeHorizontalFlip() const;

  void computeVerticalFlip_label();

  std::vector<int> computeVerticalFlip() const;
  std::vector<int> computeVerticalFlip(std::vector<int> p) const;

  void computeRotation_label();
  std::vector<int> computeRotation() const;

  /******************************************/
  
  
  bool is_canonical();
  bool is_v_symmetry() const;
  bool is_h_symmetry() const;
  bool is_rotational_symmetry() const;
  bool is_permutation();

  int length() const;
  int pi(int k) const;

  bool operator==(const Permutation& p) const;
  bool operator==(const std::vector<int>& vec) const;
  bool operator<(const Permutation& p) const;
  bool is_equal(const std::vector<int>& vec) const;
  bool is_Smaller(const std::vector<int>& vec) const;
  bool is_SmallerEqual(const std::vector<int>& vec) const;
  bool is_Smaller(const Permutation& p) const;
  bool is_small(const std::vector<int> v1, const std::vector<int> v2) const;
  bool is_smallequal(const std::vector<int> v1, const std::vector<int> v2) const;

  // for Prime module
  void merge(const std::vector<Permutation> p);

  Permutation parentPermutation() const;

  graphe makeGraphe() const;
  
  void print() const;
  void print(std::ofstream &ofs) const;
  void print(std::ofstream &ofs, std::vector<int> vec) const;
  void print(std::vector<int> vec);
  
};

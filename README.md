# Enumeration of permutation graphs

Th programs are implementation of an algorithm which enumerates all permutation graphs up to isomorphism in [1]. Our programs are written in C++. compiling the programs by `make` in the command line and, you get the execution file 'main.exe'. You run 'main.exe' with an integer as a command line argument, and then obtain a 'permX.dat' file. For example, if you type './main.exe 4' in the command line, you can obtain 'perm4.dat' file and a content of the file is as follows. 
```
( 4 3 2 1 )
( 4 3 1 2 )
( 4 1 3 2 )
( 1 4 3 2 )
( 4 1 2 3 )
( 1 4 2 3 )
( 2 4 1 3 )
( 2 1 4 3 )
( 1 2 4 3 )
( 1 2 3 4 )
( 3 4 1 2 )
```
Permutations represent permutation diagrams of permutation graphs. Each permutation is a canonical form of a permutation graph, that is, the permutations have one to one correspondence to the permutation graphs. 

We use an implementation of modular decomposition which is developped by Fabien de Montgolfier. You can see the program as [here](https://github.com/vbraun/graph-modular-decomposition). 


## References
[1] [Kazuaki Yamazaki, Toshiki Saitoh, Masashi Kiyomi, and Ryuhei Uehara: “Enumeration of nonisomorphic interval graphs and nonisomorphic permutation graphs,” Theoretical Computer Science, to appear.](https://www.sciencedirect.com/science/article/pii/S0304397519303056)
[2] [Ryuhei Uehara: Graph Catalogs, http://www.jaist.ac.jp/~uehara/graphs/](http://www.jaist.ac.jp/~uehara/graphs/)
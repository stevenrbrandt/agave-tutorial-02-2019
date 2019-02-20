#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

void usage() {
  std::cerr << "usage: drawgau input.txt" << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  if(argc != 2)
    usage();
  ifstream i(argv[1]);

  if(!i.good())
    usage();

  MPI_Init(&argc,&argv);
  int rank,size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  double minx, maxx, delx;
  i >> minx >> maxx >> delx;
  if(rank == 0) {
    cout << "minx=" << minx << endl;
    cout << "maxx=" << maxx << endl;
    cout << "delx=" << delx << endl;
    cout << "mpi size=" << size << endl;
  }

  stringstream s;
  s << "drawgau" << rank << ".txt";
  string fname = s.str();
  cout << "FILE: " << fname << endl;
  ofstream f(fname,ofstream::out);
  double delxp = (maxx - minx)/size;
  double minxp = minx+delxp*rank;
  double maxxp = minxp + delxp;
  cout << "rank=" << rank << " minxp=" << minxp << " maxxp=" << maxxp << endl;
  for(double x = minxp; x < maxxp; x += delx) {
    f << x << " " << exp(-x*x) << endl;
  }
  f.close();
  MPI_Finalize();
  return 0;
}
```shell
ssh duanzq@greatlakes.arc-ts.umich.edu
180326LOL5v5,.

module load gcc
module load openmpi

export OMP_NUM_THREADS=4

cd 587proj

g++ omp.cpp -o proj -fopenmp
g++ omp2.cpp -o proj -fopenmp
g++ omp3.cpp -o proj -fopenmp

g++ omp_dfs.cpp -o proj -fopenmp
g++ omp_bfs.cpp -o proj -fopenmp
g++ omp_bfs_dfs.cpp -o proj -fopenmp
g++ omp_bfs_dfs2.cpp -o proj -fopenmp
g++ omp_bfs_dfs_trie.cpp -o proj -fopenmp
g++ omp_bfs_dfs_trie2.cpp -o proj -fopenmp
g++ omp_bfs_dfs_trie3.cpp -o proj -fopenmp

sbatch startup.sh
sbatch startup2.sh
sbatch startup3.sh


cat proj.stdout
cat proj_time.txt
```

https://stackoverflow.com/questions/34141909/optimizing-n-queen-with-openmp-c

https://stackoverflow.com/questions/19078635/optimizing-n-queen-with-openmp

```c++
serial improvement:
1. using prefix tree (Trie)
2. use dfs instead of bfs

openmp improvement:
1. copying board each time, so no board lock
2. using char[][] instead of vector<<vector>char> to speed up when copying!!!
3. pass the pointer instead of shallow copy
  
serial bfs + parallel dfs
shuffle the queue
```


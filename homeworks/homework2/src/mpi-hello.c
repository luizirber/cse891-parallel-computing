/*
 * Copywrite (C) 2002-2004 Greg M. Kurtzer
 *
 * Written for demonstration and testing purposes for Warewulf by
 * Greg Kurtzer <GMkurtzer@lbl.gov>
 *
 * This snippet is released under the terms of copywrite law and the GNU GPL
 * which is the license that the Warewulf distribution is released under.
 *
 * Demonstration of a very basic MPI Hello World program that uses MPI
 * interprocess communication functions so all output to console is from
 * the master process running on the master node.
 *
 * Compile command: mpicc -o mpi-hello mpi-hello.c
 * How to run assuming Open-mpi (or compat):
 * # wwlist > ~/nodes
 * # mpirun -np ?? -hostfile ~/nodes ./mpi-hello
*/

#include <stdio.h>          /* Needed for printf'ing */
#include <stdlib.h>
#include <mpi.h>            /* Get the MPI functions from this header */

#define max_nodes 264       /* Max number of nodes that we should test */
#define str_length 50       /* Largest string that can be used for hostnames */

int main(int argc, char **argv)
{

   /* Declare variables */
   int   proc, rank, size, namelen;
   int   ids[max_nodes];
   char           hostname[str_length][max_nodes];
   char           processor_name[str_length];

   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Get_processor_name(processor_name,&namelen);

   if (rank==0) {
      printf("Hello From: %s I am the recieving processor %d of %d\n",processor_name, rank+1, size);
      for (proc=1;proc<size;proc++) {
         MPI_Recv(&hostname[0][proc],str_length,MPI_INT,proc,1,MPI_COMM_WORLD,&status);
         MPI_Recv(&ids[proc],str_length,MPI_INT,proc,2,MPI_COMM_WORLD,&status);
         printf("Hello From: %-20s I am processor %d of %d\n",&hostname[0][proc], ids[proc]+1, size);
      }
   } else {
      srand(rank);
      int t = rand()%10+1;
      //printf("waiting %d\n",t);
      sleep(t);
      MPI_Send(&processor_name,str_length,MPI_INT,0,1,MPI_COMM_WORLD);
      MPI_Send(&rank,str_length,MPI_INT,0,2,MPI_COMM_WORLD);
   }
   MPI_Finalize();

   return(0);
}


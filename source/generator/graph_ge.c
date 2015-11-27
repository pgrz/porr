#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*** record for qsorting a graph to provide another isomorphic to it ***/
#define RecordSize 24
typedef struct {
   char record[ RecordSize ];
} Record;

void fix_imbalanced_graph( void );
void print_graph( int v,
                  int e,
                  char* out_file,
                  int* adj_matrix,
                  int dir_flag );
void random_graph( int v,
                   int e,
                   int max_wgt,
                   char* out_file );
void seed_ran( void );
int ran( int k );     /* customized random number generator */

/* initialize array a so that a[ i ] = i */

void swap( int* a, int *b ); /* swap two ints */
/*** miscellany ***/
#define True         1
#define False        0
#define None         -999
#define Undirected   0
#define Directed     1
#define ExitProgram  0

/*** graph generators ***/
#define RandomGraph            1
#define RandomDigraph          2
#define RandomConnectedGraph   3
#define DirectedAcyclicGraph   4
#define CompleteGraph          5
#define IsomorphicGraphPair    6
#define HamiltonianCycleGraph  7
#define NetworkFlowGraph       8

/*** warnings ***/
#define IllegalDimensions   "\n\n\tNeither edge nor vertex count may be zero.\n\n"
#define IllegalFileName     "\n\n\tOutput file has illegal name.\n\n"
#define InsufficientStorage "\n\n\tThere is not enough space for this size graph.\n\n"
#define SingleVertexNetwork "\n\n\tA single vertex network is not allowed.\n\n"

/*** prompts ***/
#define ToplevelPrompts  9
static char  *toplevel_prompts[] =
   { " 0. Exit Program ",
     " 1. Random Graph ",
     " 2. Random Digraph ",
     " 3. Random Connected Graph ",
     " 4. Random DAG (Directed Acyclic Graph) ",
     " 5. Random Complete Weighted Graph ",
     " 6. Random Isomorphic Graph Pair ",
     " 7. Random Hamiltonian Cycle Graph ",
     " 8. Random Network "
   };

#define SimpleGraphAns  1
#define SimpleOrNotPrompts  2
static char *simple_or_not_prompts[] =
   { " 1. Simple ",
     " 2. Not Simple "
   };

#define WeightedAns  2
#define WeightedOrNotPrompts  2
static char *weighted_or_not_prompts[] =
   { " 1. Unweighted Edges ",
     " 2. Weighted Edges "
   };

#define EdgeIndex         0
#define VertexIndex       1
#define VerticesOnly      -111
#define EdgesAndVertices  -222
static char *dimension_prompts[] =
   { " Number of Edges:    ",
     " Number of Vertices: "
   };

#define DirectedGraphAns 1
#define DirectedPrompts 2
static char *directed_prompts[] =
   { " 1. Directed Graph ",
     " 2. Undirected Graph "
   };

static char *degree_prompt =  " Degree Per Edge:  ";

#define ConnectedAns  1
#define ConnectedPrompts  2
static char *connected_prompts[] =
   { " 1. Connected Graph ",
     " 2. Unconnected Graph "
   };

static char *main_file_prompt = " File for graph:  ";

#define TopologicalSort      0
#define IsomorphicGraph      1
#define Isomorphism          2
#define HamiltonianCycle     3
static char *file_prompts[] =
   { " File for topological sort:  ",
     " File for isomorphic graph:  ",
     " File for isomorphism:  ",
     " File for Hamiltonian cycle:  "
   };


/*** output files ***/
#define MaxFileName   128
typedef struct out_files {
   char  outfile1[ MaxFileName + 1 ];
   char  outfile2[ MaxFileName + 1 ];
   char  outfile3[ MaxFileName + 1 ];
} Outfile;

/*** graphs ***/
typedef struct graph_parms {
   int      edge_count;
   int      vertex_count;
   int      max_weight;
   int      max_degree;
} GraphParms;

typedef struct graph_props {
   int      simple_p;
   int      weighted_p;
   int      directed_p;
   int      dag_p;
   int      isomorphic_p;
   int      network_p;
} GraphProps;

   GraphProps   props;
   GraphParms   parms;
   Outfile      outfiles;

/* min, max, odd */
#undef min
#define min( x, y )   ((( x ) < ( y )) ? ( x ) : ( y ))

#undef max
#define max( x, y )   ((( x ) > ( y )) ? ( x ) : ( y ))

#define odd( num )    ( ( num ) % 2 )


int main()
{
    seed_ran();

    strcpy(outfiles.outfile1, "asdasd");

     parms.vertex_count = 10;
     parms.edge_count = 20;
     props.simple_p = True;
     props.weighted_p = False;
     parms.max_weight = 20;
    fix_imbalanced_graph();
    random_graph(  parms.vertex_count,
            parms.edge_count,
            parms.max_weight,
            outfiles.outfile1 );
    return 0;
}

void fix_imbalanced_graph( void )
{
   int  max_edges;

   if (  props.simple_p ) {
      max_edges =  parms.vertex_count
                  * (  parms.vertex_count - 1 );
      if ( !  props.directed_p )
         max_edges /= 2;
      if (  parms.edge_count > max_edges )
           parms.edge_count = max_edges;
   }
   else if (  props.dag_p ) {
      max_edges = (  parms.vertex_count
                  * (  parms.vertex_count - 1 ) ) / 2;
      if (  parms.edge_count > max_edges )
          parms.edge_count = max_edges;
   }
   else if (   props.isomorphic_p ) {
      if ( odd( parms.max_degree ) )
         if ( odd(  parms.vertex_count ) )
             parms.vertex_count++;
         if (  parms.vertex_count <=  parms.max_degree )
             parms.vertex_count =  parms.max_degree + 1;
   }
   else if (  props.network_p ) {
      max_edges =
         (  parms.vertex_count *  parms.vertex_count ) -
         (  parms.vertex_count * 3 ) + 3;
      if (  parms.edge_count >= max_edges )
          parms.edge_count = max_edges;
   }
}

/* This function generates a random undirected graph with
   v vertices and e edges.  The graph can be weighted
   (weight_flag == 1) or unweighted (weight_flag != 1). If
   it is weighted, the weights are in the range 1 to max_wgt.
   The graph can be simple (simple_flag == 1) or not simple
   (simple_flag != 1). If the graph is simple, it is assumed
   that e <= v(v-1)/2. (In this program, this assured because
   of the call to fix_imbalanced_graph.)
 */
void random_graph( int v,
                   int e,
                   int max_wgt,
                   char* out_file )
{
   int i, j, count, index, *adj_matrix;

   /* generate a simple random graph */
   if ( ( adj_matrix = ( int * ) calloc( v * v, sizeof( int ) ) )
        == NULL ) {
      printf( "Not enough room for this size graph\n" );
      return;
   }

   for ( count = 0; count < e; ) {
      i = ran( v );
      j = ran( v );

      if ( i == j )
         continue;

      if ( i > j )
         swap( &i, &j );

      index = i * v + j;
      if ( !adj_matrix[ index ] ) {
         adj_matrix[ index ] = 1 + ran( max_wgt );
         count++;
      }
   }

   print_graph( v, e, out_file, adj_matrix, Undirected );

   free( adj_matrix );
}

/*** ran, etc. ***/
void seed_ran( void )
{
   srand( ( unsigned short ) time( NULL ) );
}

/* Return a random integer between 0 and k-1 inclusive. */
int ran( int k )
{
   return rand() % k;
}

void print_graph( int v,
                  int e,
                  char* out_file,
                  int* adj_matrix,
                  int dir_flag )
{
   int i, j, index;
   FILE *fp;

   if ( ( fp = fopen( out_file, "w" ) ) == NULL ) {
      printf( "Unable to open file %s for writing.\n", out_file );
      return;
   }
   printf( "\n\tWriting graph to file %s.\n", out_file );

   fprintf( fp, "%5d   %5d\n", v, e );

   if ( !dir_flag )
      for ( i = 1; i < v; i++ )
         for ( j = i + 1; j <= v; j++ ) {
            index = ( i - 1 ) * v + j - 1;
            if ( adj_matrix[ index ] )
               fprintf( fp, "%5d   %5d   %5d\n", i, j, adj_matrix[ index ] );
         }
   else
      for ( i = 1; i <= v; i++ )
         for ( j = 1; j <= v; j++ ) {
            index = ( i - 1 ) * v + j - 1;
            if ( adj_matrix[ index ] )
               fprintf( fp, "%5d   %5d   %5d\n", i, j, adj_matrix[ index ] );
         }
   fclose( fp );
   printf( "\tGraph is written to file %s.\n", out_file );
}

void swap( int* a, int *b )
{
   int temp;

   temp = *a;
   *a = *b;
   *b = temp;
}

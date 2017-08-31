
/* Computes metrics from faces and tets 
 *
 * Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov 
 */
  


#ifndef GRIDMPI_H

#include "refine_defs.h"
#include "queue.h"
#include "grid.h"

BEGIN_C_DECLORATION

Grid *gridIdentityNodeGlobal(Grid *g, int offset );
Grid *gridSetAllLocal(Grid *g );
Grid *gridSetGhost(Grid *g, int node );

Grid *gridParallelAdapt(Grid *g, Queue *q, 
		       double minLength, double maxLength );
Grid *gridParallelPreProject(Grid *g, Queue *q );
int gridParallelEdgeSplit(Grid *g, Queue *q, int node0, int node1 );
Grid *gridParallelEdgeCollapse(Grid *g, Queue *q, int node0, int node1 );

Grid *gridParallelSmooth(Grid *grid, GridBool localOnly,
			 double optimizationLimit, double laplacianLimit,
                         GridBool smoothOnSurface );
Grid *gridParallelRelaxNegativeCells(Grid *grid, 
				     GridBool localOnly, 
				     GridBool smoothOnSurface );

Grid *gridParallelRelaxNegativeFaceAreaUV(Grid *grid, 
					  GridBool localOnly );

Grid *gridParallelSwap(Grid *grid, Queue *queue, double ARlimit );
Grid *gridParallelEdgeSwap(Grid *g, Queue *q, int node0, int node1 );
Grid *gridApplyQueue(Grid *g, Queue *q );

Grid *gridGhostDataCountByPartition(Grid *g, int total_number_of_partitions,
				    int *partition_data_count);

END_C_DECLORATION

#endif /* GRIDMPI_H */
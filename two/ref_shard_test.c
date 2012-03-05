#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ref_grid.h"
#include "ref_cell.h"
#include "ref_node.h"
#include "ref_list.h"
#include "ref_adj.h"
#include "ref_metric.h"
#include "ref_sort.h"

#include "ref_shard.h"
#include "ref_fixture.h"
#include "ref_quality.h"
#include "ref_subdiv.h"
#include "ref_swap.h"
#include "ref_math.h"
#include "ref_export.h"
#include "ref_edge.h"
#include "ref_dict.h"
#include "ref_mpi.h"

#include "ref_test.h"

static REF_STATUS set_up_hex_for_shard( REF_SHARD *ref_shard_ptr )
{
  REF_GRID ref_grid;

  TSS( ref_fixture_hex_grid( &ref_grid ), "fixure hex" );

  TSS(ref_shard_create(ref_shard_ptr,ref_grid),"create");

  return REF_SUCCESS;
}

static REF_STATUS tear_down( REF_SHARD ref_shard )
{
  REF_GRID ref_grid;

  ref_grid = ref_shard_grid(ref_shard);

  TSS(ref_shard_free(ref_shard),"free");

  TSS( ref_grid_free(ref_grid),"free" );

  return REF_SUCCESS;
}

int main( void )
{

  { /* mark */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TEIS(0,ref_shard_mark(ref_shard,1),"init mark");
    TEIS(0,ref_shard_mark(ref_shard,3),"init mark");

    TSS(ref_shard_mark_to_split(ref_shard,1,6),"mark face for 1-6");

    TEIS(2,ref_shard_mark(ref_shard,1),"split");
    TEIS(0,ref_shard_mark(ref_shard,3),"modified");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* find mark */
    REF_SHARD ref_shard;
    REF_BOOL marked;

    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_to_split(ref_shard,1,6),"mark face for 1-6");

    TSS(ref_shard_marked(ref_shard,2,5,&marked),"is edge marked?");
    TAS(!marked,"pair 2-5 not marked");

    TSS(ref_shard_marked(ref_shard,1,6,&marked),"is edge marked?");
    TAS( marked,"pair 1-6 marked");

    TSS(ref_shard_marked(ref_shard,6,1,&marked),"is edge marked?");
    TAS( marked,"pair 6-1 marked");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark and relax 1-6 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_to_split(ref_shard,1,6),"mark face for 1-6");

    TES(0,ref_shard_mark(ref_shard,3),"no yet");
    TSS(ref_shard_mark_relax(ref_shard),"relax");
    TES(2,ref_shard_mark(ref_shard,3),"yet");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark and relax 0-7 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_to_split(ref_shard,0,7),"mark face for 1-6");

    TES(0,ref_shard_mark(ref_shard,1),"no yet");
    TSS(ref_shard_mark_relax(ref_shard),"relax");
    TES(2,ref_shard_mark(ref_shard,1),"yet");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* relax nothing*/
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_relax(ref_shard),"relax");

    TES(0,ref_shard_mark(ref_shard,0),"marked");
    TES(0,ref_shard_mark(ref_shard,1),"marked");
    TES(0,ref_shard_mark(ref_shard,2),"marked");
    TES(0,ref_shard_mark(ref_shard,3),"marked");
    TES(0,ref_shard_mark(ref_shard,4),"marked");
    TES(0,ref_shard_mark(ref_shard,5),"marked");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark cell edge 0 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,0),"mark edge 0");

    TES(2,ref_shard_mark(ref_shard,1),"face 1");
    TES(2,ref_shard_mark(ref_shard,3),"face 3");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark cell edge 11 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,11),"mark edge 11");

    TES(2,ref_shard_mark(ref_shard,1),"face 1");
    TES(2,ref_shard_mark(ref_shard,3),"face 3");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark cell edge 5 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,5),"mark edge 5");

    TES(3,ref_shard_mark(ref_shard,1),"face 1");
    TES(3,ref_shard_mark(ref_shard,3),"face 3");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* mark cell edge 8 */
    REF_SHARD ref_shard;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,8),"mark edge 8");

    TES(3,ref_shard_mark(ref_shard,1),"face 1");
    TES(3,ref_shard_mark(ref_shard,3),"face 3");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* split on edge 0 */
    REF_SHARD ref_shard;
    REF_GRID ref_grid;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");
    ref_grid = ref_shard_grid(ref_shard);

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,0),"mark edge 0");

    TSS(ref_shard_mark_relax(ref_shard),"relax");
    TSS(ref_shard_split(ref_shard),"split");

    TEIS(0, ref_cell_n(ref_grid_hex(ref_grid)),"remove hex");
    TEIS(2, ref_cell_n(ref_grid_pri(ref_grid)),"add two pri");

    TEIS(4, ref_cell_n(ref_grid_tri(ref_grid)),"add four tri");
    TEIS(4, ref_cell_n(ref_grid_qua(ref_grid)),"keep four qua");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* split on edge 8 */
    REF_SHARD ref_shard;
    REF_GRID ref_grid;
    TSS(set_up_hex_for_shard(&ref_shard),"set up");
    ref_grid = ref_shard_grid(ref_shard);

    TSS(ref_shard_mark_cell_edge_split(ref_shard,0,8),"mark edge 0");

    TSS(ref_shard_mark_relax(ref_shard),"relax");
    TSS(ref_shard_split(ref_shard),"split");

    TEIS(0, ref_cell_n(ref_grid_hex(ref_grid)),"remove hex");
    TEIS(2, ref_cell_n(ref_grid_pri(ref_grid)),"add two pri");

    TEIS(4, ref_cell_n(ref_grid_tri(ref_grid)),"add two pri");
    TEIS(4, ref_cell_n(ref_grid_qua(ref_grid)),"keep one qua");

    TSS( tear_down( ref_shard ), "tear down");
  }

  { /* shard prism */

    REF_GRID ref_grid;

    TSS(ref_fixture_pri_grid(&ref_grid),"set up");

    TSS(ref_shard_prism_into_tet(ref_grid),"shard prism");

    TEIS(0, ref_cell_n(ref_grid_pri(ref_grid)),"no more pri");
    TEIS(3, ref_cell_n(ref_grid_tet(ref_grid)),"into 3 tets");

    TEIS(0, ref_cell_n(ref_grid_qua(ref_grid)),"no more qua");
    TEIS(4, ref_cell_n(ref_grid_tri(ref_grid)),"into 9 tri");

    TSS( ref_grid_free(ref_grid),"free" );
  }

  return 0;
}

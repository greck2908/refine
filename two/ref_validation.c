
#include <stdlib.h>
#include <stdio.h>

#include "ref_validation.h"

#include "ref_face.h"
#include "ref_export.h"

REF_STATUS ref_validation_all( REF_GRID ref_grid )
{

  printf(" hanging nodes?\n");
  RSS( ref_validation_hanging_node( ref_grid ), "hanging node");
  printf(" cell faces?\n");
  RSS( ref_validation_cell_face( ref_grid ), "cell face");
  printf(" muliple boundary faces?\n");
  RSS( ref_validation_multiple_face_cell( ref_grid ), "multiple cell face");

  return REF_SUCCESS;
}

REF_STATUS ref_validation_hanging_node( REF_GRID ref_grid )
{
  REF_INT node;
  REF_BOOL problem;
  REF_ADJ ref_adj;
  REF_CELL ref_cell;
  REF_INT group, cell, node_per, *nodes;

  problem = REF_FALSE;
  each_ref_node_valid_node( ref_grid_node(ref_grid), node )
    {
      if ( ref_adj_empty( ref_cell_adj(ref_grid_tet(ref_grid)), node) &&
	   ref_adj_empty( ref_cell_adj(ref_grid_pyr(ref_grid)), node) &&
	   ref_adj_empty( ref_cell_adj(ref_grid_pri(ref_grid)), node) &&
	   ref_adj_empty( ref_cell_adj(ref_grid_hex(ref_grid)), node) )
	{
	  problem = REF_TRUE;
	  printf(" hanging node %d\n",node);
	}
    }

  ref_adj_create( &ref_adj );

  each_ref_grid_ref_cell( ref_grid, group, ref_cell )
    {
      node_per = ref_cell_node_per(ref_cell);
      nodes = (REF_INT *) malloc( node_per * sizeof(REF_INT) );
      each_ref_cell_valid_cell_with_nodes( ref_cell, cell, nodes )
	{
	  for ( node = 0; node < node_per; node++ )
	    RSS( ref_adj_add( ref_adj, nodes[node], group+4*cell ), "add");
	}
      free(nodes);
    }

  each_ref_node_valid_node( ref_grid_node(ref_grid), node )
    {
      if ( ref_adj_empty( ref_adj, node ) )
	{
	  problem = REF_TRUE;
	  printf(" hanging node %d\n",node);	  
	}
    }

  ref_adj_free(ref_adj);

  return (problem?REF_FAILURE:REF_SUCCESS);
}

REF_STATUS ref_validation_cell_face( REF_GRID ref_grid )
{
  REF_FACE ref_face;
  REF_CELL ref_cell;
  REF_INT *hits;
  REF_INT face;
  REF_INT group, cell, cell_face;
  REF_INT node;
  REF_INT nodes[4];
  REF_BOOL problem;
  REF_STATUS code;

  problem = REF_FALSE;

  RSS( ref_face_create( &ref_face, ref_grid ), "face");

  hits = (REF_INT *)malloc( ref_face_n(ref_face) * sizeof(REF_INT) );
  RNS(hits,"malloc hits NULL");

  for ( face=0; face< ref_face_n(ref_face) ; face++ )
    hits[face]=0;

  each_ref_grid_ref_cell( ref_grid, group, ref_cell )
    each_ref_cell_valid_cell( ref_cell, cell )
      each_ref_cell_cell_face( ref_cell, cell_face )
        {
	  for(node=0;node<4;node++)
	    nodes[node]=ref_cell_f2n(ref_cell,node,cell,cell_face);
	  RSS( ref_face_with( ref_face, nodes, &face ), "find cell face");
	  hits[face]++;
	}
 
  ref_cell = ref_grid_tri( ref_grid );
  each_ref_cell_valid_cell( ref_cell, cell )
    {
      for(node=0;node<3;node++)
	nodes[node]=ref_cell_c2n(ref_cell,node,cell);
      nodes[3]=nodes[0];
      code = ref_face_with( ref_face, nodes, &face );
      if ( REF_SUCCESS != code)
	{
	  ref_node_location( ref_grid_node(ref_grid), nodes[0] );
	  ref_node_location( ref_grid_node(ref_grid), nodes[1] );
	  ref_node_location( ref_grid_node(ref_grid), nodes[2] );
	  ref_node_location( ref_grid_node(ref_grid), nodes[3] );
	}
      RSS( code, "find tri");
      hits[face]++;
    }
 
  ref_cell = ref_grid_qua( ref_grid );
  each_ref_cell_valid_cell( ref_cell, cell )
    {
      for(node=0;node<4;node++)
	nodes[node]=ref_cell_c2n(ref_cell,node,cell);
      RSS( ref_face_with( ref_face, nodes, &face ), "find qua");
      hits[face]++;
    }
 
  for ( face=0; face< ref_face_n(ref_face) ; face++ )
    if ( 2 != hits[face] )
      {
	problem = REF_TRUE;	
	printf(" hits %d\n",hits[face]);	  
      }

  free(hits);

  RSS( ref_face_free( ref_face ), "face free");

  return (problem?REF_FAILURE:REF_SUCCESS);

}

REF_STATUS ref_validation_multiple_face_cell( REF_GRID ref_grid )
{
  REF_CELL ref_cell;
  REF_INT group, cell, cell_face;
  REF_INT node;
  REF_INT nodes[REF_CELL_MAX_NODE_PER];
  REF_BOOL problem;
  REF_INT boundary_faces, found;

  REF_GRID viz;
  REF_INT viz_cell;

  RSS( ref_grid_empty_cell_clone( &viz, ref_grid ), "viz grid" );

  problem = REF_FALSE;

  each_ref_grid_ref_cell( ref_grid, group, ref_cell )
    each_ref_cell_valid_cell( ref_cell, cell )
    {
      boundary_faces = 0;
      each_ref_cell_cell_face( ref_cell, cell_face )
        {
	  for(node=0;node<4;node++)
	    nodes[node]=ref_cell_f2n(ref_cell,node,cell,cell_face);
	  
	  if ( nodes[0] == nodes[3] )
	    {
	      if ( REF_SUCCESS == ref_cell_with( ref_grid_tri( ref_grid ), 
						 nodes, &found ) )
		boundary_faces++;
	    }
	  else
	    {
	      if ( REF_SUCCESS == ref_cell_with( ref_grid_qua( ref_grid ), 
						 nodes, &found ) )
		boundary_faces++;
	    }
	}
      if ( boundary_faces > 1 )
	{
	  problem = REF_TRUE;
	  RSS( ref_cell_nodes( ref_cell, cell, nodes), "cell nodes");
	  RSS( ref_cell_add( ref_grid_cell(viz,group), nodes, &viz_cell), 
	       "add viz cell");
	}
    }

  if ( problem )
    {
      ref_export_tec( viz, "ref_validation_multiple_face_cell.tec" );
      return REF_FAILURE;
    }

  RSS( ref_grid_free_cell_clone( viz ), "free viz");

  return REF_SUCCESS;
}

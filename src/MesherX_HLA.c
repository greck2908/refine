
/* Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov
 */

/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <values.h>
#include "mesherx_hla.h"
#include "CADGeom/CADGeom.h"
#include "Goolache/CAPrIMesh.h"
#include "Goolache/MeshMgr.h"

int main( int argc, char *argv[] )
{

  char   project[256];
  char   outputName[256];
  int    vol=1;
  int    i;
  int    npo;
  int    nel;
  int    *iel;
  double *xyz;
  double scale;
  double scalev;
  int maxnode;
  GridBool mixedElement, blendElement, qualityImprovement, copyGridY;
  GridBool bil;

  sprintf( project,       "" );
  sprintf( outputName,       "" );
  scale = 1.0;
  scalev= 1.0;
  maxnode = 50000;
  mixedElement = FALSE;
  blendElement = FALSE;
  qualityImprovement = FALSE;
  copyGridY = FALSE;
  bil = FALSE;

  i = 1;
  while( i < argc ) {
    if( strcmp(argv[i],"-p") == 0 ) {
      i++; sprintf( project, "%s", argv[i] );
      printf("-p argument %d: %s\n",i, project);

    } else if( strcmp(argv[i],"-o") == 0 ) {
      i++; sprintf( outputName, "%s", argv[i] );
      printf("-o argument %d: %s\n",i, outputName);

    } else if( strcmp(argv[i],"-s") == 0 ) {
      i++; scale = atof(argv[i]);
      printf("-s argument %d: %f\n",i, scale);

    } else if( strcmp(argv[i],"-sv") == 0 ) {
      i++; scalev = atof(argv[i]);
      printf("-sv argument %d: %f\n",i, scalev);

    } else if( strcmp(argv[i],"-n") == 0 ) {
      i++; maxnode = atoi(argv[i]);
      printf("-n argument %d: %d\n",i, maxnode);

    } else if( strcmp(argv[i],"-m") == 0 ) {
      mixedElement = TRUE;
      printf("-m argument %d: activated mixed element layers\n",i);

    } else if( strcmp(argv[i],"-q") == 0 ) {
      qualityImprovement = TRUE;
      printf("-q argument %d: activated grid quality improvement\n",i);

    } else if( strcmp(argv[i],"-b") == 0 ) {
      blendElement = TRUE;
      printf("-b argument %d: activated blend elements\n",i);

    } else if( strcmp(argv[i],"-cy") == 0 ) {
      copyGridY = TRUE;
      printf("-cy argument %d: activated grid copy about y=0 elements\n",i);

    } else if( strcmp(argv[i],"-bil") == 0 ) {
      bil = TRUE;
      printf("-bil argument %d: activated Bil Kleb's case\n",i);

    } else if( strcmp(argv[i],"-vgbg") == 0 ) {
      MeshMgr_SetMeshSizeType( MESH_MGR_VGRID );
      printf("-vgbg argument %d: activated VGRID background spacing\n",i);

    } else if( strcmp(argv[i],"-h") == 0 ) {
      printf("Usage: flag value pairs:\n");
      printf(" -p input project name\n");
      printf(" -o output project name\n");
      printf(" -s scale inviscid background grid\n");
      printf(" -sv scale viscous grid parameters\n");
      printf(" -n maximum number of nodes\n");
      printf(" -m mixed element layers\n");
      printf(" -q use edge swapping to improve grid quality\n");
      printf(" -b use blend elements on first layer\n");
      printf(" -cy copy grid about the y=0 plane\n");
      printf(" -vgbg set background spacing to VGRID\n");
      printf(" -bil Bil Kleb's case\n");
      return(0);
    } else {
      fprintf(stderr,"Argument \"%s %s\" Ignored\n",argv[i],argv[i+1]);
      i++;
    }
    i++;
  }

  if(strcmp(project,"")==0)       sprintf(project,"../test/box1" );
  if(strcmp(outputName,"")==0) {
      sprintf( outputName, "%s_MX", project );
      printf(" %d: %s\n",i, outputName);
  }

  printf("calling MeshMgr_Initialize ... \n");
  if ( ! MeshMgr_Initialize( ) ){
    printf("ERROR: MeshMgr_Initialize broke.\n%s\n",ErrMgr_GetErrStr());
    return 1;
  }  

  printf("calling CADGeom_Start ... \n");
  if ( ! CADGeom_Start( ) ){
    printf("ERROR: CADGeom_Start broke.\n%s\n",ErrMgr_GetErrStr());
    return 1;
  }  

  printf("calling CADGeom_Load for project <%s> ... \n",project);
  if ( ! GeoMesh_LoadPart( project ) ){
    printf("ERROR: GeoMesh_LoadPart broke.\n%s\n",ErrMgr_GetErrStr());
    return 1;
  }


  if ( scale != 1.0 ) {
    MeshMgr_SetElementScale( scale );
    if ( !CAPrIMesh_CreateTShell( vol )) {
      printf("ERROR: could not create shell\n");
      return 0;
    }
  }

  MesherX_DiscretizeVolumeHLA( maxnode, scale, scalev, project, outputName,
			    mixedElement, blendElement, qualityImprovement,
			    bil );

  return(0);
}
    

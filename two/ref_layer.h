
#ifndef REF_LAYER_H
#define REF_LAYER_H

#include "ref_defs.h"

BEGIN_C_DECLORATION
typedef struct REF_LAYER_STRUCT REF_LAYER_STRUCT;
typedef REF_LAYER_STRUCT * REF_LAYER;
END_C_DECLORATION

#include "ref_adj.h"

BEGIN_C_DECLORATION

struct REF_LAYER_STRUCT {
  REF_INT n;
};

REF_STATUS ref_layer_create( REF_LAYER *ref_layer );
REF_STATUS ref_layer_free( REF_LAYER ref_layer );

#define ref_layer_n(ref_layer) ((ref_layer)->n)

END_C_DECLORATION

#endif /* REF_LAYER_H */

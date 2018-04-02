
#include "system.h"
#include "object.h"


objectdata *sphere3_data;
objectdata *sphere2_data;
objectdata *sphere1_data;
objectdata *elus_data;


void set_object_data(void *data_ptr) {

  udword data_address = (udword)data_ptr;


  sphere3_data = (objectdata *)(data_address + 131384);
  sphere2_data = (objectdata *)(data_address + 93112);
  sphere1_data = (objectdata *)(data_address + 54840);
  elus_data = (objectdata *)(data_address + 16568);


  sphere3_data->face_table = (Face *)(data_address + (udword)sphere3_data->face_table);
  sphere3_data->vertex_table = (Vector *)(data_address + (udword)sphere3_data->vertex_table);
  sphere3_data->mapping_table = (MapVector *)(data_address + (udword)sphere3_data->mapping_table);
  sphere3_data->smoothing_table = (udword *)(data_address + (udword)sphere3_data->smoothing_table);
  sphere3_data->normal_table = normal_table_3;
  sphere3_data->face_end *= sizeof(Face);
  sphere3_data->vertex_end *= sizeof(Vector);

  sphere2_data->face_table = (Face *)(data_address + (udword)sphere2_data->face_table);
  sphere2_data->vertex_table = (Vector *)(data_address + (udword)sphere2_data->vertex_table);
  sphere2_data->mapping_table = (MapVector *)(data_address + (udword)sphere2_data->mapping_table);
  sphere2_data->smoothing_table = (udword *)(data_address + (udword)sphere2_data->smoothing_table);
  sphere2_data->normal_table = normal_table_2;
  sphere2_data->face_end *= sizeof(Face);
  sphere2_data->vertex_end *= sizeof(Vector);

  sphere1_data->face_table = (Face *)(data_address + (udword)sphere1_data->face_table);
  sphere1_data->vertex_table = (Vector *)(data_address + (udword)sphere1_data->vertex_table);
  sphere1_data->mapping_table = (MapVector *)(data_address + (udword)sphere1_data->mapping_table);
  sphere1_data->smoothing_table = (udword *)(data_address + (udword)sphere1_data->smoothing_table);
  sphere1_data->normal_table = normal_table_1;
  sphere1_data->face_end *= sizeof(Face);
  sphere1_data->vertex_end *= sizeof(Vector);

  elus_data->face_table = (Face *)(data_address + (udword)elus_data->face_table);
  elus_data->vertex_table = (Vector *)(data_address + (udword)elus_data->vertex_table);
  elus_data->mapping_table = (MapVector *)(data_address + (udword)elus_data->mapping_table);
  elus_data->smoothing_table = (udword *)(data_address + (udword)elus_data->smoothing_table);
  elus_data->normal_table = normal_table_1;
  elus_data->face_end *= sizeof(Face);
  elus_data->vertex_end *= sizeof(Vector);
}


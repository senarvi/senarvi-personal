#include "vector.h"


material rock_material = {
    {0.11, 0.22, 0.33},
    {0.11, 0.22, 0.33},
    {0.33, 0.33, 0.33},
    4,
    2
};

Plane normal_table_1[MAX_FACES];
Plane normal_table_2[MAX_FACES];
Plane normal_table_3[MAX_FACES];

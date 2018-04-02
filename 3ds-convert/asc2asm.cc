/*
 * asc2asm.cpp
 *
 * Muuntaa Autodesk 3D Studion ASCII mesh-tiedoston
 * Assembler-kielen taulukoiksi
 *
 * Copyright (C) Seppo Enarvi   1996,1997
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NAME_LENGTH 10
#define MAX_SEARCH_LENGTH 100
#define FRAC_BITS 16
#define MUL (1 << FRAC_BITS)

typedef unsigned short int uword;


FILE *input_file, *output_file;

uword num_vertices, num_faces;
float *vertex_table;
float *map_table;
uword *face_table;
char  *smoothing_table;

/*
 * Initialization
 */

// [fold]  [
void uninit() {

    fclose( input_file );
    fclose( output_file );

    free( vertex_table );
    free( face_table );
    free( map_table );
}
// [fold]  ]

// [fold]  [
void init( int num_params, char *param_table[] ) {

    atexit( uninit );

    if( num_params < 2 ) {
        printf( "usage: asc2asm <source.asc> <dest.asm>\n" );
        exit( -1 );
    }

    input_file = fopen( param_table[ 1 ], "r" );
    if( input_file == NULL ) {
        printf( "File not found: %s\n", param_table[ 1 ] );
        exit( -1 );
    }
    output_file = fopen( param_table[ 2 ], "w" );
    if( output_file == NULL ) {
        printf( "Error creating file: %s\n", param_table[ 2 ] );
        exit( -1 );
    }
}
// [fold]  ]

// [fold]  [
void alloc_mem( uword num_vertices, uword num_faces) {

    vertex_table    = (float *)malloc( num_vertices * 3 * sizeof( float ) );
    map_table       = (float *)malloc( num_vertices * 2 * sizeof( float ) );
    face_table      = (uword *)malloc( num_faces    * 3 * sizeof( uword ) );
    smoothing_table = (char  *)malloc( num_faces    * 1 * sizeof( char  ) );
    if( smoothing_table == NULL ) {
        printf( "Memory allocation error\n" );
        exit( -1 );
    }
}
// [fold]  ]

/*
 * String search
 */

// [fold]  [
void goto_string( char *search_string ) {

    char input_string[ MAX_SEARCH_LENGTH ];

    do
        if( fscanf( input_file, "%s", input_string ) == EOF ) {
            printf( "Required item not found" );
            exit( -1 );
        }
    while( strncmp( input_string, search_string, strlen( search_string ) ) != 0 );
}
// [fold]  ]

/*
 * Main program
 */

// [fold]  [
int main( int argc, char *argv[] ) {

    char object_name[ MAX_NAME_LENGTH ];
    unsigned long count;
    signed long x, y, z;


    init( argc, argv );

    goto_string( "object:" );
    fscanf( input_file, "%s", &object_name );
    strcpy( object_name, &object_name[ 1 ] );
    strcpy( &object_name[ strlen( object_name ) - 1 ], "\0" );

    goto_string( "Vertices:" );
    fscanf( input_file, " %u     Faces: %u", &num_vertices, &num_faces );
    alloc_mem( num_vertices, num_faces );


    // Read vertices & mapping coordinates
    goto_string( "list:" );
    for( count = 0; count < num_vertices; count++ ) {

        goto_string( "Vertex" );
        fscanf( input_file, " %*s" );  // Ignore vertex number

        fscanf( input_file, "  X:%f     Y:%f     Z:%f     U:%f     V:%f",
                &vertex_table[ count * 3 + 0 ],
                &vertex_table[ count * 3 + 1 ],
                &vertex_table[ count * 3 + 2 ],
                &map_table[ count * 2 + 0 ],
                &map_table[ count * 2 + 1 ] );
    }

    fscanf( input_file, "  X:%f", &vertex_table[ 0 ] );

    // Read faces & smoothing groups
    goto_string( "list:" );
    for( count = 0; count < num_faces; count++ ) {

        goto_string( "Face" );
        fscanf( input_file, " %*s" );  // Ignore face number

        fscanf( input_file, "    A:%u B:%u C:%u AB:%*i BC:%*i CA:%*i",
                &face_table[ count * 3 + 0 ],
                &face_table[ count * 3 + 1 ],
                &face_table[ count * 3 + 2 ] );

        goto_string( "Smoothing:" );
        fscanf( input_file, "  %u", &smoothing_table[ count ] );
    }


    // Print vertices
    fprintf( output_file, "label   %s_vertex_table svect\n", object_name );
    for( count = 0; count < num_vertices; count++ ) {

        x = (signed long)(vertex_table[ count * 3 + 0 ] * MUL);
        y = (signed long)(vertex_table[ count * 3 + 1 ] * MUL);
        z = (signed long)(vertex_table[ count * 3 + 2 ] * MUL);

        if( x >= 0 ) fprintf( output_file, "svect   <0%xh, ",    x );
        else         fprintf( output_file, "svect   <-0%xh, ",  -x );
        if( y >= 0 ) fprintf( output_file,          "0%xh, ",    y );
        else         fprintf( output_file,          "-0%xh, ",  -y );
        if( z >= 0 ) fprintf( output_file,          "0%xh>\n",   z );
        else         fprintf( output_file,          "-0%xh>\n", -z );
    }

    // Print mapping coordinates
    fprintf( output_file, "\nlabel   %s_map_table smap\n", object_name );
    for( count = 0; count < num_vertices; count++ ) {

        x = (unsigned long)(map_table[ count * 2 + 0 ] * MUL);
        y = (unsigned long)(map_table[ count * 2 + 1 ] * MUL);

        if( x >= 0 ) fprintf( output_file, "smap    <0%xh, ",   x );
        else         fprintf( output_file, "smap    <-0%xh, ", -x );
        if( y >= 0 ) fprintf( output_file,          "0%xh>\n",  y );
        else         fprintf( output_file,          "-0%xh>\n", -y );
    }

    // Print faces
    fprintf( output_file, "\nlabel   %s_face_table sface\n", object_name );
    for( count = 0; count < num_faces; count++ )

        fprintf( output_file, "sface   <%u, %u, %u>\n",
                 face_table[ count * 3 + 0 ],
                 face_table[ count * 3 + 1 ],
                 face_table[ count * 3 + 2 ] );

    // Print smoothing groups
    fprintf( output_file, "\nlabel   %s_smoothing_table byte\n", object_name );
    for( count = 0; count < num_faces; count++ )

        fprintf( output_file, "db      %u\n",
                 smoothing_table[ count ] );

    return( 0 );
}
// [fold]  ]
// [fold]  5

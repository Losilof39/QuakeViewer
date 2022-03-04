#pragma once
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <SDL.h>
#include <string>
#include <glm.hpp>
#include <matrix.hpp>
#include "mathlib.h"

#define WIDTH 640
#define HEIGHT 480

#define BSP_OFFS 330
#define CHILDLEAF 0x8000

///////////////////////
//	PAK FILE STRUCTS
///////////////////////

typedef struct pak_header_s
{
	uint8_t id[4];
	int32_t offset;
	int32_t size;
}pak_header_t;

typedef struct pak_file_s
{
	uint8_t name[56];
	int32_t offset;
	int32_t size;
}pak_file_t;

///////////////////////
//	BSP STRUCTS
///////////////////////

typedef struct dentry_s
{
    int32_t offset;
    int32_t size;
}dentry_t;

typedef struct bsp_header_s       // The BSP file header
{ long  version;               // Model version, must be 0x17 (23).
  dentry_t entities;           // List of Entities.
  dentry_t planes;             // Map Planes.
                               // numplanes = size/sizeof(plane_t)
  dentry_t miptex;             // Wall Textures.
  dentry_t vertices;           // Map Vertices.
                               // numvertices = size/sizeof(vertex_t)
  dentry_t visilist;           // Leaves Visibility lists.
  dentry_t nodes;              // BSP Nodes.
                               // numnodes = size/sizeof(node_t)
  dentry_t texinfo;            // Texture Info for faces.
                               // numtexinfo = size/sizeof(texinfo_t)
  dentry_t faces;              // Faces of each surface.
                               // numfaces = size/sizeof(face_t)
  dentry_t lightmaps;          // Wall Light Maps.
  dentry_t clipnodes;          // clip nodes, for Models.
                               // numclips = size/sizeof(clipnode_t)
  dentry_t leaves;             // BSP Leaves.
                               // numlaves = size/sizeof(leaf_t)
  dentry_t lface;              // List of Faces.
  dentry_t edges;              // Edges of faces.
                               // numedges = Size/sizeof(edge_t)
  dentry_t ledges;             // List of Edges.
  dentry_t models;             // List of Models.
                               // nummodels = Size/sizeof(model_t)
} bsp_header_t;

typedef struct boundbox_s                // Bounding Box, Float values
{
    vec3_t min;                // minimum values of X,Y,Z
    vec3_t max;                // maximum values of X,Y,Z
} boundbox_t;

typedef struct                 // Bounding Box, Short values
{
    int16_t   min[3];                 // minimum values of X,Y,Z
    int16_t   max[3];                 // maximum values of X,Y,Z
} bboxshort_t;

typedef struct
{
    boundbox_t bound;            // The bounding box of the Model
    vec3_t origin;               // origin of model, usually (0,0,0)
    int32_t node_id0;               // index of first BSP node
    int32_t node_id1;               // index of the first Clip node
    int32_t node_id2;               // index of the second Clip node
    int32_t node_id3;               // usually zero
    int32_t numleafs;               // number of BSP leaves
    int32_t face_id;                // index of Faces
    int32_t face_num;               // number of Faces
} model_t;

typedef struct
{
    uint16_t vertex0;             // index of the start vertex
                                //  must be in [0,numvertices[
    uint16_t vertex1;             // index of the end vertex
                                 //  must be in [0,numvertices[
} edge_t;

typedef struct
{
    uint16_t plane_id;            // The plane in which the face lies
                                //           must be in [0,numplanes[ 
    uint16_t side;                // 0 if in front of the plane, 1 if behind the plane
    long ledge_id;               // first edge in the List of edges
                                 //           must be in [0,numledges[
    uint16_t ledge_num;           // number of edges in the List of edges
    uint16_t texinfo_id;          // index of the Texture info the face is part of
                                 //           must be in [0,numtexinfos[ 
    uint8_t typelight;            // type of lighting, for the face
    uint8_t baselight;            // from 0xFF (dark) to 0 (bright)
    uint8_t light[2];             // two additional light models  
    int32_t lightmap;               // Pointer inside the general light map, or -1
                                 // this define the start of the face light map
} face_t;

typedef struct
{
    long    plane;            // The plane that splits the node
                                //           must be in [0,numplanes[
    uint16_t front;               // If bit15==0, index of Front child node
                                 // If bit15==1, ~front = index of child leaf
    uint16_t back;                // If bit15==0, id of Back child node
                                 // If bit15==1, ~back =  id of child leaf
    bboxshort_t box;             // Bounding box of node and all childs
    uint16_t face_id;             // Index of first Polygons in the node
    uint16_t face_num;            // Number of faces in the node
} node_t;

typedef struct
{
    long type;                   // Special type of leaf
    long vislist;                // Beginning of visibility lists
                                 //     must be -1 or in [0,numvislist[
    bboxshort_t bound;           // Bounding box of the leaf
    uint16_t lface_id;            // First item of the list of faces
                                 //     must be in [0,numlfaces[
    uint16_t lface_num;           // Number of faces in the leaf  
    uint8_t sndwater;             // level of the four ambient sounds:
    uint8_t sndsky;               //   0    is no sound
    uint8_t sndslime;             //   0xFF is maximum volume
    uint8_t sndlava;              //
} dleaf_t;

typedef struct
{
    vec3_t normal;               // Vector orthogonal to plane (Nx,Ny,Nz)
                                // with Nx2+Ny2+Nz2 = 1
    float dist;               // Offset to plane, along the normal vector.
                                 // Distance from (0,0,0) to the plane
    long    type;                // Type of plane, depending on normal vector.
} plane_t;
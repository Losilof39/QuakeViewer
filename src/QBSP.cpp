#include "QBSP.h"

void QBSP::LoadBSPFromPak(PAK* pak)
{
	bsp_header_t bsp_hdr = { 0 };
	model_t bsp_mdl;

	pak_file_t* pak_files_stream = pak->GetPakFileStream();

	int bsp_offs = pak_files_stream[BSP_OFFS].offset;

	uint8_t* pak_stream = pak->GetPakStream();

	SDL_memcpy(&bsp_hdr, pak_stream + bsp_offs, sizeof(bsp_hdr));

	//uint8_t* offs = pak_stream + bsp_offs + bsp_hdr.entities.offset;

	printf("\n[INFO]: Loaded BSP!\n");
	printf("[INFO]: BSP Version: %d\n\n", bsp_hdr.version);

	// vertices
	int numvertices = bsp_hdr.vertices.size / 12;
	vertices.resize(numvertices);

	// edges
	int numedges = bsp_hdr.edges.size / sizeof(edge_t);
	edges.reserve(numedges);

	// list of edges
	int numledges = bsp_hdr.ledges.size / sizeof(uint16_t);

	// faces
	int numfaces = bsp_hdr.faces.size / sizeof(face_t);
	faces.reserve(numfaces);

	// list of faces
	int numlfaces = bsp_hdr.lface.size / sizeof(int16_t);

	//nodes
	int numnodes = bsp_hdr.nodes.size / sizeof(node_t);
	nodes.reserve(numnodes);

	// leaves
	int numleafs = bsp_hdr.leaves.size / sizeof(dleaf_t);
	leaves.reserve(numleafs);

	// planes
	int numplanes = bsp_hdr.planes.size / sizeof(plane_t);
	planes.reserve(numplanes);

	for (int i = 0; i < numvertices; i++)
	{
		vec3_t cur_vec;
		SDL_memcpy(&cur_vec, pak_stream + bsp_offs + bsp_hdr.vertices.offset + i*4, sizeof(vec3_t));

		vertices[i].x = cur_vec.x;
		vertices[i].y = cur_vec.y;
		vertices[i].z = cur_vec.z;
	}

	printf("[INFO]: Loaded vertices..\n");

	for (int i = 0; i < numedges; i++)
	{
		edge_t cur_edge;
		SDL_memcpy(&cur_edge, pak_stream + bsp_offs + bsp_hdr.edges.offset + i * 4, sizeof(cur_edge));

		edges.push_back(cur_edge);
	}

	printf("[INFO]: Loaded edges..\n");

	for (int i = 0; i < numledges; i++)
	{
		int16_t cur_idx;
		SDL_memcpy(&cur_idx, pak_stream + bsp_offs + bsp_hdr.ledges.offset + i * sizeof(int16_t), sizeof(int16_t));

		listedges.push_back(cur_idx);
	}

	printf("[INFO]: Loaded list of edges..\n");

	for (int i = 0; i < numfaces; i++)
	{
		face_t cur_face;
		SDL_memcpy(&cur_face, pak_stream + bsp_offs + bsp_hdr.faces.offset + i * sizeof(cur_face), sizeof(cur_face));

		faces.push_back(cur_face);
	}

	printf("[INFO]: Loaded faces..\n");

	for (int i = 0; i < numlfaces; i++)
	{
		uint16_t cur_face;
		SDL_memcpy(&cur_face, pak_stream + bsp_offs + bsp_hdr.lface.offset + i * sizeof(uint16_t), sizeof(uint16_t));

		listfaces.push_back(cur_face);
	}

	printf("[INFO]: Loaded list of faces..\n");

	for (int i = 0; i < numnodes; i++)
	{
		node_t cur_node;
		SDL_memcpy(&cur_node, pak_stream + bsp_offs + bsp_hdr.nodes.offset + i * sizeof(cur_node), sizeof(cur_node));

		nodes.push_back(cur_node);
	}

	printf("[INFO]: Loaded nodes..\n");

	for (int i = 0; i < numplanes; i++)
	{
		plane_t cur_plane;
		SDL_memcpy(&cur_plane, pak_stream + bsp_offs + bsp_hdr.planes.offset + i * sizeof(plane_t), sizeof(plane_t));

		planes.push_back(cur_plane);
	}

	printf("[INFO]: Loaded planes..\n");

	for (int i = 0; i < numleafs; i++)
	{
		dleaf_t cur_leaf;
		SDL_memcpy(&cur_leaf, pak_stream + bsp_offs + bsp_hdr.leaves.offset + i * sizeof(dleaf_t), sizeof(dleaf_t));

		leaves.push_back(cur_leaf);
	}

	printf("[INFO]: Loaded leaves..\n");

	SDL_memcpy(&bsp_mdl, pak_stream + bsp_offs + bsp_hdr.models.offset, sizeof(bsp_mdl));
}

int QBSP::IsFrontOfPlane(float normal_x, float normal_y, float normal_z, float intercept, glm::vec3 cam_pos)
{
	float distance = cam_pos.x * normal_x + cam_pos.y * normal_y + cam_pos.z * normal_z - intercept;

	if (distance >= 0.0f)
		return 1;
	else
		return -1;
}

int QBSP::FindCamLeaf(glm::vec3 cam_pos)
{
	int16_t camleaf = 0;

	while ((camleaf & CHILDLEAF) == false)
	{

		if (IsFrontOfPlane(planes[nodes[camleaf].plane].normal.x,
			planes[nodes[camleaf].plane].normal.y,
			planes[nodes[camleaf].plane].normal.z,
			planes[nodes[camleaf].plane].dist, cam_pos) == 1)
		{
			camleaf = nodes[camleaf].front;
		}
		else
		{
			camleaf = nodes[camleaf].back;
		}

	}

	return ~camleaf;
}
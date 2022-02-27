#pragma once

#include <string>
#include "quakedef.h"
#include "PAK.h"

class QBSP
{
public:

	void LoadBSPFromPak(PAK* pak);
	int FindCamLeaf(glm::vec3 cam_pos);

private:
	int IsFrontOfPlane(float normal_x, float normal_y, float normal_z, float intercept, glm::vec3 cam_pos);

	std::vector<glm::vec3> vertices;
	std::vector<edge_t> edges;
	std::vector<int16_t> listedges;
	std::vector<face_t> faces;
	std::vector<uint16_t> listfaces;
	std::vector<node_t> nodes;
	std::vector<dleaf_t> leaves;
	std::vector<plane_t> planes;
};


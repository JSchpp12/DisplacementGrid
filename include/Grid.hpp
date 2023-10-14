#pragma once

#include "HeightDisplacementMaterial.hpp"
#include "StarMesh.hpp"
#include "StarObject.hpp"
#include "Vertex.hpp"

/// <summary>
/// Grid object which will programmatically generate vertices as needed.
/// </summary>
class Grid : public star::StarObject {
public:
	Grid(int vertX, int vertY) : vertX(vertX), vertY(vertY)
	{
		//assert(this->span >= 3 && "There must be more than 3 verts used in each dimension of the grid"); 
		//assert(this->span % 2 == 1 && "The width of the grid must be an odd number"); 
	};

protected:
	struct Triangle {
		Triangle() = default;
		int laneA, laneB, laneC; 
	};
	int vertX=0, vertY=0;

	// Inherited via StarObject
	std::vector<std::unique_ptr<star::StarMesh>> loadMeshes() override;
};
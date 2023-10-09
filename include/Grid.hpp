#pragma once

#include "StarMesh.hpp"
#include "StarObject.hpp"
#include "Vertex.hpp"

/// <summary>
/// Grid object which will programmatically generate vertices as needed.
/// </summary>
class Grid : public star::StarObject {
public:
	Grid(int numVertsX, int numVertsY) : numVertsX(numVertsX), numVertsY(numVertsY)
	{ };

protected:
	struct Triangle {
		Triangle() = default;
		int laneA, laneB, laneC; 
	};
	int numVertsX=0, numVertsY = 0;

	// Inherited via StarObject
	std::vector<std::unique_ptr<star::StarMesh>> loadMeshes() override;
};
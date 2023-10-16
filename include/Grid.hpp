#pragma once

#include "VertColorMaterial.hpp"
#include "HeightDisplacementMaterial.hpp"
#include "StarMesh.hpp"
#include "StarTexture.hpp"
#include "StarObject.hpp"
#include "Vertex.hpp"
#include "Color.hpp"

#include <memory>

/// <summary>
/// Grid object which will programmatically generate vertices as needed.
/// </summary>
class Grid : public star::StarObject{
public:
	Grid(int vertX, int vertY);

	void updateTexture(); 

	const std::vector<std::unique_ptr<star::StarMesh>>& getMeshes() override { return this->meshes; };

protected:
	star::HeightDisplacementMaterial material; 
	std::vector<std::unique_ptr<star::StarMesh>> meshes;
	std::unique_ptr<star::StarTexture> displacementTexture; 
	int vertX=0, vertY=0;

};
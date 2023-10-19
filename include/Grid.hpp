#pragma once

#include "VertColorMaterial.hpp"
#include "HeightDisplacementMaterial.hpp"
#include "StarMesh.hpp"
#include "RuntimeUpdateTexture.hpp"
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

	void updateTexture(std::vector<int> locsX, std::vector<int> locY, const std::vector<star::Color> newColor);

	const std::vector<std::unique_ptr<star::StarMesh>>& getMeshes() override { return this->meshes; };
	star::Color getTexColorAt(int x, int y) { return this->material.getTexture().getRawData()->at(x).at(y); }
	int getSizeX() { return this->vertX; }
	int getSizeY() { return this->vertY; }
protected:
	int vertX=0, vertY=0;
	star::HeightDisplacementMaterial material; 
	std::vector<std::unique_ptr<star::StarMesh>> meshes;
	std::unique_ptr<star::RuntimeUpdateTexture> displacementTexture; 
};
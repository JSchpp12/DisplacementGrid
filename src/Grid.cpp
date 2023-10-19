#include "Grid.hpp"

Grid::Grid(int vertX, int vertY) : material(vertX,vertY), vertX(vertX), vertY(vertY) {
	//calculate everything in x-y
	std::vector<std::unique_ptr<star::StarMesh>> grid;
	std::vector<std::vector<star::Color>> textureData = std::vector<std::vector<star::Color>>(vertY, std::vector<star::Color>(vertX)); 


	auto verts = std::unique_ptr<std::vector<star::Vertex>>(new std::vector<star::Vertex>());
	auto indices = std::unique_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>());
	bool test = true; 
	float stepSizeX = 1.0f / (vertX - 1);
	float stepSizeY = 1.0f / (vertY - 1);
	float xCounter = 0.0f;
	uint32_t indexCounter = 0;

	for (int i = 0; i < vertY; i++) {

		for (int j = 0; j < vertX; j++) {
			verts->push_back(star::Vertex{
				glm::vec3{stepSizeY * j, 0.0f, stepSizeX * i},
				glm::vec3{0.0f, 1.0f, 0.0f}, 
				glm::vec3{0.0f,0.0f,0.0f},
				glm::vec2{stepSizeY*i, stepSizeX*j}							//texture coordinate
			});

			material.getTexture().getRawData()->at(i).at(j) = star::Color{ 255,0,0,255 };

			if (j % 2 == 1 && i % 2 == 1) {
				//this is a 'central' vert where drawing should be based around
				// 
				//uppper left
				uint32_t center = indexCounter;
				uint32_t centerLeft = indexCounter - 1;
				uint32_t centerRight = indexCounter + 1;
				uint32_t upperLeft = indexCounter - 1 - vertX;
				uint32_t upperCenter = indexCounter - vertX;
				uint32_t upperRight = indexCounter - vertX + 1;
				uint32_t lowerLeft = indexCounter + vertX - 1;
				uint32_t lowerCenter = indexCounter + vertX;
				uint32_t lowerRight = indexCounter + vertX + 1;
				//1
				indices->push_back(center);
				indices->push_back(upperLeft);
				indices->push_back(centerLeft);
				//2
				indices->push_back(center);
				indices->push_back(upperCenter);
				indices->push_back(upperLeft);

				if (i != vertY - 1 && j == vertX - 1)
				{
					//side piece
					//cant do 3,4,5,6,
					//7
					indices->push_back(center);
					indices->push_back(lowerLeft);
					indices->push_back(lowerCenter);
					//8
					indices->push_back(center);
					indices->push_back(centerLeft);
					indices->push_back(lowerLeft);

				}
				else if (i == vertY - 1 && j != vertX - 1)
				{
					//bottom piece
					//cant do 5,6,7,8
					//3
					indices->push_back(center);
					indices->push_back(upperRight);
					indices->push_back(upperCenter);
					//4
					indices->push_back(center);
					indices->push_back(centerRight);
					indices->push_back(upperRight);
				}
				else if (i != vertY - 1 && j != vertX - 1) {
					//3
					indices->push_back(center);
					indices->push_back(upperRight);
					indices->push_back(upperCenter);
					//4
					indices->push_back(center);
					indices->push_back(centerRight);
					indices->push_back(upperRight);
					//5
					indices->push_back(center);
					indices->push_back(lowerRight);
					indices->push_back(centerRight);
					//6
					indices->push_back(center);
					indices->push_back(lowerCenter);
					indices->push_back(lowerRight);
					//7
					indices->push_back(center);
					indices->push_back(lowerLeft);
					indices->push_back(lowerCenter);
					//8
					indices->push_back(center);
					indices->push_back(centerLeft);
					indices->push_back(lowerLeft);
				}

			}
			indexCounter++;
		}
	}

	this->meshes.push_back(std::unique_ptr<star::StarMesh>(new star::StarMesh(std::move(verts), std::move(indices), material))); 
}

void Grid::updateTexture(std::vector<int> locsX, std::vector<int> locsY, const std::vector<star::Color> newColor) {
	assert(this->meshes.size() > 0 && "Make sure this function is only called after the prepRender phase");

	star::RuntimeUpdateTexture& tex = this->material.getTexture();
	for (int i = 0; i < locsX.size(); i++) {
		tex.getRawData()->at(locsX[i]).at(locsY[i]) = newColor[i];
	}

	tex.updateGPU();
}
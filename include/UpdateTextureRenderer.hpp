#pragma once

#include "BasicRenderer.hpp"

#include <vulkan/vulkan.hpp>

class UpdateTextureRenderer : public star::BasicRenderer {
public:
	UpdateTextureRenderer(star::StarWindow& window, std::vector<std::unique_ptr<star::Light>>& lightList,
		std::vector<std::reference_wrapper<star::StarObject>> objectList, star::StarCamera& camera, star::RenderOptions& renderOptions,
		star::StarDevice& device); 

	virtual ~UpdateTextureRenderer(); 

	virtual void draw() override;

	virtual void createSemaphores() override; 

	vk::Semaphore& getTextureUpdateSemaphore() { return textureUpdateDoneSemaphore; }

protected:
	vk::Semaphore textureUpdateDoneSemaphore; 


};
#include "HeightDisplacementMaterial.hpp"

void star::HeightDisplacementMaterial::prepRender(StarDevice& device)
{
}

void star::HeightDisplacementMaterial::initDescriptorLayouts(StarDescriptorSetLayout::Builder& constBuilder)
{
}

void star::HeightDisplacementMaterial::buildConstDescriptor(StarDescriptorWriter writer)
{
	writer.build(this->descriptorSet);
}

void star::HeightDisplacementMaterial::bind(vk::CommandBuffer& commandBuffer, vk::PipelineLayout pipelineLayout, int swapChainImageIndex)
{
	if (this->descriptorSet) {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 2, 1, &this->descriptorSet, 0, nullptr);
	}
}

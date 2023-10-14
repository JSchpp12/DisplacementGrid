#pragma once

#include "StarMaterial.hpp"

namespace star {
	class HeightDisplacementMaterial : public StarMaterial {
	public:
		// Inherited via StarMaterial
		void prepRender(StarDevice& device) override;

		void initDescriptorLayouts(StarDescriptorSetLayout::Builder& constBuilder) override;

		void buildConstDescriptor(StarDescriptorWriter writer) override;

		void bind(vk::CommandBuffer& commandBuffer, vk::PipelineLayout pipelineLayout, int swapChainImageIndex) override;

	protected:

	};
}
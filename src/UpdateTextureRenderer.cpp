#include "UpdateTextureRenderer.hpp"

UpdateTextureRenderer::UpdateTextureRenderer(star::StarWindow& window, std::vector<std::unique_ptr<star::Light>>& lightList,
	std::vector<std::reference_wrapper<star::StarObject>> objectList, star::StarCamera& camera, star::RenderOptions& renderOptions, 
	star::StarDevice& device) : star::BasicRenderer(window, lightList, objectList, camera, renderOptions, device) { }

UpdateTextureRenderer::~UpdateTextureRenderer() {
	this->device.getDevice().destroySemaphore(textureUpdateDoneSemaphore); 
}

void UpdateTextureRenderer::draw() {
	/* Goals of each call to drawFrame:
	   *   get an image from the swap chain
	   *   execute command buffer with that image as attachment in the framebuffer
	   *   return the image to swapchain for presentation
	   * by default each of these steps would be executed asynchronously so need method of synchronizing calls with rendering
	   * two ways of doing this:
	   *   1. fences
	   *       accessed through calls to vkWaitForFences
	   *       designed to synchronize application itself with rendering ops
	   *   2. semaphores
	   *       designed to synchronize opertaions within or across command queues
	   * need to sync queue operations of draw and presentation commmands -> using semaphores
	   */

	   //wait for fence to be ready 
	   // 3. 'VK_TRUE' -> waiting for all fences
	   // 4. timeout 
	this->device.getDevice().waitForFences(inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	/* Get Image From Swapchain */

	//as is extension we must use vk*KHR naming convention
	//UINT64_MAX -> 3rd argument: used to specify timeout in nanoseconds for image to become available
	/* Suboptimal SwapChain notes */
		//vulkan can return two different flags 
		// 1. VK_ERROR_OUT_OF_DATE_KHR: swap chain has become incompatible with the surface and cant be used for rendering. (Window resize)
		// 2. VK_SUBOPTIMAL_KHR: swap chain can still be used to present to the surface, but the surface properties no longer match
	auto result = this->device.getDevice().acquireNextImageKHR(swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame]);

	if (result.result == vk::Result::eErrorOutOfDateKHR) {
		//the swapchain is no longer optimal according to vulkan. Must recreate a more efficient swap chain
		recreateSwapChain();
		return;
	}
	else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR) {
		//for VK_SUBOPTIMAL_KHR can also recreate swap chain. However, chose to continue to presentation stage
		throw std::runtime_error("failed to acquire swap chain image");
	}
	uint32_t imageIndex = result.value;

	//check if a previous frame is using the current image
	if (imagesInFlight[imageIndex]) {
		this->device.getDevice().waitForFences(1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	//mark image as now being in use by this frame by assigning the fence to it 
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	updateUniformBuffer(imageIndex);


	/* Command Buffer */
	vk::SubmitInfo submitInfo{};
	submitInfo.sType = vk::StructureType::eSubmitInfo;

	vk::Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame] };
	//where in the pipeline should the wait happen, want to wait until image becomes available
	//wait at stage of color attachment -> theoretically allows for shader execution before wait 
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput }; //each entry corresponds through index to waitSemaphores[]
	
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	//which command buffers to submit for execution -- should submit command buffer that binds the swap chain image that was just acquired as color attachment
	submitInfo.commandBufferCount = 1;
	//submitInfo.pCommandBuffers = &graphicsCommandBuffers[imageIndex];
	submitInfo.pCommandBuffers = &this->device.getGraphicsCommandBuffers()->at(imageIndex);

	//what semaphores to signal when command buffers have finished
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	//set fence to unsignaled state
	this->device.getDevice().resetFences(1, &inFlightFences[currentFrame]);
	auto submitResult = this->device.getGraphicsQueue().submit(1, &submitInfo, inFlightFences[currentFrame]);
	if (submitResult != vk::Result::eSuccess) {
		throw std::runtime_error("failed to submit draw command buffer");
	}

	/* Presentation */
	vk::PresentInfoKHR presentInfo{};
	//presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.sType = vk::StructureType::ePresentInfoKHR;

	//what to wait for 
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	//what swapchains to present images to 
	vk::SwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	//can use this to get results from swap chain to check if presentation was successful
	presentInfo.pResults = nullptr; // Optional

	//make call to present image
	auto presentResult = this->device.getPresentQueue().presentKHR(presentInfo);

	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
	if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || frameBufferResized) {
		frameBufferResized = false;
		recreateSwapChain();
	}
	else if (presentResult != vk::Result::eSuccess) {
		throw std::runtime_error("failed to present swap chain image");
	}

	//advance to next frame
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void UpdateTextureRenderer::createSemaphores()
{
	this->star::BasicRenderer::createSemaphores(); 

	//ONLY WORK WITH 1 FOR NOW
	// 
	//create semaphore to halt rendering pipe at top to wait for update transfer to complete
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		auto newSemaphoreCreate = vk::SemaphoreCreateInfo{};
		newSemaphoreCreate.sType = vk::StructureType::eSemaphoreCreateInfo; 
		this->textureUpdateDoneSemaphore = this->device.getDevice().createSemaphore(newSemaphoreCreate); 
	}
}


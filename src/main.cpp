#include <iostream>

#include "StarEngine.hpp"

#include "Application.hpp"

#include "modules/InteractionSystem.hpp"
#include "common/ConfigFile.hpp"
#include "common/RenderOptions.hpp"

#include "builders/SceneBuilder.hpp"

#include "managers/ShaderManager.hpp"
#include "managers/ObjectManager.hpp"
#include "managers/TextureManager.hpp"
#include "managers/LightManager.hpp"
#include "managers/MaterialManager.hpp"
#include "managers/MapManager.hpp"

#include "controllers/CameraController.hpp"
#include "controllers/OptionsController.hpp"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

int main() {
    auto application = Application();
    application.Load();

    //std::unique_ptr<star::OptionsController> optionsController(new star::OptionsController(*renderOptions));

    //TODO: implement better management system 
    //std::vector<star::Light*> mainLightList(lightList->size());
    //for (size_t i = 0; i < lightList->size(); i++) {
    //    mainLightList.at(i) = &lightManager->resource(lightList->at(i));
    //}

    //prepare renderer 
    //auto window = star::StarWindow(WIDTH, HEIGHT, "Starlight", star::InteractionSystem::glfwKeyHandle, star::InteractionSystem::glfwMouseButtonCallback, star::InteractionSystem::glfwMouseMovement, star::InteractionSystem::glfwScrollCallback);
    //auto renderer = star::VulkanRenderer(*configFile, *renderOptions, *shaderManager, *objectManager, *textureManager, *mapManager, *materialManager, *camera, *objectList, mainLightList, window);
    //renderer.prepare();

    //register user application callbacks

    auto time = star::Time();

    std::unique_ptr<star::RenderOptions> renderOptions(new star::RenderOptions);
    std::unique_ptr<star::OptionsController> optionsController(new star::OptionsController(*renderOptions));

    auto engine = star::StarEngine::Builder().build(application.getCamera(), application.getLights(), application.getObjects(), *renderOptions);

    try {
        engine->Run(); 
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        do {
            std::cout << "Press a key to exit..." << std::endl;
        } while (std::cin.get() != '\n');
        return EXIT_FAILURE;
    }
}
#include "Application.hpp"

using namespace star; 

int Application::disabledLightCounter = int(0);
bool Application::upCounter = true;
bool Application::rotatingCounterClock = true;

std::chrono::steady_clock::time_point Application::timeSinceLastUpdate = std::chrono::steady_clock::now();

Application::Application(star::StarScene& scene)
    : StarApplication(scene)
{
    auto mediaDirectoryPath = StarEngine::GetSetting(star::Config_Settings::mediadirectory);
    {
        
        auto redShader = StarEngine::GetSetting(star::Config_Settings::mediadirectory) + "shaders/red.frag"; 
        this->camera.setPosition(glm::vec3{ 2.0, 1.0f, 3.0f });
        auto camPosition = this->camera.getPosition();
        this->camera.setLookDirection(-camPosition);
    
        auto redShaderHandle = StarEngine::shaderManager.addResource(redShader, std::make_unique<star::StarShader>(redShader));

        std::unique_ptr<Grid> grid = std::make_unique<Grid>(1000, 1000);
        grid->setScale(glm::vec3{ 5.0, 5.0, 5.0 });
        auto gridHandle = this->scene.add(std::move(grid));
        StarObject* rawRef = &this->scene.getObject(gridHandle);
        this->gridObj = static_cast<Grid*>(rawRef); 
        gridObj->setPosition(glm::vec3{ 0.0, 0.1f, 0.0 }); 
        this->scene.add(std::unique_ptr<star::Light>(new Light(star::Type::Light::directional, glm::vec3{ 10,10,10 }))); 

        {
            auto objectPath = mediaDirectoryPath + "models/icoSphere/low_poly_icoSphere.obj";
            auto vertShaderPath = mediaDirectoryPath + "models/icoSphere/icoSphere.vert";
            auto fragShaderPath = mediaDirectoryPath + "models/icoSphere/icoSphere.frag";
        }
    }
}

void Application::Load()
{
}

void Application::onWorldUpdate()
{
    bool update = false; 

    if (star::KeyStates::state(star::KEY::UP)) {
         auto newLocY = upTexLocY + 1; 
        if (newLocY > 0 && newLocY < gridObj->getSizeY())
            upTexLocY = newLocY; 
        update = true;
    }

    if (star::KeyStates::state(star::KEY::DOWN)) {
        auto newLocY = upTexLocY - 1;
        if (newLocY > 0 && newLocY < gridObj->getSizeY())
            upTexLocY = newLocY;
        update = true;
    }

    if (star::KeyStates::state(star::KEY::RIGHT)) {
        auto newLocX = upTexLocX + 1; 
        if (newLocX > 0 && newLocX < gridObj->getSizeX())
            upTexLocX = newLocX;
        update = true;
    }

    if (star::KeyStates::state(star::KEY::LEFT)) {
        auto newLocX = upTexLocX - 1;
        if (newLocX > 0 && newLocX < gridObj->getSizeX())
            upTexLocX = newLocX;
        update = true;
    }

    if (update)
        std::cout << "Location: " << upTexLocX << ", " << upTexLocY << std::endl;

    if (star::KeyStates::state(star::KEY::SPACE)) {
        //cast ray toward plane
        auto tail = this->camera.getPosition();
        auto lookDir = glm::normalize(this->camera.getLookDirection());
        auto head = this->camera.getPosition() + lookDir;

        std::optional<glm::vec2> hitPoint = gridObj->getXYCoordsWhereRayIntersectsMe(tail, head);
        if (hitPoint.has_value()) {
            applyStrokeAroundLocation(hitPoint.value(), 50, 10);
        }
    }

    if (star::KeyStates::state(star::KEY::R)) {
        //reset texture
        std::vector<int> locsX, locsY; 
        std::vector<star::Color> colors; 

        for (int i = 0; i < gridObj->getSizeX(); i++) {
            for (int j = 0; j < gridObj->getSizeY(); j++) {
                locsX.push_back(i); 
                locsY.push_back(j); 
                colors.push_back(star::Color(255, 0, 0, 1)); 
            }
        }
        gridObj->updateTexture(locsX, locsY, colors);
    }
}

void Application::onKeyPress(int key, int scancode, int mods)
{
    //if (key == GLFW_KEY_M) {
    //    auto& light = this->sceneBuilder.light(lightList.at(disabledLightCounter));
    //    light.setEnabled();
    //    if (!upCounter && disabledLightCounter == 0) {
    //        upCounter = true;
    //    }
    //    else if (upCounter && disabledLightCounter == lightList.size() - 1) {
    //        upCounter = false;
    //    }
    //    disabledLightCounter = upCounter ? disabledLightCounter + 1 : disabledLightCounter - 1;
    //}
   

    //if (key == GLFW_KEY_Z) {
    //    sun->setEnabled();
    //}
}

std::unique_ptr<star::StarRenderer> Application::getRenderer(star::StarDevice& device, star::StarWindow& window, star::RenderOptions& options)
{
    std::vector<std::unique_ptr<Light>>& lightList = scene.getLights();
    std::vector<std::reference_wrapper<StarObject>> prepObjects;
    for (auto& obj : scene.getObjects()) {
        prepObjects.push_back(*obj.second);
    }
    auto nRender = std::unique_ptr<UpdateTextureRenderer>(new UpdateTextureRenderer(window, lightList, prepObjects, camera, options, device));
    textureUpdateDone = &nRender->getTextureUpdateSemaphore();

    return std::move(nRender); 
}

void Application::applyStrokeAroundLocation(glm::vec2 location, int width, int centerStrength)
{
    std::vector<int> locsX, locsY;
    std::vector<star::Color> colors;

    for (int i = 0; i < width; i++) {
        int locationX = 0;
        if (i < width/2) {
            locationX = location.x - i;
        }
        else {
            locationX = location.x + (i - (width/2));
        }

        for (int j = 0; j < width; j++) {
            int locationY = 0; 
            if (j > width / 2)
                locationY = location.y - j;
            else
                locationY = location.y + (j - (width/2)); 

            auto newColor = gridObj->getTexColorAt(locationX, locationY);
            if (newColor.g() < 255) {
                double strength = ((glm::abs(locationX + location.x))/location.x) * 0.5; 
                strength = strength + (((glm::abs(locationY + location.y))/location.y) * 0.5); 

                newColor = star::Color{
                newColor.r(),
                newColor.g(),
                (int)glm::floor(newColor.b() + (centerStrength * strength)),
                255 };
            }

            if (canApplyColorToLocation(locationX, locationY, newColor)) {
                locsX.push_back(locationX);
                locsY.push_back(locationY);
                colors.push_back(newColor);
            }
        }
    }

    gridObj->updateTexture(locsX, locsY, colors);
}

bool Application::canApplyColorToLocation(int x, int y, star::Color color) {
    if (x >= gridObj->getSizeX() - 1 || x < 0)
        return false;

    if (y >= gridObj->getSizeY() || y < 0)
        return false;

    return true; 
}

void Application::onKeyRelease(int key, int scancode, int mods)
{
}

void Application::onMouseMovement(double xpos, double ypos)
{
    this->mouseXPos = xpos; 
    this->mouseYPos = ypos; 
}

void Application::onMouseButtonAction(int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        std::cout << "Click"; 
        this->wasClick = true; 
    }
}

void Application::onScroll(double xoffset, double yoffset)
{
}

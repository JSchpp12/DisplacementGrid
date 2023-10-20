#include "Application.hpp"

using namespace star; 

Application::Application(star::StarScene& scene)
    : StarApplication(scene)
{
    auto mediaDirectoryPath = StarEngine::GetSetting(star::Config_Settings::mediadirectory);
    {
        this->camera.setPosition(glm::vec3{ 2.0, 1.0f, 3.0f });
        auto camPosition = this->camera.getPosition();
        this->camera.setLookDirection(-camPosition);

        std::unique_ptr<Grid> grid = std::make_unique<Grid>(1000, 1000);
        grid->setScale(glm::vec3{ 5.0, 5.0, 5.0 });
        auto gridHandle = this->scene.add(std::move(grid));
        StarObject* rawRef = &this->scene.getObject(gridHandle);
        this->gridObj = static_cast<Grid*>(rawRef); 
        gridObj->setPosition(glm::vec3{ 0.0, 0.1f, 0.0 }); 
        this->scene.add(std::unique_ptr<star::Light>(new Light(star::Type::Light::directional, glm::vec3{ 10,10,10 }))); 
    }
}

void Application::Load()
{
}

void Application::onWorldUpdate()
{
    bool update = false; 
    timeCounter += point.timeElapsedLastFrameSeconds(); 

    if (timeCounter > 0.1) {
        timeCounter = 0; 

        if (star::KeyStates::state(star::KEY::UP)) {
            update = true;
            width++;
        }
        if (star::KeyStates::state(star::KEY::DOWN)) {
            update = true;
            width--;
        }
        if (star::KeyStates::state(star::KEY::RIGHT)) {
            update = true;
            strength++;
        }
        if (star::KeyStates::state(star::KEY::LEFT)) {
            update = true;
            strength--; 
        }
    }
    
    if (update) {
        std::cout << "Strength: " << strength << std::endl; 
        std::cout << "Width: " << width << std::endl;
    }
        
    if (star::KeyStates::state(star::KEY::SPACE)) {
        //cast ray toward plane
        auto tail = this->camera.getPosition();
        auto lookDir = glm::normalize(this->camera.getLookDirection());
        auto head = this->camera.getPosition() + lookDir;

        std::optional<glm::vec2> hitPoint = gridObj->getXYCoordsWhereRayIntersectsMe(tail, head);
        if (hitPoint.has_value()) {
            applyStrokeAroundLocation(hitPoint.value(), width, strength);
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

    point.updateLastFrameTime(); 
}

void Application::onKeyPress(int key, int scancode, int mods)
{
}

std::unique_ptr<star::StarRenderer> Application::getRenderer(star::StarDevice& device, star::StarWindow& window, star::RenderOptions& options)
{
    std::vector<std::unique_ptr<Light>>& lightList = scene.getLights();
    std::vector<std::reference_wrapper<StarObject>> prepObjects;
    for (auto& obj : scene.getObjects()) {
        prepObjects.push_back(*obj.second);
    }
    auto nRender = std::unique_ptr<UpdateTextureRenderer>(new UpdateTextureRenderer(window, lightList, prepObjects, camera, options, device));
    return std::move(nRender); 
}

void Application::applyStrokeAroundLocation(glm::vec2 location, int width, int centerStrength)
{
    std::vector<int> locsX, locsY;
    std::vector<star::Color> colors;

    float strengthStep = centerStrength / (width/2.0f); 
    int upperX = location.x - (width / 2.0f); 
    int upperY = location.y - (width / 2.0f); 

    for (int i = 0; i < width; i++) {
        int locationX = upperX + i; 

        for (int j = 0; j < width; j++) {
            int locationY = upperY + j; 

            auto newColor = gridObj->getTexColorAt(locationX, locationY);
            if (newColor.g() < 255) {
                float strength = (float)centerStrength - (glm::distance(location, glm::vec2(locationX, locationY)) * strengthStep);
                strength = strength * (timeCounter * 4);

                if (strength < 0)
                    strength = 0; 

                newColor = star::Color{
                newColor.r(),
                newColor.g(),
                (int)glm::floor(newColor.b() + strength),
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
        this->wasClick = true; 
    }
}

void Application::onScroll(double xoffset, double yoffset)
{
}

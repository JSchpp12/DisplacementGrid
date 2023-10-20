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
        //auto lionPath = StarEngine::GetSetting(star::Config_Settings::mediadirectory) + "models/lion-statue/source/rapid.obj";

        //auto lion = BasicObject::New(lionPath);
        //lion->setScale(glm::vec3{ 0.04f, 0.04f, 0.04f });
        //lion->setPosition(glm::vec3{ 0.0, 0.0, 0.0 });
        //lion->rotateGlobal(star::Type::Axis::x, -90);
        //lion->moveRelative(glm::vec3{ 0.0, -1.0, 0.0 });
        //this->scene.add(std::move(lion)); 

        std::unique_ptr<Grid> grid = std::make_unique<Grid>(1000, 1000);
        grid->setScale(glm::vec3{ 5.0, 5.0, 5.0 });
        auto gridHandle = this->scene.add(std::move(grid));
        StarObject* rawRef = &this->scene.getObject(gridHandle);
        this->gridObj = static_cast<Grid*>(rawRef); 
        gridObj->setPosition(glm::vec3{ 0.0, 0.1f, 0.0 }); 
        this->scene.add(std::unique_ptr<star::Light>(new Light(star::Type::Light::directional, glm::vec3{ 10,10,10 }))); 

        //this->sceneBuilder.entity(objectList.at(0)).rotateGolbal(star::Type::Axis::x, -90);

        //load plant 
        //{
        //    auto objectPath = StarEngine::GetSetting(star::Config_Settings::mediadirectory) + "models/aloevera/aloevera.obj";
        //    this->objectList.push_back(star::SceneBuilder::GameObjects::Builder(StarEngine::sceneBuilder)
        //        .setPath(objectPath)
        //        .setPosition(glm::vec3{ -1.0f, 0.0f, -0.0f })
        //        .setScale(glm::vec3{ 1.5f, 1.5f, 1.5f })
        //        .build());
        //}
        ////table
        //{
        //    auto objectPath = mediaDirectoryPath + "models/table/Desk OBJ.obj";
        //    this->objectList.push_back(star::SceneBuilder::GameObjects::Builder(StarEngine::sceneBuilder)
        //        .setPath(objectPath)
        //        .setPosition(glm::vec3{ 0.0f, -0.4f, 0.0f })
        //        .setScale(glm::vec3{ 0.01f, 0.01f, 0.01f })
        //        .setMaterialFilePath(mediaDirectoryPath + "models/table/")
        //        .setTextureDirectory(mediaDirectoryPath + "models/table/textures/")
        //        .build());
        //}
        //rock
        //{
        //    auto objectPath = mediaDirectoryPath + "models/rock/898927_rock.obj";
        //    this->objectList.push_back(star::SceneBuilder::GameObjects::Builder(StarEngine::sceneBuilder)
        //        .setPath(objectPath)
        //        .setPosition(glm::vec3{ 0.0f, 0.0f, -0.85f })
        //        .setScale(glm::vec3{ 0.05f, 0.05f, 0.05f })
        //        .setMaterial(star::SceneBuilder::Materials::Builder(StarEngine::sceneBuilder)
        //            .setTexture(StarEngine::textureManager.addResource(star::FileHelpers::GetBaseFileDirectory(objectPath) + "textures/rock_low_Base_Color.png"))
        //            .setBumpMap(StarEngine::textureManager.addResource(star::FileHelpers::GetBaseFileDirectory(objectPath) + "textures/rock_low_Normal_DirectX.png"))
        //            .build())
        //        .build());
        //    this->rock = &StarEngine::sceneBuilder.entity(this->objectList.at(3));
        //}

        {
            auto objectPath = mediaDirectoryPath + "models/icoSphere/low_poly_icoSphere.obj";
            auto vertShaderPath = mediaDirectoryPath + "models/icoSphere/icoSphere.vert";
            auto fragShaderPath = mediaDirectoryPath + "models/icoSphere/icoSphere.frag";

            ////load light
            //this->lightList.push_back(star::SceneBuilder::Lights::Builder(this->sceneBuilder)
            //    .setType(star::Type::Light::directional)
            //    .setPosition(glm::vec3{ -2.0f, 2.0f, 0.0f })
            //    .setAmbient(glm::vec4{ 1.0f, 1.0f, 0.7f, 0.4f })
            //    .setDiffuse(glm::vec4{ 1.0f, 1.0f, 0.7, 1.0f })
            //    .setSpecular(glm::vec4{ 1.0f, 1.0f, 0.7f, 1.0f })
            //    .setDirection(glm::vec4{ 0.0f, -1.0f, 0.0f, 0.0f })
            //    .build());
            //sun = &this->sceneBuilder.light(this->lightList.at(0));

            //this->lightList.push_back(star::SceneBuilder::Lights::Builder(StarEngine::sceneBuilder)
            //    .setType(star::Type::Light::spot)
            //    .setPosition(glm::vec3{ -1.0f, 1.0f, 0.0f })
            //    .setDirection(glm::vec4{ 0.0f, -1.0f, 0.0f, 0.0f })
            //    .setDiameter(14.0f, 14.0f)
            //    .setAmbient(glm::vec4{ 1.0f, 1.0f, 1.0f, 0.01f })
            //    .setDiffuse(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f })
            //    .setSpecular(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f })
            //    .setLinkedObject(star::SceneBuilder::GameObjects::Builder(StarEngine::sceneBuilder)
            //        .setPath(objectPath)
            //        .setScale(glm::vec3{ 0.07f, 0.07f, 0.07f })
            //        .setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f })
            //        .setVertShader(StarEngine::shaderManager.addResource(vertShaderPath, std::make_unique<star::StarShader>(vertShaderPath)))
            //        .setFragShader(StarEngine::shaderManager.addResource(fragShaderPath, std::make_unique<star::StarShader>(fragShaderPath)))
            //        .build(false))
            //    .build());
            //spot = &StarEngine::sceneBuilder.light(this->lightList.at(this->lightList.size() - 1));

            //this->lightList.push_back(star::SceneBuilder::Lights::Builder(StarEngine::sceneBuilder)
            //    .setType(star::Type::Light::point)
            //    .setPosition(glm::vec3{ 0.4f, 0.4f, 0.0f })
            //    .setAmbient(glm::vec4{ 1.0f, 0.0f, 0.0f, 0.01f })
            //    .setDiffuse(glm::vec4{ 1.0f, 0.0f, 0.0f, 0.2f })
            //    .setSpecular(glm::vec4{ 1.0f, 0.0f, 0.0f, 0.2f })
            //    .setLinkedObject(star::SceneBuilder::GameObjects::Builder(StarEngine::sceneBuilder)
            //        .setPath(objectPath)
            //        .setScale(glm::vec3{ 0.07f, 0.07f, 0.07f })
            //        .setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f })
            //        .setVertShader(StarEngine::shaderManager.addResource(vertShaderPath, std::make_unique<star::StarShader>(vertShaderPath)))
            //        .setFragShader(StarEngine::shaderManager.addResource(fragShaderPath, std::make_unique<star::StarShader>(fragShaderPath)))
            //        .build(false))
            //    .build());
            //this->lightList.push_back(star::SceneBuilder::Lights::Builder(this->sceneBuilder)
            //    .setType(star::Type::Light::point)
            //    .setPosition(glm::vec3{ -1.0f, 0.4f, 0.5f })
            //    .setAmbient(glm::vec4{ 0.0f, 0.0f, 1.0f, 0.15f })
            //    .setDiffuse(glm::vec4{ 0.0f, 0.0f, 1.0f, 0.2f })
            //    .setSpecular(glm::vec4{ 0.0f, 0.0f, 1.0f, 0.2f })
            //    .build());
        }
    }
}

void Application::Load()
{
}

void Application::onWorldUpdate()
{
    bool update = false; 
    frameCounter = 0; 

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
        //applyStrokeAroundLocation(upTexLocX, upTexLocY, width); 

        //cast ray toward plane
        auto tail = this->camera.getPosition();
        auto lookDir = glm::normalize(this->camera.getLookDirection()); 
        auto head = this->camera.getPosition() + lookDir; 

        std::optional<glm::vec2> hitPoint = gridObj->getXYCoordsWhereRayIntersectsMe(tail, head);
        if (hitPoint.has_value()) {
            applyStrokeAroundLocation(hitPoint.value(), 2);
        }
    }

   /* auto now = std::chrono::steady_clock::now();
    float elapsedTime = std::chrono::duration<float>(now - timeSinceLastUpdate).count();
    rock->rotateRelative(star::Type::Axis::y, elapsedTime * 30);

    if (star::KeyStates::state(KEY::RIGHT)) {
        auto rot = glm::mat4(1.0f);
        float rotationAmt = (float)this->time.timeElapsedLastFrameSeconds() * sunSpeed;
        rot = glm::rotate(rot, rotationAmt, glm::vec3{ 0.0f, 0.0f, -1.0f });
        sun->direction = rot * sun->direction;
    }
    else if (star::KeyStates::state(KEY::LEFT)) {
        auto rot = glm::mat4(1.0f);
        float rotationAmt = (float)this->time.timeElapsedLastFrameSeconds() * sunSpeed;
        rot = glm::rotate(rot, rotationAmt, glm::vec3{ 0.0f, 0.0f, 1.0f });
        sun->direction = rot * sun->direction;
    }

    if (star::KeyStates::state(KEY::DOWN)) {
        spot->setOuterDiameter(spot->getOuterDiameter() + (spotSpeed * elapsedTime));
        std::cout << spot->getOuterDiameter() << std::endl;
    }
    else if (star::KeyStates::state(KEY::Y)) {
        spot->setOuterDiameter(spot->getOuterDiameter() - (spotSpeed * elapsedTime));
        std::cout << spot->getOuterDiameter() << std::endl;
    }

    if (star::KeyStates::state(KEY::J)) {
        spot->setInnerDiameter(spot->getInnerDiameter() + (spotSpeed * elapsedTime));
        std::cout << spot->getInnerDiameter() << std::endl;
    }
    else if (star::KeyStates::state(KEY::H)) {
        spot->setInnerDiameter(spot->getInnerDiameter() - (spotSpeed * elapsedTime));
        std::cout << spot->getInnerDiameter() << std::endl;
    }
    this->time.updateLastFrameTime();*/
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

void Application::applyStrokeAroundLocation(glm::vec2 location, int width)
{
    std::vector<int> locsX, locsY;
    std::vector<star::Color> colors;

    for (int i = 0; i < width*2; i++) {
        auto locationX = 0;
        auto locationY = 0;
        if (i < width) {
            locationX = location.x - i;
            locationY = location.y - i;
        }
        else {
            locationX = location.x + i;
            locationY = location.y + i;
        }


        for (int j = 0; j < i * 2; j++) {
            auto newColor = gridObj->getTexColorAt(upTexLocX, upTexLocY + j);
            if (newColor.g() < 255) {
                newColor = star::Color{
                newColor.r(),
                255,
                newColor.b(),
                255 };
            }

            if (canApplyColorToLocation(locationX, locationY + j, newColor)) {
                locsX.push_back(locationX);
                locsY.push_back(locationY + j);
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

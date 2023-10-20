#pragma once 

#include "Square.hpp"
#include "StarEngine.hpp"
#include "StarApplication.hpp"
#include "ConfigFile.hpp"
#include "Time.hpp"
#include "Interactivity.hpp"
#include "DebugHelpers.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "LightManager.hpp"
#include "Handle.hpp"
#include "KeyStates.hpp"
#include "Key.hpp"
#include "BasicObject.hpp"
#include "StarScene.hpp"
#include "Grid.hpp"
#include "Time.hpp"
#include "UpdateTextureRenderer.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.hpp>

#include <string> 
#include <memory> 


class Application :
    public star::StarApplication
{
public:
    Application(star::StarScene& scene);

    void Load();

    virtual std::string getApplicationName() { return "Starlight Application"; }

    void onWorldUpdate() override; 

    void onKeyPress(int key, int scancode, int mods) override;

    virtual std::unique_ptr<star::StarRenderer> getRenderer(star::StarDevice& device, star::StarWindow& window, star::RenderOptions& options) override;

protected:
    void applyStrokeAroundLocation(glm::vec2 worldCenterPoint, int width, int centerStrength);

    bool canApplyColorToLocation(int upTexLocX, int upTexLocY, star::Color color); 

private:
    star::Time point; 
    double timeCounter = 0; 
    int strength = 50;
    int width = 10;
    double mouseXPos = 0, mouseYPos = 0; 
    bool wasClick = false; 
    Grid* gridObj = nullptr; 

    static int screen_x, screen_y; 
    static int disabledLightCounter;
    static bool upCounter;
    static bool rotatingCounterClock;
    static std::chrono::steady_clock::time_point timeSinceLastUpdate;

    // Inherited via StarApplication
    void onKeyRelease(int key, int scancode, int mods) override;
    void onMouseMovement(double xpos, double ypos) override;
    void onMouseButtonAction(int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
};

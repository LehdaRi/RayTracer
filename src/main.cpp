#include "Scene.hpp"
#include "BasicCamera.hpp"
#include "PointLight.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include <iostream>


int main(void) {
    // Declare and create a new window
    sf::ContextSettings settings;
    settings.depthBits         = 24; // Request a 24 bits depth buffer
    settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
    settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    sf::Window window(sf::VideoMode(800, 600, 32), "RayTracer", sf::Style::Default, settings);
    // Activate the window for OpenGL rendering
    window.setActive();
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);
    //  Initialize GLEW
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //  Scene
    Scene scene;
    scene.loadFromObj("res/scenes/cornell.obj");

    //  Light
    PointLight light({0.55f, -0.25f, -0.6f}, {1.0f, 1.0f, 1.0f});
    //scene.addLight(&light);

    //  Camera
    BasicCamera camera(400, 300, PI*0.5f, 4.0f/3.0f, 0.2f, 4.0f);
    camera.lookAt( {0.7f, -0.8f, -0.95f},
                   {0.0f, -0.5f, 0.0f},
                   {0.0f, 1.0f, 0.0f} );

    //  Mesh
    Mesh mesh;
    mesh.loadFromObj("res/scenes/cornell.obj");

    //  Shader
    Shader shader("res/shaders/VS_Simple.glsl", "res/shaders/FS_Simple.glsl");

    float t = 0.0f;

    // The main loop - ends as soon as the window is closed
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
            break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                case sf::Keyboard::Escape:
                    window.close();
                break;
                case sf::Keyboard::Space:
                    //  Render
                    camera.render(scene, &light, "render.png");
                break;
                default:
                break;
                }
            default:
            break;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.render(shader, camera);
        camera.lookAt( {0.8f*cosf(t*PI*2*0.125f), 0.5f+0.1f*cosf(t*PI*2*0.021f), 0.8f*sinf(t*PI*2*0.125f)},
                       {0.0f, 0.0f, 0.0f},
                       {0.0f, 1.0f, 0.0f} );

        t += 1.0f/60.0f;

        window.display();
    }

    return 0;
}

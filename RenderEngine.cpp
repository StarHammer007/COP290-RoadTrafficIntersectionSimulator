#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "RenderEngine.h"

RenderEngine::RenderEngine(Road* targetRoad) {
    std::cout << "Instantiated RenderEngine for road " << targetRoad->id << std::endl;
    this->targetRoad = targetRoad;
    // Dividing by this factor gives it in viewport dimensions
    this->scalex = 25;
    this->scaley = 50;
    this->signalSize = 1;
    // Set the default background color
    this->bgcolor.push_back(1.0f);
    this->bgcolor.push_back(0.968f);
    this->bgcolor.push_back(0.3529f);
    this->monitorWidth = 800;
    this->monitorHeight = 600;
}

// Default constructor
RenderEngine::RenderEngine() {
    // Do nothing
    // std::cout << "RenderEngine() default constructor called" << std::endl;
}

void RenderEngine::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Function which receives the key events
void RenderEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Set the close_flag to true
        std::cout << "Exit keypress" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Initalize GLFW stuff
void RenderEngine::setup() {
    std::cout << "Setting up the RenderEngine" << std::endl;
    // Initialize GLFW, return error otherwise
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set the error_callback function
    glfwSetErrorCallback(RenderEngine::error_callback);

    // Create a window, context
    std::cout << "Creating a window" << std::endl;
    RenderEngine::window = glfwCreateWindow(this->monitorWidth, this->monitorHeight, "TrafficSim", NULL, NULL);

    if (!RenderEngine::window) {
        // Context creation failed
        glfwTerminate();
        std::cout << "Could not create window" << std::endl;
        exit(EXIT_FAILURE);
    }


    RenderEngine::isInitialized = true;
    // Make the context current
    glfwMakeContextCurrent(RenderEngine::window);
    // Set the swap interval
    glfwSwapInterval(1);
    // Set the key_callback method
    glfwSetKeyCallback(RenderEngine::window, RenderEngine::key_callback);
}

float RenderEngine::getTime() {
    float time = glfwGetTime();
    return time;
}

void RenderEngine::render(double delT) {
    double beginTime = RenderEngine::getTime();
    double oldTime = RenderEngine::getTime();
    double currentTime = RenderEngine::getTime();
    std::cout << "Starting Render routine"<< std::endl;
    bool update = false;
    while((currentTime - beginTime < delT) && !glfwWindowShouldClose(RenderEngine::window)) {
    	if (currentTime - oldTime >= 0.04) {
        	// Update the simulation based on previously decided parameters, set new parameters
        	this->targetRoad->updateSim(currentTime - oldTime);
    		update = true;
    	}

        float ratio;
        int width=800, height=800;
        glfwGetFramebufferSize(RenderEngine::window, &width, &height);

        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the background
        glClearColor((float)this->bgcolor[0], (float)this->bgcolor[1], (float)this->bgcolor[2], 1.0f);
        // Render the road
        RenderEngine::renderRoad();

        // Iterate over the vehicles
        for(auto v: this->targetRoad->vehicles) {
            renderVehicle(v);
        }

        // Swap buffers and check for events
        glfwSwapBuffers(RenderEngine::window);
        glfwPollEvents();
	if (update) {
		update = false;
        	oldTime = currentTime;
	}	
        currentTime = glfwGetTime();
        #ifdef RENDER_ENGINE_H
        RenderEngine::getTime();
        #else
        #endif
    }
}

void RenderEngine::renderRoad() {
    // Render the road in gray
    glColor3f(0.2f, 0.2f, 0.2f);
    float ycoord = this->targetRoad->width/((float)this->scaley);
    float xcoord = this->targetRoad->length/(float)this->scalex - 1.0;
    glRectd(-1.0f, ycoord, xcoord, -ycoord);
    // std::cout <<"Roadwa "<< ycoord<<" " << xcoord << std::endl;
    // Render the signal as a strip
    float xsignal = this->targetRoad->signalPosition/(float)this->scalex - 1.0;
    glColor3f((float)this->targetRoad->signal_rgb[0]/255.0f, (float)this->targetRoad->signal_rgb[1]/255.0f, (float)this->targetRoad->signal_rgb[2]/255.0f);
    glRectd(xsignal, ycoord, xsignal + this->signalSize/(float)this->scalex, -ycoord);
    int lanes = this->targetRoad->lanes;
    double lanewidth = this->targetRoad->width/(double)this->targetRoad->lanes;
    for(int i=0;i<lanes-1;i++){
      glColor3f(1.0f,1.0f,1.0f);
      float ystart = -ycoord + 2*(i+1)*lanewidth/(float)this->scaley;
      float width = 0.01f;
      glRectd(-1.0f, ystart, xcoord, ystart + width);
    }
}

void RenderEngine::endSim() {
    glfwTerminate();
}

void RenderEngine::renderVehicle(Vehicle* vehicle) {
    if (vehicle->isOnRoad) {
        // Render only if the vehicle is on the Road
        float x = -1.0 + (float)vehicle->currentPosition.first/(float)this->scalex;
        float y = 2*( - (float)this->targetRoad->width/2 + (float)vehicle->currentPosition.second)/(this->scaley);
        float delx = vehicle->length/(float)this->scalex;
        float dely = 2*vehicle->width/(float)this->scaley;

        // Set the correct color
        glColor3f((float)vehicle->color_rgb[0]/255.0f,
        (float)vehicle->color_rgb[1]/255.0f,
        (float)vehicle->color_rgb[2]/255.0f);

        // Render the rectangle
        // std::cout << "Vehiclewa "<<vehicle->type<<" "<<vehicle->width<<" "<<delx << " " <<dely << std::endl;
        glRectd(x, y, x -  delx, y - dely);
    }
}

// Created by Indeximal 08.04.2018

#include "IndeximalGameEngine.h"

bool wireframeOn = false;
bool quadScreenOn = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		wireframeOn = !wireframeOn;
	} else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		quadScreenOn = !quadScreenOn;
	}
}

int main(int argc, char *argv[]) {

	ige::Display display(1080, 720, false, "Example Display");
	glfwSetKeyCallback(display.getNativeWindowPtr(), key_callback);

	ige::Camera camera(display, { -2.0f, 0.0f, 2.0f }, 80.0f, 0.1f, 100.0f);

	ige::GBuffer framebuffer(display.getWidth(), display.getHeight());
	framebuffer.setClearColor(129 / 256.0f, 199 / 256.0f, 249 / 256.0f);

	ige::StaticShader shader;
	ige::GeometryPassShader gShader;
	gShader.projectionMatrix = camera.getProjectionMatrix();

	//ige::Model frog = ige::loadModelFromObjFile("res/Frog.obj");
	//ige::Model dog("res/LowPolyDog.obj");
	//glm::mat4 modelMat = glm::scale(glm::mat4(), glm::vec3(1.0f));

	ige::Model treeModel("res/SimpleUVTree.obj");
	ige::Texture treeTex("res/TreeUVTex.png");
	ige::Entity<ige::GeometryPassShader> tree(treeModel, treeTex, gShader, { 0.0f, 0.0f, 0.0f }, 0.6f);
	ige::Entity<ige::GeometryPassShader> tree1(treeModel, treeTex, gShader, { 1.0f, 3.0f, 0.0f }, 0.55f);
	ige::Entity<ige::GeometryPassShader> tree2(treeModel, treeTex, gShader, { -2.0f, 1.0f, 0.0f }, 0.4f);
	ige::Entity<ige::GeometryPassShader> tree3(treeModel, treeTex, gShader, { -3.0f, -2.0f, 0.0f }, 0.7f);

	glm::vec3 lightDirection = { 1.0f, -1.0f, -2.0f };
	shader.lightDirection = lightDirection;
	shader.projectionMatrix = camera.getProjectionMatrix();

	ige::LightingPassShader lightingShader;
	lightingShader.lightDirection = lightDirection;

	ige::Shader2D quadShader;
	ige::Geometry2DShader gQuadShader;

	//ige::Quad smallQuad(-0.5f, -0.5f, 0.5f, 0.5f);
	ige::Quad fullQuad;
	ige::Quad topLeftQuad(-1.0f, 0.0f, 0.0f, 1.0f);
	ige::Quad topRightQuad(0.0f, 0.0f, 1.0f, 1.0f);
	ige::Quad bottomLeftQuad(-1.0f, -1.0f, 0.0f, 0.0f);
	ige::Quad bottomRightQuad(0.0f, -1.0f, 1.0f, 0.0f);

	ige::Texture awesomeFace("res/awesomeface.png");

	ige::setAlphaBlending(false);
	ige::setCulling(true);

	ige::Terrain terrain(ige::getRandomSeed(), 50, 50, 1.5f, 3, 0.17f, 2.0f, 2.0f, 0.5f);
	terrain.move({ -20.0f, -20.0f, 0.2f });


	while (display.update()) {
		camera.update();

		/* --- Geometry Pass --- */
		ige::renderToFramebuffer(framebuffer);
		ige::setWireframe(wireframeOn);
		ige::setDepthTest(true);
		framebuffer.clear();

		// Tree
		ige::useShader(&gShader);
		gShader.viewMatrix = camera.getViewMatrix();
		tree.render();
		tree1.render();
		tree2.render();
		tree3.render();

		//ige::useShader(&gQuadShader);
		//gQuadShader.texture = awesomeFace;
		//ige::renderQuad(smallQuad);

		// Terrain
		//terrain.render(camera, lightDirection);


		/* --- Lighting Pass --- */
		ige::renderToDisplay(&display);
		ige::setWireframe(false);
		ige::setDepthTest(false);


		/* --- Split Screen --- */
		if (quadScreenOn) {
			ige::useShader(&quadShader);
			quadShader.texture = framebuffer.getColorBuffer();
			ige::renderQuad(topLeftQuad);
			quadShader.texture = framebuffer.getNormalBuffer();
			ige::renderQuad(topRightQuad);
			quadShader.texture = framebuffer.getPositionBuffer();
			ige::renderQuad(bottomLeftQuad);
			quadShader.texture = framebuffer.getDepthBuffer();
			ige::renderQuad(bottomRightQuad);
		} else {
			/* --- Lighting Pass --- */
			ige::useShader(&lightingShader);
			lightingShader.albedoTexture = framebuffer.getColorBuffer();
			lightingShader.normalTexture = framebuffer.getNormalBuffer();
			lightingShader.positionTexture = framebuffer.getPositionBuffer();
			lightingShader.viewPosition = camera.getPosition();

			ige::renderQuad(fullQuad);
		}
	}

	return 0;
}
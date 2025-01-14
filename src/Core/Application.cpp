#include "Application.h"

#include <string>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void Application::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    cam.mouse_callback(xpos, ypos);
}

static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

Application::Application()
{
	glViewport(0, 0, START_WIDTH, START_HEIGHT);
	glfwSetCursorPosCallback(m_Window.getGLFWwindow(), mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(message_callback, nullptr);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window.getGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	m_FB = new Framebuffer(
		Framebuffer::CreateInfo{
			std::span<const Framebuffer::Attachment>{
				std::array<Framebuffer::Attachment, 4>{
					Framebuffer::Attachment{GL_COLOR_ATTACHMENT0, START_WIDTH, START_HEIGHT, GL_RGBA16F},
					Framebuffer::Attachment{GL_COLOR_ATTACHMENT1, START_WIDTH, START_HEIGHT, GL_RGB16F},
					Framebuffer::Attachment{GL_COLOR_ATTACHMENT2, START_WIDTH, START_HEIGHT, GL_RGB16F},
					Framebuffer::Attachment{GL_DEPTH_ATTACHMENT, START_WIDTH, START_HEIGHT}
				}
			}
		}
	);

	lights.push_back({});

	Texture cubemapTex{ {"res/textures/skybox.png", GL_TEXTURE_CUBE_MAP} };

	Shader postShader;
	postShader.AttachShader({ "res/shaders/post.frag", GL_FRAGMENT_SHADER });
	postShader.AttachShader({ "res/shaders/post.vert", GL_VERTEX_SHADER });

	Shader gridShader;
	gridShader.AttachShader({ "res/shaders/grid.frag", GL_FRAGMENT_SHADER });
	gridShader.AttachShader({ "res/shaders/grid.vert", GL_VERTEX_SHADER });

	Shader cubemapShader;
	cubemapShader.AttachShader({ "res/shaders/cubemap.frag", GL_FRAGMENT_SHADER });
	cubemapShader.AttachShader({ "res/shaders/cubemap.vert", GL_VERTEX_SHADER });

	m_PostComp = new Composite(std::move(postShader));
	m_GridComp = new Composite(std::move(gridShader));
	cubemap = new Cube({ std::move(cubemapTex), true, std::move(cubemapShader) });
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::run()
{
	float currentFrame = glfwGetTime();
	float lastFrame = currentFrame;
	float dt = 0;

	while (!m_Window.shouldClose())
	{
		glfwPollEvents();
		handleInputs();
		
		currentFrame = glfwGetTime();
		dt = std::max(currentFrame - lastFrame, 0.05f);
		lastFrame = currentFrame;

		if (m_Window.wasWindowResized())
		{
			m_Window.resetWindowResizeFlag();
			glViewport(0, 0, m_Window.getWindowRes().x, m_Window.getWindowRes().y);
			cam.setAspectRatio(m_Window.getWindowRes().x/m_Window.getWindowRes().y);
			m_FB->changeRes(m_Window.getWindowRes().x, m_Window.getWindowRes().y, 0);	// G_ALBEDO
			m_FB->changeRes(m_Window.getWindowRes().x, m_Window.getWindowRes().y, 1);	// G_POSITION
			m_FB->changeRes(m_Window.getWindowRes().x, m_Window.getWindowRes().y, 2);	// G_NORMAL
		}

		cam.update(m_Window.getGLFWwindow());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_FB->bind();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClearDepth(0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		cubemap->Draw({ cam.getProjMatrix(), glm::mat4(glm::mat3(cam.getViewMatrix())), cam.getPosition() });
		glDepthMask(GL_TRUE);

		if (isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// OBJECTS DRAW

		//plane.Draw({cam.getProjMatrix(), cam.getViewMatrix()});
		model.Draw({ cam.getProjMatrix(), cam.getViewMatrix(), cam.getPosition() });

		// END

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_GridComp->Draw({ true, cam.getPosition(), {}, cam.getViewMatrix(), cam.getProjMatrix()});

		m_FB->unbind();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTextureUnit(0, m_FB->getTexHandle(0));
		glBindTextureUnit(1, m_FB->getTexHandle(1));
		glBindTextureUnit(2, m_FB->getTexHandle(2));
		m_FB->bindDepthTex(0);

		m_PostComp->Draw({ false, cam.getPosition(), lights });

		ImGui::Begin("Main");
		if (ImGui::Button("Add Light"))
		{
			lights.push_back({});
		}
		ImGui::End();

		ImGui::Begin("Light");
		int i = 0;
		for (const Light& light : lights)
		{
			std::string pos = "Pos " + std::to_string(i);
			std::string color = "Color " + std::to_string(i);
			ImGui::DragFloat3(pos.c_str(), (float*)glm::value_ptr(light.pos), 0.5f);
			ImGui::ColorEdit3(color.c_str(), (float*)glm::value_ptr(light.color));
			i++;
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window.getGLFWwindow());
	}
}

void Application::handleInputs()
{
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_O) == GLFW_PRESS)	// Wire frame
	{
		isWireframe = true;
	}

	else if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_P) == GLFW_PRESS) // Default
	{
		isWireframe = false;
	}
	
	else if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) // Quit
	{
		glfwSetWindowShouldClose(m_Window.getGLFWwindow(), true);
	}
}
#pragma once

//std
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

//vendor
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Vendor/imGUI/imgui_impl_glfw.h"
#include "Vendor/imGUI/imgui_impl_opengl3.h"


//define
#include "LNdefine.h"

//src
#include "LN/Core/Log.h"
#include "LN/Core/Assert.h"
#include "LN/Core/Window.h"
#include "LN/Core/LayerStack.h"

#include "LN/Renderer/VertexBuffer.h"
#include "LN/Renderer/IndexBuffer.h"
#include "LN/Renderer/VertexBufferLayout.h"
#include "LN/Renderer/VertexArray.h"
#include "LN/Renderer/shader.h"
#include "LN/Renderer/Renderer.h"
#include "LN/Renderer/Texture.h"
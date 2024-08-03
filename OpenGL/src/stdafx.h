#pragma once

//std
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <memory>

//vendor
#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Vendor/imGUI/imgui_impl_glfw.h"
#include "Vendor/imGUI/imgui_impl_opengl3.h"


//src
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "shader.h"
#include "Renderer.h"
#include "Texture.h"
#pragma once

// This file is meant to expose everything necessary to use all features of the engine to an outside project, such as the editor, or a game project.

#include "Engine.h"
#include "Application.h"

// Core
#include "Core/Types.h"
#include "Core/Common.h"
#include "Core/Math/Mat.h"
#include "Core/Math/Vec.h"
#include "Core/Logger.h"
#include "Core/StringID.h"

// Graphics
#include "Graphics/Models/ModelManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Sampler.h"
#include "Graphics/DXUtil.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"

// Input
#include "Input/Input.h"

// Scene
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/Scene.h"

// Window
#include "Window/Window.h"
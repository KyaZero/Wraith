#pragma once

// This file is meant to expose everything necessary to use all features of the engine to an outside project, such as
// the editor, or a game project.

#include "Application.h"
#include "Engine.h"

// Core
#include "Core/Common.h"
#include "Core/Logger.h"
#include "Core/Math/Mat.h"
#include "Core/Math/Vec.h"
#include "Core/StringID.h"
#include "Core/Types.h"

// Graphics
#include "Graphics/Camera.h"
#include "Graphics/DXUtil.h"
#include "Graphics/Models/ModelManager.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/TextureManager.h"

// Input
#include "Input/Input.h"

// Scene
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/ScriptableEntity.h"

// Window
#include "Window/Window.h"
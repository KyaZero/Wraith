[![Windows](https://github.com/KyaZero/Wraith/actions/workflows/windows.yml/badge.svg)](https://github.com/KyaZero/Wraith/actions/workflows/windows.yml)

# Wraith
A hobby game engine, currently supports very basic 3D, and 2D with sprites and text.

## Current screenshot of the project
![](https://i.imgur.com/heEW7pR.png "Editor")


## How to generate and open the project
```shell
# cmd
project_root> scripts/gen.bat
project_root> scripts/open.bat

# bash
project_root> scripts/gen
project_root> scripts/open

# you can also pass params to cmake
project_root> scripts/gen -Ax64 -DCMAKE_UNITY_BUILD=ON
```


## Thirdparty libraries
* [Dear ImGui](https://github.com/ocornut/imgui)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [stb](https://github.com/nothings/stb#stb_libs)
* [entt](https://github.com/skypjack/entt)
* [color-console](https://github.com/imfl/color-console)
* [glfw](https://github.com/glfw/glfw)
* [harfbuzz](https://github.com/harfbuzz/harfbuzz)
* [freetype](https://github.com/freetype/freetype)
* [fribidi](https://github.com/fribidi/fribidi)
* [msdfgen](https://github.com/Chlumsky/msdfgen)
* [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/)
* [dubu-serialize](https://github.com/Husenap/dubu-serialize)
* [dubu-rect-pack](https://github.com/Husenap/dubu-rect-pack)

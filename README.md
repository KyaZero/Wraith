[![Windows](https://github.com/KyaZero/Wraith/actions/workflows/windows.yml/badge.svg)](https://github.com/KyaZero/Wraith/actions/workflows/windows.yml)

# Wraith
A hobby game engine, currently only supports 2D

## Current screenshot of the project
![](https://i.imgur.com/ThYudl9.png "Editor")


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
* [stb](https://github.com/nothings/stb#stb_libs)
* [entt](https://github.com/skypjack/entt)
* [color-console](https://github.com/imfl/color-console)
* [glfw](https://github.com/glfw/glfw)

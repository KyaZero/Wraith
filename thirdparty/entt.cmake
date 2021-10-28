message("-- Fetching External Project: entt")

FetchContent_Declare(
  entt
  GIT_REPOSITORY  https://github.com/skypjack/entt.git
  GIT_TAG         v3.8.1
)

FetchContent_MakeAvailable(entt)

set_target_properties(aob PROPERTIES FOLDER "thirdparty/entt")
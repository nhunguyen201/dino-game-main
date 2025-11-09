CFLAGS		+= -I./sources/app/game/dino_game
CPPFLAGS	+= -I./sources/app/game/dino_game

VPATH += sources/app/game/dino_game

# CPP source files
SOURCES_CPP += sources/app/game/dino_game/dino_cloud.cpp
SOURCES_CPP += sources/app/game/dino_game/dino_game.cpp
SOURCES_CPP += sources/app/game/dino_game/dino_obstacle.cpp
SOURCES_CPP += sources/app/game/dino_game/dino_player.cpp
SOURCES_CPP += sources/app/game/dino_game/rect.cpp
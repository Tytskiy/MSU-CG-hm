#include "Level.h"

Level::Level(const std::string &levelPath, const Size &numTiles) :
        sizeOfLevel{TILE_SIZE * numTiles.width, TILE_SIZE * numTiles.height} {

    std::ifstream tmp(levelPath);
    std::string textLevel((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());

    textLevel.erase(std::remove(textLevel.begin(), textLevel.end(), '\n'), textLevel.end());

    staticObjects = new Sprite({255, 255, 255, 255}, {sizeOfLevel.width, sizeOfLevel.height});
    collisionMap = new bool[sizeOfLevel.width * sizeOfLevel.height]{false};

    for (int k = 0; k < textLevel.size(); k++) {
        auto currSprite = levelSprites[textLevel[k]];
        auto currPosition = Point{TILE_SIZE * (k % numTiles.width), TILE_SIZE * (k / numTiles.width)};
        if (textLevel[k] == '@') {
            playerPos = currPosition;
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == '%') {
            dynamicObjects.emplace_back(new DestructObject(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == ' ') {
            dynamicObjects.emplace_back(new Trap(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == 'x') {
            dynamicObjects.emplace_back(new Portal(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == 'D') {
            dynamicObjects.emplace_back(new Door(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == 'T') {
            dynamicObjects.emplace_back(new Spikes(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == 'K') {
            dynamicObjects.emplace_back(new Key(currPosition, {TILE_SIZE, TILE_SIZE}, 0));
            setTile(levelSprites['.'], currPosition);
        } else if (textLevel[k] == '#') {
            setTile(currSprite, currPosition);
            setCollision(currPosition);
        } else if (textLevel[k] == '.') {
            setTile(currSprite, currPosition);
        }
    }

}

Level::~Level() {
    delete staticObjects;
    delete collisionMap;
    for (auto c:dynamicObjects) {
        delete c;
    }
    dynamicObjects.clear();
}

void Level::setTile(const Sprite &sprite, const Point &topLeft) {
    for (int i = 0; i < TILE_SIZE; i++) {
        for (int j = 0; j < TILE_SIZE; j++) {
            staticObjects->setPixel({topLeft.x + j, topLeft.y + i}, sprite({j, i}));
        }
    }
}

void Level::setCollision(const Point &topLeft) {
    for (int i = 0; i < TILE_SIZE; i++) {
        for (int j = 0; j < TILE_SIZE; j++) {
            collisionMap[sizeOfLevel.width * (topLeft.y + i) + j + topLeft.x] = true;
        }
    }
}

std::list<GameObject *> &Level::getDO() {
    return dynamicObjects;
}


const Point &Level::getPlayerPosition() const {
    return playerPos;
}

bool *Level::getCollisionMap() const {
    return collisionMap;
}

Sprite &Level::getStaticObjects() const {
    return *staticObjects;
}

Size Level::getSize() const {
    return sizeOfLevel;
}

const Point &Level::getLevelPosition() const {
    return levelPos;
}

void Level::setLevelPosition(const Point &p) {
    levelPos = p;
}

#include "Level8.h"
#define LEVEL8_WIDTH 43
#define LEVEL8_HEIGHT 8

#define LEVEL8_ENEMY_COUNT 29

unsigned int level8_data[] =
{
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level8::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL8_WIDTH, LEVEL8_HEIGHT, level8_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("spaceship.png");

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.fire = new Entity();
    state.fire->isActive = false;
    state.fire->entityType = ATTACK;
    state.fire->position = state.player->position;
    state.fire->movement = glm::vec3(1.0, 0.0, 0.0);
    state.fire->velocity = glm::vec3(1.0, 0.0, 0.0);
    state.fire->speed = 4.0f;
    state.fire->textureID = Util::LoadTexture("fire.png");

    state.enemies = new Entity[LEVEL8_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("alien.png");
    
    for (int i = 0; i < LEVEL8_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 1;
        state.enemies[i].velocity = glm::vec3(1);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].aiType = JUMPER;
        state.enemies[i].aiState = IDLE;

        state.enemies[i].width = 0.7;
        state.enemies[i].height = 0.7;
    }

    state.enemies[0].position = glm::vec3(7, -1, 0);
    state.enemies[0].isActive = false;

    state.enemies[1].position = glm::vec3(9, -1, 0);
    state.enemies[2].position = glm::vec3(11, -1, 0);
    state.enemies[3].position = glm::vec3(13, -1, 0);
    state.enemies[4].position = glm::vec3(15, -1, 0);
    state.enemies[5].position = glm::vec3(17, -1, 0);
    state.enemies[6].position = glm::vec3(19, -1, 0);
    state.enemies[7].position = glm::vec3(21, -1, 0);
    state.enemies[8].position = glm::vec3(23, -1, 0);
    state.enemies[9].position = glm::vec3(25, -1, 0);
    state.enemies[10].position = glm::vec3(27, -1, 0);
    state.enemies[11].position = glm::vec3(29, -1, 0);
    state.enemies[12].position = glm::vec3(31, -1, 0);
    state.enemies[13].position = glm::vec3(33, -1, 0);
    state.enemies[14].position = glm::vec3(35, -1, 0);

    state.enemies[15].position = glm::vec3(9, -6, 0);
    state.enemies[16].position = glm::vec3(11, -6, 0);
    state.enemies[17].position = glm::vec3(13, -6, 0);
    state.enemies[18].position = glm::vec3(15, -6, 0);
    state.enemies[19].position = glm::vec3(17, -6, 0);
    state.enemies[20].position = glm::vec3(19, -6, 0);
    state.enemies[21].position = glm::vec3(21, -6, 0);
    state.enemies[22].position = glm::vec3(23, -6, 0);
    state.enemies[23].position = glm::vec3(25, -6, 0);
    state.enemies[24].position = glm::vec3(27, -6, 0);
    state.enemies[25].position = glm::vec3(29, -6, 0);
    state.enemies[26].position = glm::vec3(31, -6, 0);
    state.enemies[27].position = glm::vec3(33, -6, 0);
    state.enemies[28].position = glm::vec3(35, -6, 0);
}

void Level8::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, state.map);

    if (state.fire->isActive) {
        state.fire->Update(deltaTime, state.player, state.enemies, state.map);

        if (glm::distance(state.fire->position, state.player->position) > 3.0f) {
            state.fire->isActive = false;
            state.player->shootFire = false;
        }

        if (!state.fire->isActive && !state.enemies->isActive) {
            state.player->shootFire = false;
        }
    }
    for (int i = 0; i < LEVEL8_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.map);
    }

    if (state.player->position.x >= 35) {
        state.nextScene = 9;
    }
}
void Level8::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL8_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    if (state.fire->isActive) {
        state.fire->Render(program);
    }
}

void Level8::Reset() {
    Level8::Initialize();
}
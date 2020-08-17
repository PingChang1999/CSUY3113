#include "Level10.h"
#define LEVEL10_WIDTH 172
#define LEVEL10_HEIGHT 8

#define LEVEL10_ENEMY_COUNT 99

unsigned int level10_data[] =
{
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level10::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL10_WIDTH, LEVEL10_HEIGHT, level10_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 5.0f;
    state.player->textureID = Util::LoadTexture("spaceship.png");

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 7.0f;

    state.fire = new Entity();
    state.fire->isActive = false;
    state.fire->entityType = ATTACK;
    state.fire->position = state.player->position;
    state.fire->movement = glm::vec3(1.0, 0.0, 0.0);
    state.fire->velocity = glm::vec3(1.0, 0.0, 0.0);
    state.fire->speed = 4.0f;
    state.fire->textureID = Util::LoadTexture("fire.png");


    state.enemies = new Entity[LEVEL10_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("alien.png");

    for (int i = 0; i < LEVEL10_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 1;
        state.enemies[i].velocity = glm::vec3(1);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].aiState = IDLE;

        state.enemies[i].width = 0.7;
        state.enemies[i].height = 0.7;
    }

    for (int i = 0; i < 15; i++) {
        state.enemies[i].aiType = WALKER;
    }

    for (int i = 15; i < 51; i++) {
        state.enemies[i].aiType = JUMPER;
    }

    for (int i = 51; i < LEVEL10_ENEMY_COUNT; i++) {
        state.enemies[i].aiType = WALKER;
    }

    state.enemies[0].position = glm::vec3(7, -1, 0);
    state.enemies[0].isActive = false;

    state.enemies[1].position = glm::vec3(9, -3, 0);
    state.enemies[2].position = glm::vec3(11, -3, 0);
    state.enemies[3].position = glm::vec3(13, -2, 0);
    state.enemies[4].position = glm::vec3(15, -2, 0);
    state.enemies[5].position = glm::vec3(17, -2, 0);
    state.enemies[6].position = glm::vec3(19, -4, 0);
    state.enemies[7].position = glm::vec3(21, -4, 0);
    state.enemies[8].position = glm::vec3(23, -4, 0);
    state.enemies[9].position = glm::vec3(25, -4, 0);
    state.enemies[10].position = glm::vec3(27, -3, 0);
    state.enemies[11].position = glm::vec3(29, -3, 0);
    state.enemies[12].position = glm::vec3(31, -3, 0);
    state.enemies[13].position = glm::vec3(33, -4, 0);
    state.enemies[14].position = glm::vec3(35, -4, 0);

    state.enemies[15].position = glm::vec3(40, -1, 0);
    state.enemies[16].position = glm::vec3(40, -6, 0);
    state.enemies[17].position = glm::vec3(44, -1, 0);
    state.enemies[18].position = glm::vec3(44, -6, 0);
    state.enemies[19].position = glm::vec3(46, -1, 0);
    state.enemies[20].position = glm::vec3(46, -6, 0);
    state.enemies[21].position = glm::vec3(50, -1, 0);
    state.enemies[22].position = glm::vec3(50, -6, 0);
    state.enemies[23].position = glm::vec3(52, -1, 0);
    state.enemies[24].position = glm::vec3(52, -6, 0);
    state.enemies[25].position = glm::vec3(56, -1, 0);
    state.enemies[26].position = glm::vec3(56, -6, 0);
    state.enemies[27].position = glm::vec3(58, -1, 0);
    state.enemies[28].position = glm::vec3(58, -6, 0);
    state.enemies[29].position = glm::vec3(60, -1, 0);
    state.enemies[30].position = glm::vec3(60, -6, 0);
    state.enemies[31].position = glm::vec3(62, -1, 0);
    state.enemies[32].position = glm::vec3(62, -6, 0);


    state.enemies[33].position = glm::vec3(66, -1, 0);
    state.enemies[34].position = glm::vec3(66, -6, 0);
    state.enemies[35].position = glm::vec3(70, -3, 0);
    state.enemies[36].position = glm::vec3(70, -4, 0);
    state.enemies[37].position = glm::vec3(72, -3, 0);
    state.enemies[38].position = glm::vec3(72, -4, 0);
    state.enemies[39].position = glm::vec3(76, -1, 0);
    state.enemies[40].position = glm::vec3(76, -6, 0);
    state.enemies[41].position = glm::vec3(78, -1, 0);
    state.enemies[42].position = glm::vec3(78, -6, 0);
    state.enemies[43].position = glm::vec3(82, -3, 0);
    state.enemies[44].position = glm::vec3(82, -4, 0);
    state.enemies[45].position = glm::vec3(84, -3, 0);
    state.enemies[46].position = glm::vec3(84, -4, 0);
    state.enemies[47].position = glm::vec3(88, -1, 0);
    state.enemies[48].position = glm::vec3(88, -6, 0);
    state.enemies[49].position = glm::vec3(90, -1, 0);
    state.enemies[50].position = glm::vec3(90, -6, 0);

    state.enemies[51].position = glm::vec3(94, -1, 0);
    state.enemies[52].position = glm::vec3(94, -6, 0);
    state.enemies[53].position = glm::vec3(98, -3.5, 0);
    state.enemies[54].position = glm::vec3(98, -3.5, 0);
    state.enemies[55].position = glm::vec3(102, -1, 0);
    state.enemies[56].position = glm::vec3(102, -6, 0);
    state.enemies[57].position = glm::vec3(104, -3.5, 0);
    state.enemies[58].position = glm::vec3(104, -3.5, 0);
    state.enemies[59].position = glm::vec3(107, -1, 0);
    state.enemies[60].position = glm::vec3(107, -6, 0);
    state.enemies[61].position = glm::vec3(109, -3.5, 0);
    state.enemies[62].position = glm::vec3(111, -3.5, 0);
    state.enemies[63].position = glm::vec3(113, -1, 0);
    state.enemies[64].position = glm::vec3(113, -6, 0);
    state.enemies[65].position = glm::vec3(115, -3.5, 0);
    state.enemies[66].position = glm::vec3(117, -3.5, 0);
    state.enemies[67].position = glm::vec3(119, -1, 0);

    state.enemies[68].position = glm::vec3(123, -1, 0);
    state.enemies[69].position = glm::vec3(123, -2, 0);
    state.enemies[70].position = glm::vec3(123, -5, 0);
    state.enemies[71].position = glm::vec3(123, -6, 0);
    state.enemies[72].position = glm::vec3(127, -1, 0);
    state.enemies[73].position = glm::vec3(127, -4, 0);
    state.enemies[74].position = glm::vec3(127, -5, 0);
    state.enemies[75].position = glm::vec3(127, -6, 0);
    state.enemies[76].position = glm::vec3(131, -1, 0);
    state.enemies[77].position = glm::vec3(131, -2, 0);
    state.enemies[78].position = glm::vec3(131, -3, 0);
    state.enemies[79].position = glm::vec3(131, -6, 0);
    state.enemies[80].position = glm::vec3(135, -1, 0);
    state.enemies[81].position = glm::vec3(135, -2, 0);
    state.enemies[82].position = glm::vec3(135, -3, 0);
    state.enemies[83].position = glm::vec3(135, -4, 0);
    state.enemies[84].position = glm::vec3(140, -1, 0);
    state.enemies[85].position = glm::vec3(140, -2, 0);
    state.enemies[86].position = glm::vec3(140, -4, 0);
    state.enemies[87].position = glm::vec3(140, -5, 0);
    state.enemies[88].position = glm::vec3(140, -6, 0);
    state.enemies[89].position = glm::vec3(145, -1, 0);
    state.enemies[90].position = glm::vec3(145, -2, 0);
    state.enemies[91].position = glm::vec3(145, -3, 0);
    state.enemies[92].position = glm::vec3(145, -4, 0);
    state.enemies[93].position = glm::vec3(145, -6, 0);
    state.enemies[94].position = glm::vec3(150, -1, 0);
    state.enemies[95].position = glm::vec3(150, -3, 0);
    state.enemies[96].position = glm::vec3(150, -4, 0);
    state.enemies[97].position = glm::vec3(150, -5, 0);
    state.enemies[98].position = glm::vec3(150, -6, 0);

}
void Level10::Update(float deltaTime) {
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
    for (int i = 0; i < LEVEL10_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.map);
    }

    if (state.player->position.x >= 160) {
        state.player->win = true;
    }
}
void Level10::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL10_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    if (state.fire->isActive) {
        state.fire->Render(program);
    }
}

void Level10::Reset() {
    Level10::Initialize();
}
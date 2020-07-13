#include "Entity.h"

bool top = 0;

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
    acceleration = glm::vec3(0);

    speed = 0;

    animCols = 1;
    animRows = 1;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) { //going up
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object) && isActive && object->isActive)
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            
            if (velocity.x > 0) { //moving to the right
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                object->collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                object->collidedLeft = true;
            }
        }
    }
}

void Entity::AIWaitAndGo(Entity* player)
{
    switch (aiState)
    {
    case IDLE:
        if (glm::distance(position, player->position) < 6.0f) {
            switch (aiType) {
                case WALKER:
                    aiState = WALKING;
                    break;
                case JUMPER:
                    aiState = JUMPING;
                    break;
                case FLYER:
                    aiState = FLYING;
                    break;
            }
        }
        break;

    case WALKING:
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        break;

    case JUMPING:
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        if (collidedBottom) {
            velocity.y += 5.0f;
            collidedBottom = false;
        }
        break;

    case FLYING:
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        if (collidedBottom) {
            velocity.y += 1.0f;
            acceleration.y = -0.25f;
            collidedBottom = false;
        }
        break;
    }
}

void Entity::AI(Entity* player) {
    CheckCollisionsX(player, 1);
    CheckCollisionsY(player, 1);

    switch (aiType) {
    case WALKER:
        AIWaitAndGo(player);
        break;

    case JUMPER:
        AIWaitAndGo(player);
        break;
    
    case FLYER:
        AIWaitAndGo(player);
        break;

    default:
        break;
    }
}

void Entity::Projectile(Entity* target, Entity* player, float deltaTime) {
    collidedRight = false;
    collidedLeft = false;
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.x += velocity.x * deltaTime;
    CheckCollisionsX(target, 1);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f));

    if (collidedRight || collidedLeft) {
        isActive = false;
        target->isActive = false;

        player->enemiesKilled++;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* enemyTarget, Entity* platforms, int platformCount)
{
    if (isActive == false) return;

    if (entityType == ENEMY) {
        AI(player); //process input for an enemy
    }
    else if (entityType == OBJECT) {
        Projectile(enemyTarget, player, deltaTime);
    }

    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime; //keep adding to velocity with the acceleration

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platforms, platformCount); // Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platforms, platformCount); // Fix if needed

    //if collided with something, what was it, do what?

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f));
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {

    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
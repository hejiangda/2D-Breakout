#include "game.h"
#include "resourcemanager.h"
#include "spriterenderer.h"
#include "ballobject.h"
#include <algorithm>
// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;

Game::Game(SceneGLWindow* window):
    AbstractGLScene(window), State(GAME_ACTIVE)
{
    window->setScene(this);
    connect(window, &SceneGLWindow::keySiginal,
            this, &Game::ProcessInput);

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}
void Game::initialize()
{
    AbstractGLScene::initialize();
    glViewport(0,0,window()->width(),window()->height());
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    Init();
}
// Initial velocity of the Ball
const QVector2D INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

BallObject *Ball;
void Game::Init()
{
    ResourceManager::LoadShader("../2D-Breakout/shaders/sprite.vert","../2D-Breakout/shaders/sprite.frag",nullptr,"sprite");
    QMatrix4x4 projection;
    projection.ortho(0,this->window()->width(),this->window()->height(),0,-1,1);
    ResourceManager::GetShader("sprite").bind();
    ResourceManager::GetShader("sprite").setUniformValue("sprite",0);
    ResourceManager::GetShader("sprite").setUniformValue("projection",projection);

    //    ResourceManager::LoadTexture("../2D-Breakout/textures/awesomeface.png",GL_TRUE,"face");
    //    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load textures
    ResourceManager::LoadTexture("../2D-Breakout/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("../2D-Breakout/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("../2D-Breakout/textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("../2D-Breakout/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("../2D-Breakout/textures/paddle.png", true, "paddle");
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load levels
    GameLevel one; one.Load("../2D-Breakout/levels/one.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel two; two.Load("../2D-Breakout/levels/two.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel three; three.Load("../2D-Breakout/levels/three.lvl", this->window()->width(), this->window()->height() * 0.5);
    GameLevel four; four.Load("../2D-Breakout/levels/four.lvl", this->window()->width(), this->window()->height() * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // Configure game objects
    QVector2D playerPos = QVector2D(this->window()->width() / 2 - PLAYER_SIZE.x() / 2, this->window()->height() - PLAYER_SIZE.y());
    Player = new GameObject(playerPos, PLAYER_SIZE, &ResourceManager::GetTexture("paddle"));

    QVector2D ballPos = playerPos + QVector2D(PLAYER_SIZE.x() / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                          &ResourceManager::GetTexture("face"));
}
// 使用定时器更新游戏时间
void Game::UpdateGame(GLfloat dt)
{
    Ball->Move(dt, this->window()->width());
    // Check for collisions
    this->DoCollisions();
}


void Game::ProcessInput(Qt::Key k)
{
    qDebug()<<k;
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * deltat;
        // Move playerboard
        if (k==Qt::Key_A)
        {
            qDebug()<<Player->Position.x();
            if (Player->Position.x() >= 0)
            {
                Player->Position.setX(Player->Position.x()- velocity);
                if (Ball->Stuck)
                    Ball->Position.setX(Ball->Position.x()-velocity);
            }
        }
        if (k==Qt::Key_D)
        {
            if (Player->Position.x() <= this->window()->width() - Player->Size.x())
            {
                Player->Position.setX( Player->Position.x()+ velocity);
                if (Ball->Stuck)
                    Ball->Position.setX( Ball->Position.x()+ velocity);
            }
        }
        if (k==Qt::Key_Space)
            Ball->Stuck = false;
    }
}
// 使用定时器定时渲染
void Game::paint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    UpdateGame(0.0075);
    Render();
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), QVector2D(0, 0), QVector2D(this->window()->width(), this->window()->height()), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
    }
}
GLboolean Game::CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // Get center point circle first
    QVector2D center(one.Position.x() + one.Radius,one.Position.y() + one.Radius);
    // Calculate AABB info (center, half-extents)
    QVector2D aabb_half_extents(two.Size.x() / 2, two.Size.y() / 2);
    QVector2D aabb_center(
                two.Position.x() + aabb_half_extents.x(),
                two.Position.y() + aabb_half_extents.y()
                );
    // Get difference vector between both centers
    QVector2D difference = center - aabb_center;
    QVector2D clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Add clamped value to AABB_center and we get the value of box closest to circle
    QVector2D closest = aabb_center + clamped;
    // Retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return difference.length() < one.Radius;
}
void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Ball, box))
            {
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
            }
        }
    }
}

QVector2D Game::clamp(QVector2D value, QVector2D min, QVector2D max) {
    return QVector2D(std::max(min.x(), std::min(max.x(), value.x())),std::max(min.y(), std::min(max.y(), value.y())));
}

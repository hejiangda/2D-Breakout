#include "game.h"
#include "resourcemanager.h"
#include "spriterenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;

Game::Game(SceneGLWindow* window):
    AbstractGLScene(window), State(GAME_ACTIVE)
{

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
}
// 使用定时器更新游戏时间
void Game::Update(GLfloat dt)
{

}


//void Game::ProcessInput(GLfloat dt)
//{

//}
// 使用定时器定时渲染
void Game::paint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
    }
}

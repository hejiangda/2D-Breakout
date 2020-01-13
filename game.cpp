#include "game.h"
#include "resourcemanager.h"
#include "spriterenderer.h"

SpriteRenderer *Renderer;

Game::Game(SceneGLWindow* window):
    AbstractGLScene(window)
{

}

Game::~Game()
{
    delete Renderer;
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
    ResourceManager::GetShader("sprite").setUniformValue("image",0);
    ResourceManager::GetShader("sprite").setUniformValue("projection",projection);

    ResourceManager::LoadTexture("../2D-Breakout/texture/awesomeface.png",GL_TRUE,"face");
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
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
    Renderer->DrawSprite(ResourceManager::GetTexture("face"),QVector2D(200,200),QVector2D(300,400),45.f,QVector3D(0.f,1.f,0.f));
}

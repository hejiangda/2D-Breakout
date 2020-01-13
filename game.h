#ifndef GAME_H
#define GAME_H
#include "abstractglscene.h"
#include "sceneglwindow.h"
#include <QVector2D>
#include "gamelevel.h"
#include "gameobject.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
// Initial size of the player paddle
const QVector2D PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);

class SceneGLWindow;
class Game:public QObject, public AbstractGLScene
{
    Q_OBJECT
public:
    Game(SceneGLWindow* window);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void initialize();
    void paint();
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
//    GLuint                 Width, Height;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    // Constructor/Destructor
//    Game(GLuint width, GLuint height);

    // GameLoop
    // complete in sceneglwindow
//    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void Init();
};

#endif // GAME_H

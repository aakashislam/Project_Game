#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>



using namespace std;
using namespace sf;

const double SCREEN_WIDTH = 800;
const double SCREEN_HEIGHT = 600;
const int BORDER_LEFT = 140;
const int BORDER_RIGHT = 660;
const int RACER_WIDTH = 40;
const int RACER_HEIGHT = 70;

enum class GameState {
    MainMenu,
    Game,
    GameOver
};

enum class Difficulty {
    Easy,
    Mid,
    Hard
};

int RandomNumber(int a, int b) {
    static bool first = true;
    if (first) {
        srand(time(NULL));
        first = false;
    }
    int result = a + rand() % ((b + 1) - a);
    result = (result / 10) * 10;
    return result;
}


void resetGame(double& racerX, double& racerY, double& obstacleX, double& obstacleY, GameState& gameState, bool& soundOn, Sound& gameSound) {
    gameState = GameState::Game;
    obstacleY = 0;
    obstacleX = RandomNumber(BORDER_LEFT,BORDER_RIGHT);
    racerX = SCREEN_WIDTH/2;
    racerY = SCREEN_HEIGHT-RACER_HEIGHT;
}

int main() {
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "2D CAR RACING"); //top e title

    int score = 0;
    double gameSpeed = 0.08; // Default easy
    Difficulty difficulty = Difficulty::Easy;


    SoundBuffer gameSoundBuffer;
    gameSoundBuffer.loadFromFile("sound/naruto.mp3");
    Sound gameSound;
    gameSound.setBuffer(gameSoundBuffer);
    gameSound.setLoop(true); // loop e cholbe seshe


    Font font,font2;
    font.loadFromFile("font/xirod.ttf");
    font2.loadFromFile("font/2.ttf");

    Texture menuback, background, racer, obstacle;
    menuback.loadFromFile("cars/menuback.jpg");
    background.loadFromFile("cars/background.png");  //texture
    racer.loadFromFile("cars/racer.png");
    obstacle.loadFromFile("cars/obs1.png");

    Sprite menubackSprite(menuback);
    Sprite backgroundSprite(background);   // sprites
    Sprite racerSprite(racer);
    Sprite obstacleSprite(obstacle);


    double racerX = SCREEN_WIDTH / 2;
    double racerY = SCREEN_HEIGHT - RACER_HEIGHT;
    double obstacleX = RandomNumber(BORDER_LEFT, BORDER_RIGHT); // starting position
    double obstacleY = 0;


    GameState gameState = GameState::MainMenu;
    bool soundOn = true; // default sound on

    int highScore = 0;
    std::ifstream file("highscore.txt"); //input file stream?
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        std::ofstream newFile("highscore.txt"); //1st time create korbe
        newFile << 0;
        newFile.close();
    }

    //     Main menu text
    Text gameName("TRAFFIC MANIA", font, 45);
    gameName.setPosition(SCREEN_WIDTH / 2 - 230, SCREEN_HEIGHT / 2 - 150);
    gameName.setFillColor(Color::Cyan);
    Text gameName2("TRAFFIC MANIA", font, 45);
    gameName2.setPosition(SCREEN_WIDTH / 2 - 234, SCREEN_HEIGHT / 2 - 152);

    Text startGame("1. Start Game", font2, 20);
    startGame.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 45);
    Text sound("2. Sound: On", font2, 20);
    sound.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 15);
    Text difficultytxt("3. Difficulty: Easy", font2, 20);
    difficultytxt.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 15);
    Text exit("4. Exit", font2,     20);
    exit.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 45);

    //         Game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (gameState == GameState::MainMenu) {
                    if (startGame.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        resetGame(racerX, racerY, obstacleX, obstacleY, gameState, soundOn, gameSound);
                    }
                    if (sound.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        soundOn = !soundOn;
                        sound.setString(soundOn ? "2. Sound: On" : "2. Sound: Off");
                    }
                    if (difficultytxt.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if (difficulty == Difficulty::Easy) {
                            difficulty = Difficulty::Mid;
                            gameSpeed = 0.14;
                            difficultytxt.setString("3. Difficulty: Mid");
                        } else if (difficulty == Difficulty::Mid) {
                            difficulty = Difficulty::Hard;
                            gameSpeed = 0.16;
                            difficultytxt.setString("3. Difficulty: Hard");
                        } else {
                            difficulty = Difficulty::Easy;
                            gameSpeed = 0.1 8;
                            difficultytxt.setString("3. Difficulty: Easy");
                        }
                    }
                    if (exit.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                    }
                }
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    if (gameState == GameState::GameOver) {
                        gameState = GameState::MainMenu;
                        score = 0;
                    }
                }
            }
        }


        if (gameState == GameState::Game) {
            if (soundOn && gameSound.getStatus() != Sound::Playing) {
                gameSound.play();
            }


            obstacleY += gameSpeed;
            if (obstacleY > SCREEN_HEIGHT) {
                obstacleY = 0;
                obstacleX = RandomNumber(BORDER_LEFT, BORDER_RIGHT); //obstacle movement
                score += 5;
            }


            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                if (racerX > BORDER_LEFT) racerX -= gameSpeed;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                if (racerX < BORDER_RIGHT) racerX += gameSpeed; //racer movement
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                if (racerY > 0) racerY -= gameSpeed;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                if (racerY < SCREEN_HEIGHT - RACER_HEIGHT) racerY += gameSpeed;
            }

            // Collision
            if (((racerX>=(obstacleX-30)) &&(racerX<=(obstacleX+30)))&&((racerY>=(obstacleY-30)) &&(racerY)<=(obstacleY+30))) {

                gameState = GameState::GameOver;
                gameSound.stop();
            }
        } else {
            if (gameSound.getStatus() == Sound::Playing) {
                gameSound.stop();
            }
        }

       //loop e render
        window.clear();
        if (gameState == GameState::MainMenu) {
            window.draw(menubackSprite);
            window.draw(gameName2);
            window.draw(gameName);

            window.draw(startGame);
            window.draw(sound);
            window.draw(difficultytxt);
            window.draw(exit);
            Text highScoreText("High Score: " + std::to_string(highScore), font, 30);
            highScoreText.setPosition(SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT - 75);
            highScoreText.setFillColor(Color::Green);
            window.draw(highScoreText);
        } else if (gameState == GameState::Game) {
            window.draw(backgroundSprite);
            window.draw(racerSprite);
            window.draw(obstacleSprite);
            Text scoreText("Score: " + std::to_string(score), font, 20);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
        } else if (gameState == GameState::GameOver) {
            window.draw(backgroundSprite);
            Text gameOverText("Game Over!", font, 45);
            gameOverText.setPosition(SCREEN_WIDTH / 2 -            180, SCREEN_HEIGHT / 2 - 150);
            gameOverText.setFillColor(Color::Red);
            window.draw(gameOverText);
            Text finalScoreText("Final score: " + std::to_string(score), font, 20);
            finalScoreText.setPosition(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2);
            window.draw(finalScoreText);
            if (score > highScore) {
                highScore = score;
                std::ofstream file("highscore.txt");
                file << highScore;
                file.close();
            }



            Text restartText("Press Escape to go back to Menu", font2, 20);
            restartText.setPosition(SCREEN_WIDTH / 2 - 155, SCREEN_HEIGHT - 30);
            restartText.setFillColor(Color::Black);
            window.draw(restartText);
        }

        racerSprite.setPosition(racerX, racerY);
        obstacleSprite.setPosition(obstacleX, obstacleY); //update position


        window.display();
    }

    return 0;
}

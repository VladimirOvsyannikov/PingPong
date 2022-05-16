/* --------------------------------------------------------
 *    File: pong_defs.h
 *  Author: Vladimir Ovsyannikov
 *   Class: programming I COP 2006 CRN 13969
 * Purpose: definitions for pong game
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>

#ifndef SFML_5_PlayerPaddle_PONG_DEFS_H // if not defied         //preproccesor directives (start with a # sign)
#define SFML_5_PlayerPaddle_PONG_DEFS_H // defined


// Global Constants
// --------------------------------------------------------

// window properties
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::White;

// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Blue;

// define global constant and types for the ball
const float BALL_RADIUS = 10.0;

// define global constants and types for walls
const float WALL_THICKNESS = 10.0;
const sf::Color WALL_COLOR = sf::Color::Red;

// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;
const float BALL_SPEED_X = BALL_RADIUS * (10.0/1000.0);    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * (8.5/1000.0);   // span  vertically

// define global constant and type for paddle
const float PADDLE_HEIGHT = 80.0;
const float PADDLE_THICKNESS = 10.0;
const sf:: Color PADDLE_COLOR = sf:: Color::Green;
const float PADDLE_SPEED = PADDLE_HEIGHT / 10.0 / 1000.0;

// Type definitions
// --------------------------------------------------------

enum Direction {
    Exit = -1,
    None = 0, //Technically don't need to show the value from here down
    Left = 1,
    Up = 2,
    Right = 3,
    Down = 4,
    Start = 5
};

// ball properties
struct Ball {
    float radius; // 4 bytes + 0 bytes
    float coordinateX; // 4 bytes + 4 bytes
    float coordinateY; // 4 bytes + 8 bytes
    float velocityX; // 4 bytes + 12 bytes
    float velocityY; // 4 bytes + 16 bytes
    sf::Color color; // 4 bytes + 20 bytes
}; // 24 bytes in total, every properties is 4 bytes

//Nested wall and border properties
struct Block {
    float left;
    float top;
    float width;
    float height;
    sf::Color color;
    sf::RectangleShape rectangle;
};
// moving block (paddle) structure properties
struct Paddle {
    Block block;
    float blockVelocityX;
    float blockVelocityY;
};
// moving block (AI paddle) structure properties
struct PaddleAi {
    Block block;
    float blockAiVelocityX;
    float blockAiVelocityY;
};
// structure properties for the borders
struct Borders {
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
};

#endif //SFML_5_PlayerPaddle_PONG_DEFS_Hsssss
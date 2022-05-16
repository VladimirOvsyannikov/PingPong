/* --------------------------------------------------------
 *    File: PongFinal.cpp
 *  Author: Vladimir Ovsyannikov
 *   Class: Programming I COP 2006 CRN 13969
 * Purpose: implementation of collision detection for paddle
 * and top/bottom walls. Also, implement AI Paddle.
 * -------------------------------------------------------- */
#include "pong_defs.h" //preprocessor directive

// Function declarations (prototypes)
// --------------------------------------------------------
void setup(Ball &ball, Borders &block, Paddle &paddle, PaddleAi &paddleAi);
Direction processInput();
bool update(Direction &input, Ball &ball, Borders block, Paddle &paddle,
            PaddleAi &paddleAi, float delta, bool &gameStart);
void render(sf::RenderWindow &window, Ball ball, Borders block, Paddle paddle, PaddleAi paddleAi, float delta);
bool collisionDetection(Ball ball, Block block);
bool paddleCollisionDetection(Block block, Block paddle);
bool paddleAiCollisionDetection (Block block, Block paddleAi);
bool doCollisionChecks(Ball &ball, Borders &block, Paddle &paddle, PaddleAi &paddleAi);


/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello SFML");
    window.clear(WINDOW_COLOR);

    // declare a ball variable and populate it in the center of the window
    Ball theBall;
    // declare a variable of type Borders
    Borders theWalls;
    // declare a variable of type paddle
    Paddle thePaddle;
    // declare a variable of type PaddleAi
    PaddleAi theAiPaddle;
    // call to set up function
    setup(theBall, theWalls, thePaddle, theAiPaddle);


    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;

    bool gameStart = false;

    bool gameOver = false;
    while (!gameOver) {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;

        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

            gameOver = update(userInput, theBall, theWalls, thePaddle,theAiPaddle, delta, gameStart);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        // ------------------------------------------------
        render(window, theBall, theWalls, thePaddle, theAiPaddle, delta);

    } // end main game loop

    // game ending, close the graphics window
    window.close();

    return 0;   // return success to the OS
} // end main


/**
 * setup holds all the initialization code for game objects
 * @param ball - structure variable with properties for ball
 * @param block - structure variable with properties for wall
 * @param paddle - structure variable with properties for paddle
 */
void setup(Ball &ball, Borders &block, Paddle &paddle, PaddleAi &paddleAi) {

    // variable declaration for ball
    ball.radius = BALL_RADIUS;
    ball.coordinateX = paddle.block.left + paddle.block.width + BALL_RADIUS + 1;
    ball.coordinateY = paddle.block.top + (paddle.block.height / 2.0);
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;

    // variable declaration for left wall
    block.leftWall.left = 0.0;
    block.leftWall.top = 0.0;
    block.leftWall.width = WALL_THICKNESS;
    block.leftWall.height = WINDOW_HEIGHT;
    block.leftWall.rectangle.setSize(sf:: Vector2(block.leftWall.width, block.leftWall.height));
    block.leftWall.rectangle.setPosition(block.leftWall.left, block.leftWall.top);
    block.leftWall.rectangle.setFillColor(WALL_COLOR);

    // variable declaration for top wall
    block.topWall.left = 0.0;
    block.topWall.top = 0.0;
    block.topWall.width = WINDOW_WIDTH;
    block.topWall.height = WALL_THICKNESS;
    block.topWall.rectangle.setSize(sf::Vector2(block.topWall.width, block.topWall.height));
    block.topWall.rectangle.setPosition(block.topWall.left, block.topWall.top);
    block.topWall.rectangle.setFillColor(WALL_COLOR);

    // variable declaration for right wall
    block.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    block.rightWall.top = 0.0;
    block.rightWall.width = WALL_THICKNESS;
    block.rightWall.height = WINDOW_HEIGHT;
    block.rightWall.rectangle.setSize(sf::Vector2(block.rightWall.width, block.rightWall.height));
    block.rightWall.rectangle.setPosition(block.rightWall.left, block.rightWall.top);
    block.rightWall.rectangle.setFillColor(WALL_COLOR);

    // variable declaration for bottom wall
    block.bottomWall.left = 0.0;
    block.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    block.bottomWall.width = WINDOW_WIDTH;
    block.bottomWall.height = WALL_THICKNESS;
    block.bottomWall.rectangle.setSize(sf::Vector2(block.bottomWall.width, block.bottomWall.height));
    block.bottomWall.rectangle.setPosition(block.bottomWall.left, block.bottomWall.top);
    block.bottomWall.rectangle.setFillColor(WALL_COLOR);

    // variable declaration for paddle
    paddle.block.left = PADDLE_THICKNESS * 2; // sets paddle away from the left wall 2 paddle thickness
    paddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0; // center paddle vertically
    paddle.block.width = PADDLE_THICKNESS;
    paddle.block.height = PADDLE_HEIGHT;
    paddle.block.color = PADDLE_COLOR;
    paddle.block.rectangle.setSize(sf::Vector2(paddle.block.width, paddle.block.height));
    paddle.block.rectangle.setFillColor(PADDLE_COLOR);
    paddle.blockVelocityX = 0.0;
    paddle.blockVelocityY = 0.0;

    // variable declaration for AI paddle
    paddleAi.block.left = block.rightWall.left - (2 * PADDLE_THICKNESS); // sets paddle away from the right wall 2 paddle thicknesses
    paddleAi.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0; // center paddle vertically
    paddleAi.block.width = PADDLE_THICKNESS;
    paddleAi.block.height = PADDLE_HEIGHT;
    paddleAi.block.color = PADDLE_COLOR;
    paddleAi.block.rectangle.setSize(sf::Vector2(paddleAi.block.width, paddleAi.block.height));
    paddleAi.block.rectangle.setFillColor(PADDLE_COLOR);
    paddleAi.blockAiVelocityX = 0.0;
    paddleAi.blockAiVelocityY = 0.0;
} // end setup


/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return Direction - user input (default no-input=None, quit=Exit)
 */
Direction processInput() {

    Direction input = None;  // no input

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = Start;
    }
    return input;
} // end getUserInput


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param block - render wall position
 * @param paddle - update paddle velocity and position
 * @param delta - current frame time
 * @param gameStart - start the game
 * @return bool - True/False - ball hit left wall
 */
bool update(Direction &input, Ball &ball, Borders block, Paddle &paddle,
            PaddleAi &paddleAi, float delta, bool &gameStart) {

    // set bool game over variable to false
    bool gameOver = false;

    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case 1: Left:
                // ball.velocityX -= BALL_SPEED_X;
                break;
            case 2: Up:
                paddle.blockVelocityY -= PADDLE_SPEED;
                // ball.velocityY -= BALL_SPEED_Y;
                break;
            case 3: Right:
                // ball.velocityX += BALL_SPEED_X;
                break;
            case 4: Down:
                paddle.blockVelocityY += PADDLE_SPEED;
                // ball.velocityY += BALL_SPEED_Y;
                break;
            case 5: Start:
                if (!gameStart) {
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y;

                    if ((int(delta * 10) & 1) % 2) {
                        ball.velocityY *= -1; //ball goes down if odd
                        gameStart = true;
                    }
                }
                break;
        }
        // clear input

    }

    // adjust the location of the paddle for speed * time
    // ------------------------------------------------
    paddle.block.left += paddle.blockVelocityX * delta;
    paddle.block.top += paddle.blockVelocityY * delta;

    // adjust the location of the AI paddle for speed * time
    paddleAi.block.left += paddleAi.blockAiVelocityX * delta;
    paddleAi.block.top += paddleAi.blockAiVelocityY * delta;

    // if statement to adjust the location of the ball for speed * time
    if (gameStart == true) {
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;
        if (ball.coordinateY < paddleAi.block.top + (paddleAi.block.height / 2.0)) {
            paddleAi.blockAiVelocityY -= PADDLE_SPEED;
        }
        else if (ball.coordinateY > paddleAi.block.top + (paddleAi.block.height / 2.0)){
            paddleAi.blockAiVelocityY += PADDLE_SPEED;
        }
    }
    else {
        ball.coordinateX = paddle.block.left + paddle.block.width + BALL_RADIUS + 1;
        ball.coordinateY = paddle.block.top + (paddle.block.height / 2.0);
    }

    // call to the do collision check function
    gameOver = doCollisionChecks(ball, block, paddle, paddleAi);

    return gameOver;
} // end update


/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param block - structure variable with properties for the wall
 * @param paddle - structure variable with properties for paddle
 * @param delta  - amount of frame time plus lag (in ms)
 */
void render(sf::RenderWindow &window, Ball ball, Borders block, Paddle paddle, PaddleAi paddleAi, float delta) {

    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;circle.setFillColor(ball.color);
    circle.setRadius(ball.radius);
    // set screen coordinates relative to the center of the circle
    circle.setOrigin(ball.radius , ball.radius); //Not (0.0,0.0), because ball would be off the screen partially
    // calculate current drawing location relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX * delta;
    float yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    //draw the rectangle borders
    // ------------------------------------------------
    sf:: RectangleShape rectangle;
    //Draw left wall
    window.draw(block.leftWall.rectangle);
    //Draw top wall
    window.draw(block.topWall.rectangle);
    //Draw right wall
    window.draw(block.rightWall.rectangle);
    //Draw bottom wall
    window.draw(block.bottomWall.rectangle);


    // draw the paddle
    // ------------------------------------------------
    sf::RectangleShape rectanglePaddle;// calculate current drawing location relative to speed and frame-time
    float Paddle_xPosition = paddle.block.left + paddle.blockVelocityX * delta;
    float Paddle_yPosition = paddle.block.top + paddle.blockVelocityY * delta;
    // call the setPosition function passing the new x and y coordinates
    paddle.block.rectangle.setPosition(Paddle_xPosition, Paddle_yPosition);
    // draw the paddle on the window
    window.draw(paddle.block.rectangle);

    //draw the AI paddle
    // ------------------------------------------------
    sf::RectangleShape rectangleAiPaddle;// calculate current drawing location relative to speed and frame-time
    float PaddleAi_xPosition = paddleAi.block.left + paddleAi.blockAiVelocityX * delta;
    float PaddleAi_yPosition = paddleAi.block.top + paddleAi.blockAiVelocityY * delta;
    // call the setPosition function passing the new x and y coordinates
    paddleAi.block.rectangle.setPosition(PaddleAi_xPosition, PaddleAi_yPosition);
    // draw the paddle on the window
    window.draw(paddleAi.block.rectangle);
    // show the new window
    // ------------------------------------------------
    window.display();
} // end render


/**
 *Collision detection to reflect ball off bordering walls
 * @param ball - structure variable with properties for the ball
 * @param Block - structure variable with properties for the wall
 * @Return bool - True/False - ball touches border
*/
bool collisionDetection(Ball ball, Block block) {

    // Boolean value between relationship of ball and border
    // ------------------------------------------------
    // Declare local bool ballCollision variable to return false
    bool ballCollision = false;

    // Declare local float variables for ball (left,top,right and bottom) and calculate them
    float ball_left = ball.coordinateX - ball.radius;
    float ball_top = ball.coordinateY - ball.radius;
    float ball_right = ball.coordinateX + ball.radius;
    float ball_bottom = ball.coordinateY + ball.radius;

    // Declare local float variable for borders (right and bottom)
    float wall_right = block.left + block.width;
    float wall_bottom = block.top + block.height;
    // Unnecessary left and top border, for clarity
    float wall_left = block.left;
    float wall_top = block.top;

    // If statement for the bounding box intersection algorithm
    // ------------------------------------------------
    // Return true if all conditions check
    if (ball_left < wall_right &&
        ball_right > wall_left &&
        ball_top < wall_bottom &&
        ball_bottom > wall_top) {
        ballCollision = true;
    }
    return ballCollision;
}  // end collisionDetection


/**
* Collision detection to reflect the moving block off the stationary block
 * @param block - structure variable with properties for the stationary walls
 * @param paddle - structure variable with properties for the moving wall
 * @return bool - True/false - paddle touches bottom or top wall
*/
bool paddleCollisionDetection(Block block, Block paddle) {

    // boolean value between relationship of ball and border
    // ------------------------------------------------
    // declare local bool paddleCollision variable to return false
    bool paddleCollision = false;

    // declare local float variables for moving paddle (right and bottom) and calculate them
    float moving_right = paddle.left + paddle.width;
    float moving_bottom = paddle.top + paddle.height;
    // unnecessary left and top moving paddle, for clarity
    float moving_left = paddle.left;
    float moving_top = paddle.top;

    // declare local float variable for borders (right and bottom)
    float stationary_right = block.left + block.width;
    float stationary_bottom = block.top + block.height;
    // Unnecessary left and top stationary wall, for clarity
    float stationary_left = block.left;
    float stationary_top = block.top;

    // if statement for the bounding box intersection algorithm
    // ------------------------------------------------
    // return true if all conditions check
    if (moving_left < stationary_right &&
        moving_right > stationary_left &&
        moving_top < stationary_bottom &&
        moving_bottom > stationary_top) {
        paddleCollision = true;
    }
    return paddleCollision;
} // end paddleCollisionDetection


/**
 * Collision detection to reflect the moving AI paddle off the stationary top and bottom wall
 * @param block
 * @param paddleAi
 * @return bool - True/false - AI paddle touches bottom or top wall
 */
bool paddleAiCollisionDetection (Block block, Block paddleAi) {
    bool paddleAiCollision = false;

    // declare local float variables for AI paddle (right and bottom) and calculate them
    float movingAi_right = paddleAi.left + paddleAi.width;
    float movingAi_bottom = paddleAi.top + paddleAi.height;
    //Unnecessary left and top moving paddle, for clarity
    float movingAi_left = paddleAi.left;
    float movingAi_top = paddleAi.top;

    // declare local float variable for borders (right and bottom)
    float stationary_right = block.left + block.width;
    float stationary_bottom = block.top + block.height;
    // Unnecessary left and top stationary wall, for clarity
    float stationary_left = block.left;
    float stationary_top = block.top;

    if (movingAi_left < stationary_right &&
        movingAi_right > stationary_left &&
        movingAi_top < stationary_bottom &&
        movingAi_bottom > stationary_top) {
    paddleAiCollision = true;
}

return paddleAiCollision;
} // end paddleCollisionDetection


/**
* Collision detection to reflect the moving block off the stationary block
 * @param ball - structure variable with properties for the wall
 * @param block - structure variable with properties for the stationary walls
 * @param paddle - structure variable with properties for the moving wall
 * @param paddleAi - structure variable with properties for the AI wall
 * @return bool - True/false - if a collision was detected between two objects
*/
bool doCollisionChecks(Ball &ball, Borders &block, Paddle &paddle, PaddleAi &paddleAi) {

    // if statement calling collision check function passing in the ball and paddle block structure
    bool gameOver = false;

    // reflect ball if collision is detected between ball and paddle
    if (collisionDetection(ball, paddle.block)) {
        ball.velocityX *= -1;
        ball.coordinateX = paddle.block.left + paddle.block.width + ball.radius + 1;
    }

    // reflect ball if collision is detected between ball and AI paddle
    if (collisionDetection(ball, paddleAi.block)) {
        ball.velocityX *= -1;
        ball.coordinateX = paddleAi.block.left - paddleAi.block.width - 1;
    }
        if ((ball.velocityX > 0) && paddleAi.blockAiVelocityY < 0) {
            ball.velocityY += ball.velocityY / 2.0;
        }
        else if ((ball.velocityY > 0 ) && paddleAi.blockAiVelocityY > 0 ) {
            ball.velocityY += ball.velocityY / 2.0;
        }
        if ((ball.velocityY > 0 ) && paddleAi.blockAiVelocityY < 0 ) {
        ball.velocityY -= ball.velocityY / 2.0;
        }
        else if ((ball.velocityY < 0 ) && paddleAi.blockAiVelocityY > 0 ) {
        ball.velocityY += ball.velocityY / 2.0;
        }
    // If / else-if statements for ball collisions
        // ------------------------------------------------
        // check horizontal ball collisions
    if (collisionDetection(ball, block.leftWall)) {
        ball.velocityX *= -1;
        ball.coordinateX = block.leftWall.left + block.leftWall.width + ball.radius + 1;
        gameOver = true;
    }
    else if (collisionDetection(ball, block.rightWall)) {
        ball.velocityX *= -1;
        ball.coordinateX = block.rightWall.left - ball.radius - 1;
        gameOver = true;
    }

    // check vertical ball collisions
    if (collisionDetection(ball, block.topWall)) {
        ball.velocityY *= -1;
        ball.coordinateY = block.topWall.top + block.topWall.height + ball.radius + 1;
    }
    else if (collisionDetection(ball, block.bottomWall)) {
        ball.velocityY *= -1;
        ball.coordinateY = block.bottomWall.top - ball.radius - 1;
    }

    // If / else-if statements for paddle collisions with stationary wall
    // ------------------------------------------------
    // check vertical paddle collisions

    if (paddleCollisionDetection(paddle.block, block.topWall)) {
        paddle.blockVelocityY = 0.0;
        paddle.block.top = block.topWall.top + block.topWall.height + 1;
    }
    else if (paddleCollisionDetection(paddle.block, block.bottomWall)) {
        paddle.blockVelocityY = 0.0;
        paddle.block.top = block.bottomWall.top - paddle.block.height - 1;
    }

    // If / else-if statements for AI paddle collisions with stationary wall
    // ------------------------------------------------
    // check vertical paddle collisions

    if (paddleAiCollisionDetection(paddleAi.block, block.topWall)) {
        paddleAi.blockAiVelocityY = 0.0;
        paddleAi.block.top = block.topWall.top + block.topWall.height + 1;
    }
    else if (paddleAiCollisionDetection(paddleAi.block, block.bottomWall)) {
        paddleAi.blockAiVelocityY = 0.0;
        paddleAi.block.top = block.bottomWall.top - paddle.block.height - 1;
    }
    return gameOver;
} // end doCollisionChecks

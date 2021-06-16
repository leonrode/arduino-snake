#include <BetterJoystick.h>
#include <LedControl.h>

/*
  Apple class will contain a spawn function which randomizes its new position,
  and a display function, which just sets the lcd pixel to the position
*/
class Apple {
  private:

    // used for blinking effect
    bool ledOn = true;
  public:
    int row;
    int col;
    Apple() {
      randomSeed(analogRead(0));
      spawn(); // initialize apple position
    }
    void spawn() {
      ledOn = true;
      row = random(0, 7);
      col = random(0, 7);
    }

    void display(LedControl* lcd) {

      lcd->setLed(0, row, col, ledOn);
      ledOn = !ledOn;
    }

};

class Snake {

  private:

    Joystick* joystick;
    
    DIRECTION currentDirection;
    DIRECTION lastDirection; // used to disallow the snake from turning into itself
    int* x;
    int* y;
    
    int numParts = 1;
    
  public:
    int headX;
    int headY;
    Snake() {

      joystick = new Joystick(A1, A2, 3);
      x = new int[64]; // 8x8 = 64 => 64 possible joint positions
      y = new int[64];

      reset();
    }

    void reset() {
      headX = random(3, 6);
      headY = random(3, 6);

      x[0] = headX;
      y[0] = headY;

      x[1] = headX - 1;
      y[1] = headY;

      x[2] = headX - 2;
      y[2] = headY;

      x[3] = headX - 3;
      y[3] = headY;

      numParts = 4;
      currentDirection = DIRECTION::RIGHT;
    }
    void input() {
      lastDirection = currentDirection;
      // checking to make sure the snake didn't turn into itself
      if (joystick->facingDirection() == DIRECTION::UP && lastDirection != DIRECTION::DOWN) currentDirection = DIRECTION::UP;
      else if (joystick->facingDirection() == DIRECTION::DOWN && lastDirection != DIRECTION::UP) currentDirection = DIRECTION::DOWN;
      else if (joystick->facingDirection() == DIRECTION::LEFT && lastDirection != DIRECTION::RIGHT) currentDirection = DIRECTION::LEFT;
      else if (joystick->facingDirection() == DIRECTION::RIGHT && lastDirection != DIRECTION::LEFT) currentDirection = DIRECTION::RIGHT;
    }
    void update() {
      input();

      // shifting
      for (int i = numParts; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
      }

      // wrapping behavior
      if (lastDirection != 2 && currentDirection == DIRECTION::UP) {
        y[0] = y[0] - 1 < 0 ? 7 : y[0] - 1;
      } else  if (currentDirection == DIRECTION::DOWN) {
        y[0] = y[0] + 1  > 7 ? 0 : y[0] + 1;
      } else if (currentDirection == DIRECTION::LEFT) {
        x[0] = x[0] - 1 < 0 ? 7  : x[0] - 1;
      } else if (currentDirection == DIRECTION::RIGHT) {
        x[0] = x[0] + 1 > 7 ? 0 : x[0] + 1;
      }

      headX = x[0];
      headY = y[0];
    }

    void display(LedControl* lcd) {
      // for each joint, set the led to be on
      for (int i = 0; i < numParts; i++) {
        lcd->setLed(0, y[i], x[i], true);
      }
    }

    void addPiece() {
      // setting the next piece to be the same position as the one before it,
      // to give an effect of adding a piece
      x[numParts] = x[numParts - 1];
      y[numParts] =  y[numParts - 1];
      numParts++;
    }

    bool isInBody(int row, int col) {

      // given any position,
      // if any of the snake's joints are situated
      // in that position, the position is in the  body
      for (int i = 0; i < numParts; i++) {
        if (x[i] == col && y[i] == row) {
          return true;
        }
      }

      return false;
    }

    bool hitSelf() {
      // if the head position matches any body position,
      // the snake hit itself
      for (int i = 1; i < numParts; i++) {
        if (x[i] == headX && y[i] == headY) {
          return true;
        }
      }
      return false;
    }

};


/*
  The Game class will be responsible for initializing the LCD matrix,
  keeping Apple and Snake members.
*/
class Game {

  private:
    LedControl* lcd;
    Joystick* joystick;

    Snake* snake;
    Apple* apple;
    
    int DELAY = 500;
    int score = 0;

    // pins for matrix
    const int DIN = 7;
    const int CLK = 10;
    const int CS = 11;

    void displayTensNum(int num) {
      /*

        100% better way of doing this
        like being smart and calculating the rows based on the numbers
        but I dont want to

      */
      switch (num) {
        case 0: {
            lcd->setLed(0, 0, 0, true);
            lcd->setLed(0, 0, 1, true);
            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 0, true);
            lcd->setLed(0, 1, 2, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 2, true);

            lcd->setLed(0, 3, 0, true);
            lcd->setLed(0, 3, 2, true);

            lcd->setLed(0, 4, 0, true);
            lcd->setLed(0, 4, 1, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }

        case 1: {
            lcd->setLed(0, 0, 2, true);
            lcd->setLed(0, 1, 2, true);
            lcd->setLed(0, 2, 2, true);
            lcd->setLed(0, 3, 2, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }
        case 2: {
            lcd->setLed(0, 0, 0, true);
            lcd->setLed(0, 0, 1, true);
            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 2, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 1, true);
            lcd->setLed(0, 2, 2, true);

            lcd->setLed(0, 3, 0, true);

            lcd->setLed(0, 4, 0, true);
            lcd->setLed(0, 4, 1, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }

        case 3: {
            lcd->setLed(0, 0, 0, true);
            lcd->setLed(0, 0, 1, true);
            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 2, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 1, true);
            lcd->setLed(0, 2, 2, true);

            lcd->setLed(0, 3, 2, true);

            lcd->setLed(0, 4, 0, true);
            lcd->setLed(0, 4, 1, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }

        case 4: {
            lcd->setLed(0, 0, 0, true);

            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 0, true);
            lcd->setLed(0, 1, 2, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 1, true);
            lcd->setLed(0, 2, 2, true);

            lcd->setLed(0, 3, 2, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }

        case 5:  {
            lcd->setLed(0, 0, 0, true);
            lcd->setLed(0, 0, 1, true);
            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 0, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 1, true);
            lcd->setLed(0, 2, 2, true);

            lcd->setLed(0, 3, 2, true);

            lcd->setLed(0, 4, 0, true);
            lcd->setLed(0, 4, 1, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }
        case 6: {
            lcd->setLed(0, 0, 0, true);
            lcd->setLed(0, 0, 1, true);
            lcd->setLed(0, 0, 2, true);

            lcd->setLed(0, 1, 0, true);

            lcd->setLed(0, 2, 0, true);
            lcd->setLed(0, 2, 1, true);
            lcd->setLed(0, 2, 2, true);


            lcd->setLed(0, 3, 0, true);
            lcd->setLed(0, 3, 2, true);

            lcd->setLed(0, 4, 0, true);
            lcd->setLed(0, 4, 1, true);
            lcd->setLed(0, 4, 2, true);
            break;
          }
      }
    }


    void displayOnesNum(int num) {
      switch (num) {
        case 0: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);
            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 5, true);
            lcd->setLed(0, 3, 7, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

        case 1: {
            lcd->setLed(0, 0, 7, true);
            lcd->setLed(0, 1, 7, true);
            lcd->setLed(0, 2, 7, true);
            lcd->setLed(0, 3, 7, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }
        case 2: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 5, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

        case 3: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 7, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

        case 4: {
            lcd->setLed(0, 0, 5, true);

            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);
            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 7, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

        case 5:  {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 7, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }
        case 6: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);


            lcd->setLed(0, 3, 5, true);
            lcd->setLed(0, 3, 7, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

          case 7: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 7, true);
            lcd->setLed(0, 2, 7, true);
            lcd->setLed(0, 3, 7, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

          case 8: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);
            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);


            lcd->setLed(0, 3, 5, true);
            lcd->setLed(0, 3, 7, true);

            lcd->setLed(0, 4, 5, true);
            lcd->setLed(0, 4, 6, true);
            lcd->setLed(0, 4, 7, true);
            break;
          }

          case 9: {
            lcd->setLed(0, 0, 5, true);
            lcd->setLed(0, 0, 6, true);
            lcd->setLed(0, 0, 7, true);

            lcd->setLed(0, 1, 5, true);
            lcd->setLed(0, 1, 7, true);

            lcd->setLed(0, 2, 5, true);
            lcd->setLed(0, 2, 6, true);
            lcd->setLed(0, 2, 7, true);

            lcd->setLed(0, 3, 7, true);
            lcd->setLed(0, 4, 7, true);

            break;
          }

          
      }
    }

  public:
    bool gameOver = false;

    Game() {
      // initializing our lcd
      lcd = new LedControl(DIN, CLK, CS);
      lcd->shutdown(0, false);
      lcd->setIntensity(0, 15);

      // creating our snake/apple
      snake = new Snake();
      apple = new Apple();
      
      // spawn our first apple
      spawnApple();
    }


    void displayScore() {
      // extract the tens digit 
      displayTensNum((int)floor(score / 10));
      displayOnesNum(score % 10);
    }

    void resetGame() {
      delay(5000);
      gameOver = false;
      score = 0;
      DELAY = 500;
      
      snake->reset();

      spawnApple();
    }

    void spawnApple() {
      // make sure the apple doesn't spawn in the snake
      apple->spawn();
      while (snake->isInBody(apple->row, apple->col)) {
        apple->spawn();
      }
    }

    bool snakeIsEating() {
      return snake->headY == apple->row && snake->headX == apple->col;
    }
    void run() {

      while (!gameOver) {
        lcd->clearDisplay(0);

        snake->update();

        if (snakeIsEating()) {

          score++;
          snake->addPiece();
          spawnApple();

        }


        if (snake->hitSelf()) {
          displayScore();
          delay(5000);
          resetGame();
        }
        snake->display(lcd);
        apple->display(lcd);
        
        delay(DELAY - (score * 10));
      }

      lcd->clearDisplay(0);
    }
};

Game game;

void setup() {}

void loop() {
  game.run();
}

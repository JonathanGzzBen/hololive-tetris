#include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;

const int PlayfieldWidth = 20;
const int PlayfieldHeight = 10;
const int NumBlocksFigure = 4;
const int NumberOfColors = 7;

int field[PlayfieldWidth][PlayfieldHeight] = {0};

struct Point {
  int x, y;
} pA[NumBlocksFigure], pB[NumBlocksFigure];

int figures[7][4] = {
    1, 3, 5, 7,  // I
    2, 4, 5, 7,  // Z
    3, 5, 4, 6,  // S
    3, 5, 4, 7,  // T
    2, 3, 5, 7,  // L
    3, 5, 7, 6,  // J
    2, 3, 4, 5,  // O
};

sf::SoundBuffer line_clear_sounds_buffers[NumberOfColors];

std::string line_clear_sounds[NumberOfColors] = {
    "aqua_qua.wav", "ahoy.wav",     "pekopeko.wav", "aqua_qua.wav",
    "pekopeko.wav", "aqua_qua.wav", "ahoy.wav",
};
sf::Sound line_cleared_sound;

bool isValidPosition() {
  for (int i{0}; i < NumBlocksFigure; i++) {
    if (pA[i].x < 0 || pA[i].x >= PlayfieldHeight ||
        pA[i].y >= PlayfieldWidth) {
      return 0;
    } else if (field[pA[i].y][pA[i].x]) {
      return 0;
    }
  }
  return 1;
}

int main() {
  srand(time(0));

  for (int i{0}; i < NumberOfColors; i++) {
    if (!line_clear_sounds_buffers[i].loadFromFile("sounds/" +
                                                   line_clear_sounds[i])) {
      std::cout << "Could not load sound file: " << line_clear_sounds[i];
    }
  }

  RenderWindow window(VideoMode(320, 480), "The Game!");

  Texture t;
  t.loadFromFile("images/tiles.png");
  Texture backgroundTexture;
  backgroundTexture.loadFromFile("images/background.png");
  Texture frameTexture;
  frameTexture.loadFromFile("images/frame.png");

  Sprite s(t), backgroundSprite(backgroundTexture), frameSprite(frameTexture);

  int dx = 0;
  bool rotate = false;
  int colorNum = 1;
  float timer = 0;
  float delay = 0.3;

  Clock clock;

  while (window.isOpen()) {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event e;
    while (window.pollEvent(e)) {
      if (e.type == Event::Closed) {
        window.close();
      }

      if (e.type == Event::KeyPressed) {
        if (e.key.code == Keyboard::Up) {
          rotate = true;
        } else if (e.key.code == Keyboard::Left) {
          dx = -1;
        } else if (e.key.code == Keyboard::Right) {
          dx = 1;
        }
      }
      if (e.type == Event::Resized) {
        sf::FloatRect visibleArea(0, 0, e.size.width, e.size.height);
        window.setView(sf::View(visibleArea));
      }
    }

    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      delay = 0.05;
    }

    // Move
    for (int i{0}; i < NumBlocksFigure; i++) {
      pB[i] = pA[i];
      pA[i].x += dx;
    }
    if (!isValidPosition()) {
      for (int i{0}; i < NumBlocksFigure; i++) {
        pA[i] = pB[i];
      }
    }

    // Rotate
    if (rotate) {
      Point rotationPoint = pA[1];
      for (int i = 0; i < NumBlocksFigure; i++) {
        int x{pA[i].y - rotationPoint.y};
        int y{pA[i].x - rotationPoint.x};
        pA[i].x = rotationPoint.x - x;
        pA[i].y = rotationPoint.y + y;
      };
      if (!isValidPosition()) {
        for (int i{0}; i < NumBlocksFigure; i++) {
          pA[i] = pB[i];
        }
      }
    }

    // Tick
    if (timer > delay) {
      for (int i{0}; i < NumBlocksFigure; i++) {
        pB[i] = pA[i];
        pA[i].y += 1;
      }

      if (!isValidPosition()) {
        // Place figure in playfield
        for (int i{0}; i < NumBlocksFigure; i++) {
          field[pB[i].y][pB[i].x] = colorNum;
        }

        // Get next figure color/shape
        colorNum = 1 + (rand() % NumberOfColors);
        int n = rand() % 7;
        for (int i{0}; i < NumBlocksFigure; i++) {
          pA[i].x = figures[n][i] % 2;
          pA[i].y = figures[n][i] / 2;
        }
      }
      timer = 0;
    }

    // Check lines
    int k{PlayfieldWidth - 1};
    for (int i{PlayfieldWidth - 1}; i > 0; i--) {
      int count{0};
      for (int j{0}; j < PlayfieldHeight; j++) {
        if (field[i][j]) {
          count++;
        }
        field[k][j] = field[i][j];
      }
      if (count < PlayfieldHeight) {
        k--;
      } else {
        // Line is cleared
        line_cleared_sound.setBuffer(line_clear_sounds_buffers[colorNum - 1]);
        line_cleared_sound.play();
        std::cout << "line cleared, reproduce sound: "
                  << line_clear_sounds[colorNum - 1] << std::endl;
      }
    }

    dx = 0;
    rotate = 0;
    delay = 0.3;

    // Draw

    window.clear(Color::White);
    window.draw(backgroundSprite);

    for (int i{0}; i < PlayfieldWidth; i++) {
      for (int j{0}; j < PlayfieldHeight; j++) {
        if (field[i][j] == 0) {
          continue;
        }

        s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
        s.setPosition(j * 18, i * 18);
        s.move(28, 31);
        window.draw(s);
      }
    }

    for (int i{0}; i < NumBlocksFigure; i++) {
      s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
      s.setPosition(pA[i].x * 18, pA[i].y * 18);
      s.move(28, 31);
      window.draw(s);
    }
    window.draw(frameSprite);
    window.display();
  }

  return 0;
}

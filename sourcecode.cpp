// CODE LEAKED BY TheLeaker-maker.
// This is only half of the code.

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

// Car class definition
class Car {
public:
    sf::RectangleShape shape;
    float speed;
    bool movingUp, movingDown, movingLeft, movingRight;

    Car(float x, float y) {
        shape.setSize(sf::Vector2f(100, 50)); // Car size
        shape.setFillColor(sf::Color::Red);   // Car color
        shape.setPosition(x, y);
        speed = rand() % 5 + 3; // Random speed between 3 and 7
        movingUp = movingDown = movingLeft = movingRight = false;
    }

    void move() {
        if (movingUp) shape.move(0, -speed);
        if (movingDown) shape.move(0, speed);
        if (movingLeft) shape.move(-speed, 0);
        if (movingRight) shape.move(speed, 0);
    }

    void aiMove() {
        int direction = rand() % 4;
        if (direction == 0) movingUp = true;
        else if (direction == 1) movingDown = true;
        else if (direction == 2) movingLeft = true;
        else if (direction == 3) movingRight = true;
    }

    void stop() {
        movingUp = movingDown = movingLeft = movingRight = false;
    }

    void resetPosition() {
        if (shape.getPosition().y > 600) shape.setPosition(rand() % 800, -50);
        if (shape.getPosition().y < -50) shape.setPosition(rand() % 800, 650);
    }
};

// Person class definition
class Person {
public:
    sf::CircleShape shape;
    float speed;

    Person(float x, float y) {
        shape.setRadius(20); // Person size
        shape.setFillColor(sf::Color::Blue); // Person color
        shape.setPosition(x, y);
        speed = 2.0f;
    }

    void move() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) shape.move(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) shape.move(0, speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) shape.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) shape.move(speed, 0);
    }

    void aiMove() {
        int direction = rand() % 4;
        if (direction == 0) shape.move(0, -speed); // Move up
        else if (direction == 1) shape.move(0, speed); // Move down
        else if (direction == 2) shape.move(-speed, 0); // Move left
        else if (direction == 3) shape.move(speed, 0); // Move right
    }
};

// Function to check if two objects collide
bool checkCollision(const sf::RectangleShape &car, const sf::CircleShape &person) {
    return car.getGlobalBounds().intersects(person.getGlobalBounds());
}

// Function to reset the game
void resetGame(Car &playerCar, std::vector<Car> &traffic, std::vector<Person> &people, int &score) {
    playerCar.shape.setPosition(400, 300);
    score = 0;
    for (auto &car : traffic) {
        car.shape.setPosition(rand() % 800, rand() % 600);
    }
    for (auto &person : people) {
        person.shape.setPosition(rand() % 800, rand() % 600);
    }
}

// Function to display the score
void displayScore(sf::RenderWindow &window, int score) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);

    window.draw(scoreText);
}

// Function to add traffic
void addTraffic(std::vector<Car> &traffic) {
    if (rand() % 100 < 10) { // 10% chance of adding a new car every frame
        traffic.push_back(Car(rand() % 800, -50)); // Add car at the top of the screen
    }
}

int main() {
    srand(time(0)); // Seed for random number generation

    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Car and People Game");

    // Load sound effects
    sf::SoundBuffer bufferCollision;
    if (!bufferCollision.loadFromFile("collision.wav")) {
        std::cerr << "Error loading collision sound!" << std::endl;
    }
    sf::Sound collisionSound(bufferCollision);

    // Create the car and person objects
    Car playerCar(400, 300);
    Car trafficCar1(200, 100);
    Car trafficCar2(600, 500);
    Car trafficCar3(100, 200);
    Car trafficCar4(700, 400);
    Person pedestrian(100, 400);
    Person pedestrian2(700, 200);
    Person pedestrian3(300, 50);

    std::vector<Car> traffic = { trafficCar1, trafficCar2, trafficCar3, trafficCar4 };
    std::vector<Person> people = { pedestrian, pedestrian2, pedestrian3 };

    int score = 0; // Game score
    bool gameOver = false; // Game over flag
    bool isPaused = false; // Game paused flag

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                // Restart the game on pressing R
                resetGame(playerCar, traffic, people, score);
                gameOver = false;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                // Pause the game on pressing P
                isPaused = !isPaused;
            }
        }

        if (gameOver) {
            // Display game over text
            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) {
                std::cerr << "Error loading font" << std::endl;
                return -1;
            }

            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over! Press R to Restart");
            gameOverText.setCharacterSize(36);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(200, 250);

            window.clear(sf::Color::White);
            window.draw(gameOverText);
            displayScore(window, score);
            window.display();
            continue;
        }

        if (isPaused) {
            // Display paused text
            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) {
                std::cerr << "Error loading font" << std::endl;
                return -1;
            }

            sf::Text pausedText;
            pausedText.setFont(font);
            pausedText.setString("Game Paused! Press P to Resume");
            pausedText.setCharacterSize(36);
            pausedText.setFillColor(sf::Color::Yellow);
            pausedText.setPosition(200, 250);

            window.clear(sf::Color::White);
            window.draw(pausedText);
            displayScore(window, score);
            window.display();
            continue;
        }

        // Player car movement (using arrow keys)
        playerCar.movingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        playerCar.movingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        playerCar.movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        playerCar.movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        playerCar.move();

        // Traffic AI movement
        for (auto &car : traffic) {
            car.aiMove();
            car.move();
            car.resetPosition();
        }

        // Pedestrian AI movement
        for (auto &person : people) {
            person.aiMove();
        }

        // Check for collisions between cars and pedestrians
        for (auto &car : traffic) {
            for (auto &person : people) {
                if (checkCollision(car.shape, person.shape)) {
                    collisionSound.play();
                    car.stop();
                    person.shape.setFillColor(sf::Color::Green); // Pedestrian turns green on collision
                }
            }
        }

        // Check for collisions between player car and pedestrians
        for (auto &person : people) {
            if (checkCollision(playerCar.shape, person.shape)) {
                collisionSound.play();
                gameOver = true;
                break;
            }
        }

        // Update score based on time alive
        score++;

        // Add more traffic over time
        addTraffic(traffic);

        // Clear the window
        window.clear(sf::Color::White);

        // Draw background (simple scrolling effect)
        sf::RectangleShape background(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color::Cyan); // Sky color
        window.draw(background);

        // Draw everything
        window.draw(playerCar.shape);
        for (auto &car : traffic) {
            window.draw(car.shape);
        }
        for (auto &person : people) {
            window.draw(person.shape);
        }

        // Display score
        displayScore(window, score);

        // Display everything
        window.display();
    }

    return 0;
}

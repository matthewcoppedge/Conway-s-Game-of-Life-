#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

typedef std::vector<std::vector<sf::RectangleShape>> GRID;

const int screenWidth = 1920;
const int screenHeight = 1080;
const int gridSize = 10; 

bool start = false;
bool reset_scene = false;
bool randomize = false;

void randomizeBoard(GRID& grid) {
    for (auto& row : grid) {
        for(auto& cel : row) {
            int a = rand() % 100;
            (a <= 10) ? cel.setFillColor(sf::Color::Blue) : cel.setFillColor(sf::Color::White); 
        }
    }
}

void reset(GRID& grid) {
    for (auto& row : grid) {
        for(auto& cel : row) {
            cel.setFillColor(sf::Color::White);
        }
    }
}

int countAliveNeighbors(const GRID& grid, int x, int y) {
    int total = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            if ( (y + i) >= 0 && (x + j) >= 0 && (y + i) < grid.size() && (x + j) < grid[y].size() && grid[y + i][x + j].getFillColor() == sf::Color::Blue)
                ++total;
        }
    }

    return total;
}

void updateCells(GRID& grid) {

    std::vector<std::vector<bool>> updateFlags(grid.size(), std::vector<bool>(grid[0].size(), false));

    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            int aliveNeighbors = countAliveNeighbors(grid, x, y);
            if (grid[y][x].getFillColor() == sf::Color::Blue) {
                updateFlags[y][x] = (aliveNeighbors == 2 || aliveNeighbors == 3);
            }
            else {
                updateFlags[y][x] = (aliveNeighbors == 3);
            }
        }
    }

    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            grid[y][x].setFillColor(updateFlags[y][x] ? sf::Color::Blue : sf::Color::White);
        }
    }

}

void drawGrid(sf::RenderWindow& window, const GRID& grid) {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            window.draw(cell);
        }
    }
}


int main() {

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Cellular Automata");

    // Create 2D vector to represent the grid
    std::vector<std::vector<sf::RectangleShape>> grid;
    for (int y = 0; y < screenHeight; y += gridSize) {
        std::vector<sf::RectangleShape> row;
        for (int x = 0; x < screenWidth; x += gridSize) {
            sf::RectangleShape cell(sf::Vector2f(gridSize, gridSize));
            cell.setPosition(x, y);
            cell.setFillColor(sf::Color::White);
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(0.5f);
            row.push_back(cell);
        }
        grid.push_back(row);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    start = !start;
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    randomize = !randomize;
                }
            }


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    reset_scene = !reset_scene;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    int cellX = static_cast<int>(worldPos.x) / gridSize;
                    int cellY = static_cast<int>(worldPos.y) / gridSize;

                    if (cellX >= 0 && cellX < grid[0].size() && cellY >= 0 && cellY < grid.size()) {
                        grid[cellY][cellX].setFillColor(sf::Color::Blue);
                    }
                }
            }
        }
        if(randomize) {
            start = false;
            reset_scene = false;
            randomizeBoard(grid);
            randomize = false;
        }

        if (start)
            updateCells(grid);

        if (reset_scene) {
            reset(grid);
            start = false;
            reset_scene = false;
        }
        

        window.clear(sf::Color::White);
        drawGrid(window, grid);
        window.display();
    }

    return 0;
}


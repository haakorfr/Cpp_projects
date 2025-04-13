
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Minesweeper.h"

using namespace std;

const sf::Color open_fill_color = sf::Color::White, closed_fill_color(192, 192, 192);
const sf::Color number_colors[9] = {
    sf::Color::White,
    sf::Color::Blue,
    sf::Color(0, 128, 0),
    sf::Color::Red,
    sf::Color(0, 0, 128),
    sf::Color(128, 0, 0),
    sf::Color(0, 128, 128),
    sf::Color::Black,
    sf::Color(128, 128, 128)
};
const sf::Color mine_color = sf::Color::Red;

const int tile_size = 32;
const int border_size = 2;

int main() {
    srand(time(nullptr));

    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")){
        cout << "Fant ikke skrifttypen 'sansation.ttf'" << endl;
        exit(0);
    }


    cout << "Skriv inn høyde, bredde og antall miner: ";
    int height = 20, width = 30, mines = 40;
    cin >> height >> width >> mines;

    Minesweeper* game = new Minesweeper(width, height, mines);

    sf::RenderWindow window(sf::VideoMode(width * tile_size, height * tile_size), "Minesweeper", sf::Style::Close);

    cout << "Velkommen til Minesveiper!" << endl;
    cout << "Klikk en rute for å åpne den. Trykk ESC eller Q for å avslutte, eller MELLOMROM for å starte på nytt" << endl;

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                case sf::Keyboard::Escape:
                case sf::Keyboard::Q:
                    window.close();
                    break;
                case sf::Keyboard::Space:
                    delete game;
                    game = new Minesweeper(width, height, mines);
                    break;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left && !game->isGameOver()) {
                    int row = event.mouseButton.y / tile_size;
                    int col = event.mouseButton.x / tile_size;

                    game->openTile(row, col);

                    if (game->isGameOver()) {
                        cout << "SPILLET ER OVER! Trykk ESC eller Q for å avslutte, eller MELLOMROM for å starte på nytt" << endl;
                    }
                }
                break;
            }
        }

        window.clear();


        for(int row = 0; row < height; ++row) {
            for(int col = 0; col < width; ++col) {
                const int tile_x = col * tile_size, tile_y = row * tile_size;

                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
                tile.setFillColor(game->isTileOpen(row, col) ? open_fill_color : closed_fill_color);
                tile.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

                window.draw(tile);

                if (game->isTileOpen(row, col) || (game->isGameOver() && game->isTileMine(row, col))) {
                    sf::Text text;
                    text.setStyle(sf::Text::Bold);
                    text.setCharacterSize(tile_size / 2.0);

                    if (game->isTileMine(row, col)) {
                        text.setString("X");
                        text.setFillColor(mine_color);
                    }
                    else {
                        int num_adjacent_mines = game->numAdjacentMines(row, col);
                        if(num_adjacent_mines == 0) continue; // Ikke tegn nuller
                        text.setString(to_string(num_adjacent_mines));
                        text.setFillColor(number_colors[num_adjacent_mines]);
                    }

                    text.setFont(font);

                    sf::FloatRect text_rect = text.getLocalBounds();
                    text.setOrigin(text_rect.left + text_rect.width  / 2.0,
                                   text_rect.top  + text_rect.height / 2.0);
                    text.setPosition(tile_x + tile_size / 2.0, tile_y + tile_size / 2.0);

                    window.draw(text);
                }
            }
        }

        window.display();
    }

    delete game;

    return 0;
}

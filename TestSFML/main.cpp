#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

std::vector<sf::Vector2f> getStarVertices(float radius, sf::Vector2f center, float angle)
{
    std::vector<sf::Vector2f> result;
    result.reserve(7);

    for (int i = 0; i < 7; ++i)
    {
        float alpha = 2.f * 3.1415926535f / 7.f * static_cast<float>(i);
        float x = radius * std::cos(alpha + angle);
        float y = radius * std::sin(alpha + angle);
        result.push_back(center + sf::Vector2f(x, y));
    }

    return result;
}






int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
    window.setFramerateLimit(60);
    sf::CircleShape shape(150.f, 3);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(300, 300);
    shape.setOrigin(75, 75);

    sf::RectangleShape rectangle(sf::Vector2f(200.f, 200.f));
    rectangle.setPosition(300.f, 300.f);
    rectangle.setOrigin(100.f, 100.f);
    rectangle.setFillColor(sf::Color::Blue);

    float angle = 0.f;

    sf::Texture texture;
    texture.loadFromFile("C:/Users/thoma/Downloads/cours3decembre2/Truc.bmp");

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Font font;
    if (!font.loadFromFile("C:/Users/thoma/Downloads/cours3decembre2/Venus Rising Rg.otf"))
        std::cerr << "Failed to load font" << std::endl;

    sf::Text text;
    text.setFont(font);

    text.setString("Hello world");

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("C:/Users/thoma/Downloads/cours3decembre2/Plants vs. Zombies Garden Warfare 2 - Graveyard Ops OST - Special Wave (Version B) (Extended).mp3"))
        return -1;
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    text.setCharacterSize(24); // in pixels, not points!
    text.setFillColor(sf::Color::White);

    const sf::Clock clock;
    const sf::Clock spawnClock;
    const sf::Time refreshTime = sf::seconds(1.f / 60.f);
    auto startSpawn = spawnClock.getElapsedTime().asMilliseconds();
    auto previous = clock.getElapsedTime().asMilliseconds();
    auto lag = 0.0;

    int counter = 0;
    bool isStopped = false;

    while (window.isOpen())
    {
        if (const auto lastSpawnTick = spawnClock.getElapsedTime().asMilliseconds(); lastSpawnTick - startSpawn >= 1000)
        {
            if (!isStopped)
                text.setString(std::to_string(++counter));

            startSpawn = lastSpawnTick;
        }

        const auto current = clock.getElapsedTime().asMilliseconds();
        const auto elapsed = current - previous;
        previous = current;
        lag += elapsed;
        window.clear();

        // input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            sf::FloatRect boundingBox = rectangle.getGlobalBounds();

            sf::Vector2i localPosition = sf::Mouse::getPosition(window);

            if (boundingBox.contains(static_cast<sf::Vector2f>(localPosition)))
            {
                rectangle.setFillColor(sf::Color::Red);
            }
            else
                rectangle.setFillColor(sf::Color::Blue);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                isStopped = !isStopped;
        }


        std::vector<sf::Vertex> lines;
        auto P = getStarVertices(200.f, sf::Vector2f(300.f, 300.f), angle);
        angle = angle + 0.01f;

        for (int i = 0; i < 7; ++i)
        {
            sf::RectangleShape rectangle(sf::Vector2f(3.f, 3.f));
            rectangle.setPosition(P[i]);
            rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
        }

        lines.reserve(14);

        int startIdx = 0;
        for (int i = 0; i < 7; ++i)
        {
            int endIdx = (startIdx + 3) % 7;
            lines.push_back(P[startIdx]);
            lines.push_back(P[endIdx]);
            startIdx = endIdx;
        }

        sprite.setOrigin(64, 64);
        sprite.setPosition(300.f, 300.f);
        rectangle.setRotation(45);



        while (refreshTime.asMilliseconds() > 0.0
            && lag >= refreshTime.asMilliseconds())
        {
            // update
            sprite.setRotation(-2.f * (angle * 180 / 3.14));
            lag -= refreshTime.asMilliseconds();
        }

        window.draw(shape);
        window.draw(rectangle);
        window.display();

    }

    return 0;
}
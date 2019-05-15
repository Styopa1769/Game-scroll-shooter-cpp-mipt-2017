#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
void menu(RenderWindow & window) {

	sf::SoundBuffer buffer1;
	buffer1.loadFromFile("Sounds/shoot.ogg.");// тут загружаем в буфер что то
	sf::Sound sound1;
	sound1.setBuffer(buffer1);
	bool f1,f2,f3;
	f1 = 0; f2 = 0; f3 = 0;

	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("Sounds/buttonpress.ogg.");// тут загружаем в буфер что то
	sf::Sound sound2;
	sound2.setBuffer(buffer2);
	bool k1, k2, k3;
	k1 = 0; k2 = 0; k3 = 0;

	sf::Music music;//создаем объект музыки
	music.openFromFile("Sounds/MainMenuTheme.ogg");//загружаем файл
	
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/playw.png");
	menuTexture2.loadFromFile("images/credw.png");
	menuTexture3.loadFromFile("images/exitw.png");
	aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/menu.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 60);
	menu2.setPosition(100, 150);
	menu3.setPosition(100, 260);
	menuBg.setPosition(0, 0);

	//////////////////////////////МЕНЮ///////////////////
	if (isMenu) music.play();
	while (isMenu)
	{
		menu1.setColor(Color::Black);
		menu2.setColor(Color::Black);
		menu3.setColor(Color::Black);
		menuNum = 0;
		//window.clear(Color(129, 181, 221));

		if (IntRect(100, 60, 300, 60).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 160, 300, 60).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 260, 300, 60).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }
		
		if ((IntRect(100, 60, 300, 60).contains(Mouse::getPosition(window))) && (f1 == 0)) { sound1.play(); f1 = 1; }
		if (!IntRect(100, 60, 300, 60).contains(Mouse::getPosition(window))) f1 = 0;
		if ((IntRect(100, 160, 300, 60).contains(Mouse::getPosition(window))) && (f2 == 0)) { sound1.play(); f2 = 1; }
		if (!IntRect(100, 160, 300, 60).contains(Mouse::getPosition(window))) f2 = 0;
		if ((IntRect(100, 260, 300, 60).contains(Mouse::getPosition(window))) && (f3 == 0)) { sound1.play(); f3 = 1; }
		if (!IntRect(100, 260, 300, 60).contains(Mouse::getPosition(window))) f3 = 0;
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { sound2.play(); sf::sleep(sf::microseconds(300000)); isMenu = false; }//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { sound2.play(); sf::sleep(sf::microseconds(300000)); window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3)  { sound2.play(); sf::sleep(sf::microseconds(300000)); window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
	////////////////////////////////////////////////////
}
	

		

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include "iostream"
#include "level.h"
#include <vector>
#include <list>
#include <math.h>
#include "LifeBar.h"
#include "menu.h"


using namespace sf;
class fly{
public:
	float x, y;
	fly(float X, float Y)
	{
		x = X; y = Y;
	}
	void update(float time)
	{
		y -= 0.03*time;
		view.setCenter(x,y);
	}
};
////////////////////////////////////����� �����-��������//////////////////////////
class Entity {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer, shootTimer;
	int w, h, health, damage;
	bool life, isMove, playerNear, isShoot;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H){
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}

	virtual void update(float time) = 0;//��� ������� �������������� ��� �-���

};
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, stay } state;
	int playerScore;
	bool isShoot;
	bool exitFromTheMapBot, exitFromTheMapTop;

	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H){
		playerScore = isShoot = 0; state = stay; obj = lev.GetAllObjects(); exitFromTheMapBot = 0, exitFromTheMapTop = 0;
		if (name == "Player1"){
			sprite.setTextureRect(IntRect(205, 220, w, h));
		}
	}

	void control(){
		if (Keyboard::isKeyPressed){
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {
				state = up; speed = 0.1;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down; speed = 0.1;
			}

			/////�������
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				isShoot = true;
			}
		}
	}



	/*void checkCollisionWithMap(float Dx, float Dy)
	{

		for (int i = 0; i<obj.size(); i++)
		if (getRect().intersects(obj[i].rect))
		{
			if (obj[i].name == "solid")
			{
				if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; }
				if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0)	{ x = obj[i].rect.left - w; }
				if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; }
			}
		}
	}*/

	void update(float time)
	{
		control();
		switch (state)
		{
		case right: if (x < 1105) dx = speed; else dx = 0;  sprite.setTextureRect(IntRect(410, 220, 90, 60)); break;
		case left:  if (x > 0) dx = -speed; else dx = 0; sprite.setTextureRect(IntRect(10, 220, 90, 60)); break;
		case up: if (exitFromTheMapTop == false) dy = -speed; else dy = 0; sprite.setTextureRect(IntRect(205, 30, 90, 60)); break;
		case down: if (exitFromTheMapBot == false) dy = speed; else dy = 0; sprite.setTextureRect(IntRect(205, 420, 90, 60)); break;
		//case stay: break;
		}
		x += dx*time;
		//checkCollisionWithMap(dx, 0);
		y -= 0.03*time;
		//if (life) { setPlayerCoordinateForView(600, y + 80); }
		y += dy*time;
		//checkCollisionWithMap(0, dy);
		if ((dx == 0) && (dy == 0)) sprite.setTextureRect(IntRect(205, 220, 90, 60));
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0){ life = false; }
		if (!isMove){ speed = 0;}
		
		
	}
};



class Enemy :public Entity{
public:
	bool playerNear;
	bool isShoot;
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		playerNear = false;
		isShoot = false;

		if (name == "tankleft")
		{
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			health = 200;
			//dx = 0.1;
		}
		if (name == "tankright")
		{
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			health = 200;
			//dx = 0.1;
		}
		if (name == "heli")
		{
			sprite.setTextureRect(IntRect(0, 0, 32, 64));
			dx = 0.08;//����������� ��������� �� �
			health = 150;
		}
		if (name == "airplane")
		{
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			dx = 0.08;//����������� ��������� �� �
			health = 100;
		}
	}

	void update(float time)//������� ���������� ���������.
	{
		if (name == "airplane")
		{
			x += dx * time;//���������� �������� �� �����������
			moveTimer += time;//���������� ������
			if (moveTimer > 4000) { dx *= -1; moveTimer = 0; }//���� ������ �������� 2 ���, �� �������� ����������� �������� ���������,� ������ ����������
			sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
		}
		if (name == "heli")
		{
			x += dx * time;//���������� �������� �� �����������
			moveTimer += time;//���������� ������
			if (moveTimer > 4000) { dx *= -1; moveTimer = 0; }//���� ������ �������� 2 ���, �� �������� ����������� �������� ���������,� ������ ����������
			sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
		}
		if (name == "tankleft")
		{
			sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
		}
		if (name == "tankright")
		{
			sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
		}
	}
};

class Bullet :public Entity{//����� ����
public:
	int direction;//����������� ����
	int damage = 0;
	Bullet(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int dir, int DAMAGE) :Entity(image, Name, X, Y, W, H){//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
		direction = dir;
		damage = DAMAGE;
		speed = 0.1;
		w = 10;
		h = 20;
		if (name == "Bullet1"){
			sprite.setTextureRect(IntRect(0, 1, w, h));
			damage = 4;
		}
		if (name == "Bullet2"){
			sprite.setTextureRect(IntRect(0, 29, w, h));
			damage = 3;
		}
		if (name == "Bullet3"){
			sprite.setTextureRect(IntRect(0, 53, w, h));
			damage = 2;
		}
		if (name == "Bullet4"){
			sprite.setTextureRect(IntRect(0, 74, w, h));
			damage = 1;
		}
		if (name == "Bullet5"){
			sprite.setTextureRect(IntRect(0, 96, 10, 10));
		}
		life = true;
		//���� ������������� � ������������
	}


	void update(float time)
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;//������� �������� state = left
		case 1: dx = speed; dy = 0;   break;//������� �������� state = right
		case 2: dx = 0; dy = speed;   break;//������� �������� state = down
		case 3: dx = 0; dy = -speed;   break;//������� �������� state = up
		}

		x += dx*time;//���� �������� ���� �� �
		y += dy*time;//�� �

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
	}
};

class Bonus : public Entity
{
public:
	Bonus(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		if (name == "bonus"){
			sprite.setTextureRect(IntRect(0, 0, 58, 58));
		}
	}
	void update(float time)
	{
		sprite.setPosition(x, y);
		if (health <= 0){ life = false; }
	}
};



int main()
{
	RenderWindow window(VideoMode(1280, 720), "Game"); menu(window);
	view.reset(FloatRect(0, 0, 1280, 720));

	Level lvl;
	lvl.LoadFromFile("map.tmx");

	Image heroImage;
	heroImage.loadFromFile("images/airplane.png");

	Image heliImage;
	heliImage.loadFromFile("images/heli.png");
	//heliImage.createMaskFromColor(Color(255, 255, 255));

	Image tankLeftImage;
	tankLeftImage.loadFromFile("images/tankleft.png");
	//tankLeftImage.createMaskFromColor(Color(255, 255, 255));

	Image tankRightImage;
	tankRightImage.loadFromFile("images/tankright.png");
	//tankRightImage.createMaskFromColor(Color(255, 255, 255));

	Image airplaneImage;
	airplaneImage.loadFromFile("images/air.png");
	airplaneImage.createMaskFromColor(Color(255, 255, 255));

	Image bonusImage;
	bonusImage.loadFromFile("images/bonus.png");
	bonusImage.createMaskFromColor(Color(255, 255, 255));

	Image healthImage;
	healthImage.loadFromFile("images/health.png");
	healthImage.createMaskFromColor(Color(255, 255, 255));

	Texture t;
	Sprite s;

	t.loadFromImage(healthImage);
	s.setTexture(t);
	s.setTextureRect(IntRect(0, 0, 188, 56));

	Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������
	BulletImage.createMaskFromColor(Color(0, 0, 0));//����� ��� ���� �� ������� �����

	SoundBuffer shootBuffer1;//������ ����� ��� �����
	shootBuffer1.loadFromFile("Sounds/Shot.wav");//��������� � ���� ����
	Sound shot1(shootBuffer1);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer shootBuffer2;//������ ����� ��� �����
	shootBuffer2.loadFromFile("Sounds/Shot2.ogg");//��������� � ���� ����
	Sound shot2(shootBuffer2);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer shootBuffer3;//������ ����� ��� �����
	shootBuffer3.loadFromFile("Sounds/Shot3.ogg");//��������� � ���� ����
	Sound shot3(shootBuffer3);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer shootBuffer4;//������ ����� ��� �����
	shootBuffer4.loadFromFile("Sounds/Shot4.ogg");//��������� � ���� ����
	Sound shot4(shootBuffer4);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer shootBuffer5;//������ ����� ��� �����
	shootBuffer5.loadFromFile("Sounds/GameOver.ogg");//��������� � ���� ����
	Sound GameOver(shootBuffer5);//������� ���� � ��������� � ���� ���� �� ������


	
	Music music;//������� ������ ������
	music.openFromFile("Sounds/maintheme.ogg");//��������� ����
	music.setLoop(true);
	music.play();//������������� ������
	

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;//������ ��������.��� �������������� ����� ��������� ������


	std::vector<Object> he = lvl.GetObjects("heli");
	std::vector<Object> tl = lvl.GetObjects("tankleft");
	std::vector<Object> tr  = lvl.GetObjects("tankright");
	std::vector<Object> ap = lvl.GetObjects("airplane");

	std::vector<Object> k = lvl.GetObjects("bonus");

	std::cout << "k. size " << k.size() << "\n";

	for (int i = 0; i < he.size(); i++) entities.push_back(new Enemy(heliImage, "heli", lvl, he[i].rect.left, he[i].rect.top, 32, 64));
	for (int i = 0; i < tl.size(); i++) entities.push_back(new Enemy(tankLeftImage, "tankleft", lvl, tl[i].rect.left, tl[i].rect.top, 32, 32));
	for (int i = 0; i < tr.size(); i++) entities.push_back(new Enemy(tankRightImage, "tankright", lvl, tr[i].rect.left, tr[i].rect.top, 32, 32));
	for (int i = 0; i < ap.size(); i++) entities.push_back(new Enemy(airplaneImage, "airplane", lvl, ap[i].rect.left, ap[i].rect.top, 32, 32));

	for (int i = 0; i < k.size(); i++)  entities.push_back(new Bonus(bonusImage, "bonus", lvl, k[i].rect.left, k[i].rect.top, 58, 58));

	std::cout << "ent. size " << entities.size() << "\n";

	Object player = lvl.GetObject("player");
	
	Player p(heroImage, "Player1", lvl, player.rect.left - 20, player.rect.top, 90, 60);
	
	fly fly(640, p.y - 250);

	float start = p.y -250;

	Clock clock;
	
	float movetimer = 0, musicTimer = 8000;

	int bonus_num = 0, i = 0;

	Font font;//����� 
	font.loadFromFile("CyrilicOld.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 100);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setColor(Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text.setStyle(Text::Bold);//������ �����.
	Text text2("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text2.setColor(Color::Black);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text2.setStyle(Text::Bold);//������ �����.

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			movetimer += time;
			if (event.type == sf::Event::Closed)
				window.close();
			if ((p.isShoot == true) && (movetimer > 1800))
			{
				if (bonus_num == 0)
				{ 
					p.isShoot = false; 
					entities.push_back(new Bullet(BulletImage, "Bullet4", lvl, p.x + 53, p.y, 10, 20, 3, 1));
					entities.push_back(new Bullet(BulletImage, "Bullet4", lvl, p.x + 27, p.y, 10, 20, 3, 1));
					shot1.play();//������ ���� ����
					movetimer = 0; 
				}
				if (bonus_num == 1)
				{
					p.isShoot = false;
					entities.push_back(new Bullet(BulletImage, "Bullet3", lvl, p.x + 53, p.y, 10, 20, 3, 2));
					entities.push_back(new Bullet(BulletImage, "Bullet3", lvl, p.x + 27, p.y, 10, 20, 3, 2));
					shot2.play();//������ ���� ����
					movetimer = 0;
				}
				if (bonus_num == 2)
				{
					p.isShoot = false;
					entities.push_back(new Bullet(BulletImage, "Bullet2", lvl, p.x + 53, p.y, 10, 20, 3, 3));
					entities.push_back(new Bullet(BulletImage, "Bullet2", lvl, p.x + 27, p.y, 10, 20, 3, 3));
					shot3.play();//������ ���� ����
					movetimer = 0;
				}
				if (bonus_num == 3)
				{
					p.isShoot = false;
					entities.push_back(new Bullet(BulletImage, "Bullet1", lvl, p.x + 53, p.y, 10, 20, 3, 4));
					entities.push_back(new Bullet(BulletImage, "Bullet1", lvl, p.x + 27, p.y, 10, 20, 3, 4));
					shot4.play();//������ ���� ����
					movetimer = 0;
				}
			}

		}

		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false || ((b->y - p.y) > 800) || (p.health <= 0))	{ it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else { it++;}//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}

		for (it = entities.begin(); it != entities.end(); it++)//���������� �� ��-��� ������
		{
			//std::cout << "bonus x =" << (*it)->x << "bonus y =" << (*it)->y << "\n";
			if (((*it)->name == "bonus") && (p.getRect().intersects((*it)->getRect())))
			{
				std::cout << "x of it " << (*it)->getRect().left << "\n";
				std::cout << "y of it " << (*it)->getRect().top << "\n";
				std::cout << "height of it " << (*it)->getRect().height << "\n";
				std::cout << "width of it " << (*it)->getRect().width << "\n";
				std::cout << "x of p " << p.getRect().left << "\n";
				std::cout << "y of p " << p.getRect().top << "\n";
				std::cout << "height of p " << p.getRect().height << "\n";
				std::cout << "width of p " << p.getRect().width << "\n";
				(*it)->life = false;
				std::cout << "bonus x =" << (*it)->x << "bonus y =" << (*it)->y << "\n";
				std::cout << "coords of char: " << p.x << " " << p.y << "\n";
				std::cout << "bonus podobran \n";
				bonus_num++;
			}

			if (((*it)->name == "Bullet1") || ((*it)->name == "Bullet2") || ((*it)->name == "Bullet3") || ((*it)->name == "Bullet4"))
			{
					for (it2 = entities.begin(); it2 != entities.end(); it2++)//���������� �� ��-��� ������
					{
						if ((*it2)->playerNear)
						if ((((*it2)->name == "airplane") || ((*it2)->name == "tankright") || ((*it2)->name == "tankleft") || ((*it2)->name == "heli")) && ((*it2)->getRect().intersects((*it)->getRect())))
						{
							if ((*it)->name == "Bullet1")
							(*it2)->health -= 80;
							if ((*it)->name == "Bullet2")
								(*it2)->health -= 60;
							if ((*it)->name == "Bullet3")
								(*it2)->health -= 40;
							if ((*it)->name == "Bullet4")
								(*it2)->health -= 20;
							(*it)->life = false;
							if ((*it2)->health <= 0)
							{
								(*it2)->life = false;
								if ((*it2)->name == "tankleft")
									p.playerScore += 200;
								if ((*it2)->name == "tankright")
									p.playerScore += 200;
								if ((*it2)->name == "air")
									p.playerScore += 100;
								if ((*it2)->name == "heli")
									p.playerScore += 150;
							}
							std::cout << "health: " << (*it2)->health << "\n";
						}
					}
				if ((fly.y - (*it)->y) > 720) (*it)->life = false;
			}

			if (((*it)->name == "airplane") || ((*it)->name == "tankleft") || ((*it)->name == "tankright") || ((*it)->name == "heli"))
			{
				if ((p.y - (*it)->y) < 500 && (((*it)->y - p.y) < 500)) (*it)->playerNear = true; else (*it)->playerNear = false;
				if ((*it)->playerNear)
				{
					(*it)->isShoot = true;
					(*it)->shootTimer += time;
				}
				if ((*it)->isShoot == true && ((*it)->shootTimer > 3000))
				{
					(*it)->shootTimer = 0;
					p.isShoot = false;
					if (((*it)->name == "airplane") || ((*it)->name == "heli"))
						entities.push_back(new Bullet(BulletImage, "Bullet5", lvl, (*it)->x, (*it)->y, 10, 10, 2, 1));
					if ((*it)->name == "tankleft")
						entities.push_back(new Bullet(BulletImage, "Bullet5", lvl, (*it)->x, (*it)->y, 10, 10, 1, 1));
					if ((*it)->name == "tankright")
						entities.push_back(new Bullet(BulletImage, "Bullet5", lvl, (*it)->x, (*it)->y, 10, 10, 0, 1));
				}
			}
			if (((*it)->name == "Bullet5") && (p.getRect().intersects((*it)->getRect())))
			{
				(*it)->life = false;
				p.health -= 20;
			}

		}
		//std::cout << "coords of fly: " << fly.x << " " << fly.y << "\n";

		if ((p.health) > 0 && (fly.y > 0))
		{
			fly.update(time);
			if ((p.y - fly.y) > 240) p.exitFromTheMapBot = true; else p.exitFromTheMapBot = false;
			if ((fly.y - p.y) > 240) p.exitFromTheMapTop = true; else p.exitFromTheMapTop = false;
			p.update(time);//update ������
		}
		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);
		//std::cout << "ent. size " << entities.size() << "\n";
		for (it = entities.begin(); it != entities.end(); it++)
		{
			if (((*it)->name == "air") || ((*it)->name == "tankleft") || ((*it)->name == "tankright") || ((*it)->name == "heli"))
			{
				if ((*it)->playerNear)
				{
					window.draw((*it)->sprite); i++;
				}
			}
			else 
			{
				window.draw((*it)->sprite); i++;
			}
		}
		//std::cout << "narisovano: " << i << "\n";
		i = 0;
		window.draw(p.sprite);
		if (p.health <= 0)
		{
			music.stop();
			musicTimer += time;
			if (musicTimer > 8000)
			{
				musicTimer = 0;
				GameOver.play();
			}
			std::ostringstream playerHealthString;    // �������� ����������
			playerHealthString << p.health;		//������� � ��� ����� �����, �� ���� ��������� ������
			text.setString("GAME OVER");//������ ������ ������ � �������� �������������� ���� ������ ������� .str() 
			text.setPosition(view.getCenter().x-200, view.getCenter().y-100);//������ ������� ������, �������� �� ������ ������
			window.draw(text);//����� ���� �����
		}
		std::ostringstream playerScoreString;    // �������� ����������
		playerScoreString << p.playerScore + int(start-fly.y);		//������� � ��� ����� �����, �� ���� ��������� ������
		text2.setString("SCORE: " + playerScoreString.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str() 
		text2.setPosition(fly.x - 580, fly.y - 350);//������ ������� ������, �������� �� ������ ������
		window.draw(text2);//����� ���� �����
		if (p.health >= 0)
		if (p.health <= 100)
		{
			s.setTextureRect(IntRect(150 - (p.health / 20) * 30, 0, 150, 30));
		}
		if (fly.y <= 10)
		{
			std::ostringstream playerHealthString;    // �������� ����������
			playerHealthString << p.health;		//������� � ��� ����� �����, �� ���� ��������� ������
			text.setString("YOU WIN");//������ ������ ������ � �������� �������������� ���� ������ ������� .str() 
			text.setPosition(view.getCenter().x - 200, view.getCenter().y - 100);//������ ������� ������, �������� �� ������ ������
			window.draw(text);//����� ���� �����
		}
		s.setPosition(fly.x + 600 - p.health, fly.y - 350);
		window.draw(s);
		window.display();
	}
	return 0;
}
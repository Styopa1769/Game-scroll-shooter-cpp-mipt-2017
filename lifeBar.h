#include <SFML/Graphics.hpp>
using namespace sf;

class LifeBar
{
public:
	Image image;
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;

	LifeBar()
	{
		image.loadFromFile("images/health.png");
		image.createMaskFromColor(Color(255,255,255));
		t.loadFromImage(image);
		s.setTexture(t);
		s.setTextureRect(IntRect(0, 0, 188, 56));
		max = 100;
	}

	void update(int k)// k-текущее здоровье

	{
		if (k >= 0)
		if (k <= max)
			s.setTextureRect(IntRect(188-(k/20)*37*6, 0, 188-(k/20)*37*6, 56));//если не отрицательно и при этом меньше максимума, то устанавливаем новое значение (новый размер) для черного прямоугольника
	}

	void position(float x, float y)
	{
		s.setPosition(x,y);//позиция на экране
	}

};
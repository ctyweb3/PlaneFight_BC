// 飞机大战
// EasyX
#include <iostream>
#include <graphics.h> //easyx
#include <vector>
#include <conio.h>
#include <fstream> // 添加文件读写头文件
using namespace std;

constexpr auto swidth = 600;
constexpr auto sheight = 1100;

constexpr unsigned int SHP = 4;

constexpr auto hurttime = 1000; // ms

// 文件路径常量
constexpr auto SCORE_FILE = "highscore.dat";

bool PointInRect(int x, int y, RECT &r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}

bool RectDuangRect(RECT &r1, RECT &r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}

// 设置界面函数
void Settings()
{
	GameSettings settings;

	LPCTSTR title = _T("游戏设置");
	LPCTSTR difficultyText = _T("难度：");
	LPCTSTR bulletTypeText = _T("子弹类型：");
	LPCTSTR soundText = _T("声音：");
	LPCTSTR backText = _T("返回");

	RECT difficultyRect, bulletTypeRect, soundRect, backRect;
	TCHAR difficultyValue[50], bulletTypeValue[50], soundValue[50];

	// 更新显示文本
	auto updateText = [&]()
	{
		switch (settings.difficulty)
		{
		case 0:
			_stprintf_s(difficultyValue, 50, _T("简单"));
			break;
		case 1:
			_stprintf_s(difficultyValue, 50, _T("普通"));
			break;
		case 2:
			_stprintf_s(difficultyValue, 50, _T("困难"));
			break;
		default:
			_stprintf_s(difficultyValue, 50, _T("普通"));
			break;
		}

		switch (settings.bulletType)
		{
		case 0:
			_stprintf_s(bulletTypeValue, 50, _T("普通子弹"));
			break;
		case 1:
			_stprintf_s(bulletTypeValue, 50, _T("跟踪子弹"));
			break;
		case 2:
			_stprintf_s(bulletTypeValue, 50, _T("散射子弹"));
			break;
		default:
			_stprintf_s(bulletTypeValue, 50, _T("普通子弹"));
			break;
		}

		_stprintf_s(soundValue, 50, settings.enableSound ? _T("开启") : _T("关闭"));
	};

	updateText();

	while (true)
	{
		BeginBatchDraw();
		setbkcolor(WHITE);
		cleardevice();

		// 标题
		settextstyle(50, 0, _T("宋体"));
		settextcolor(BLACK);
		outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 8, title);

		// 设置选项
		settextstyle(30, 0, _T("宋体"));

		// 难度设置
		difficultyRect.left = swidth / 4;
		difficultyRect.top = sheight / 8 * 2.5;
		difficultyRect.right = difficultyRect.left + textwidth(difficultyText) + textwidth(difficultyValue);
		difficultyRect.bottom = difficultyRect.top + textheight(difficultyText);

		outtextxy(difficultyRect.left, difficultyRect.top, difficultyText);
		outtextxy(difficultyRect.left + textwidth(difficultyText), difficultyRect.top, difficultyValue);

		// 子弹类型设置
		bulletTypeRect.left = swidth / 4;
		bulletTypeRect.top = sheight / 8 * 3.5;
		bulletTypeRect.right = bulletTypeRect.left + textwidth(bulletTypeText) + textwidth(bulletTypeValue);
		bulletTypeRect.bottom = bulletTypeRect.top + textheight(bulletTypeText);

		outtextxy(bulletTypeRect.left, bulletTypeRect.top, bulletTypeText);
		outtextxy(bulletTypeRect.left + textwidth(bulletTypeText), bulletTypeRect.top, bulletTypeValue);

		// 声音设置
		soundRect.left = swidth / 4;
		soundRect.top = sheight / 8 * 4.5;
		soundRect.right = soundRect.left + textwidth(soundText) + textwidth(soundValue);
		soundRect.bottom = soundRect.top + textheight(soundText);

		outtextxy(soundRect.left, soundRect.top, soundText);
		outtextxy(soundRect.left + textwidth(soundText), soundRect.top, soundValue);

		// 返回按钮
		backRect.left = swidth / 2 - textwidth(backText) / 2;
		backRect.top = sheight / 8 * 6;
		backRect.right = backRect.left + textwidth(backText);
		backRect.bottom = backRect.top + textheight(backText);

		outtextxy(backRect.left, backRect.top, backText);

		EndBatchDraw();

		// 处理鼠标点击
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			// 难度设置
			if (PointInRect(mess.x, mess.y, difficultyRect))
			{
				settings.difficulty = (settings.difficulty + 1) % 3;
				updateText();
				settings.SaveSettings();
			}
			// 子弹类型设置
			else if (PointInRect(mess.x, mess.y, bulletTypeRect))
			{
				settings.bulletType = (settings.bulletType + 1) % 3;
				updateText();
				settings.SaveSettings();
			}
			// 声音设置
			else if (PointInRect(mess.x, mess.y, soundRect))
			{
				settings.enableSound = !settings.enableSound;
				updateText();
				settings.SaveSettings();
			}
			// 返回
			else if (PointInRect(mess.x, mess.y, backRect))
			{
				return;
			}
		}
	}
}

// 一个简单主菜单
void Welcome()
{
	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR tsettings = _T("游戏设置");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, tsettingsr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);

	settextstyle(40, 0, _T("宋体"));
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5 * 2;
	tplayr.bottom = tplayr.top + textheight(tplay);

	tsettingsr.left = swidth / 2 - textwidth(tsettings) / 2;
	tsettingsr.right = tsettingsr.left + textwidth(tsettings);
	tsettingsr.top = sheight / 5 * 3;
	tsettingsr.bottom = tsettingsr.top + textheight(tsettings);

	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5 * 4;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(tsettingsr.left, tsettingsr.top, tsettings);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (PointInRect(mess.x, mess.y, tplayr))
			{
				return;
			}
			else if (PointInRect(mess.x, mess.y, tsettingsr))
			{
				Settings();
				// 重新显示主菜单
				BeginBatchDraw();
				setbkcolor(WHITE);
				cleardevice();
				settextstyle(60, 0, _T("宋体"));
				settextcolor(BLACK);
				outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);

				settextstyle(40, 0, _T("宋体"));
				outtextxy(tplayr.left, tplayr.top, tplay);
				outtextxy(tsettingsr.left, tsettingsr.top, tsettings);
				outtextxy(texitr.left, texitr.top, texit);
				EndBatchDraw();
			}
			else if (PointInRect(mess.x, mess.y, texitr))
			{
				exit(0);
			}
		}
	}
}

// 读取最高分数
unsigned long long ReadHighScore()
{
	unsigned long long highScore = 0;
	ifstream inFile(SCORE_FILE, ios::binary);
	if (inFile)
	{
		inFile.read(reinterpret_cast<char *>(&highScore), sizeof(highScore));
		inFile.close();
	}
	return highScore;
}

// 保存最高分数
void SaveHighScore(unsigned long long score)
{
	unsigned long long highScore = ReadHighScore();
	// 只有当当前分数高于历史最高分时才保存
	if (score > highScore)
	{
		ofstream outFile(SCORE_FILE, ios::binary);
		if (outFile)
		{
			outFile.write(reinterpret_cast<const char *>(&score), sizeof(score));
			outFile.close();
		}
	}
}

void Over(unsigned long long &kill)
{
	printf_s("o");
	TCHAR *str = new TCHAR[128];
	_stprintf_s(str, 128, _T("当前击杀数：%llu"), kill);

	// 显示当前分数
	settextcolor(RED);
	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);

	// 读取并显示历史最高分
	unsigned long long highScore = ReadHighScore();
	TCHAR *highScoreStr = new TCHAR[128];
	_stprintf_s(highScoreStr, 128, _T("历史最高分：%llu"), highScore);
	outtextxy(swidth / 2 - textwidth(highScoreStr) / 2, sheight / 5 + 40, highScoreStr);

	// 如果创造了新纪录，显示恭喜信息
	if (kill > highScore)
	{
		TCHAR *newRecordStr = new TCHAR[128];
		_stprintf_s(newRecordStr, 128, _T("恭喜您创造了新纪录！"));
		outtextxy(swidth / 2 - textwidth(newRecordStr) / 2, sheight / 5 + 80, newRecordStr);
		delete[] newRecordStr;

		// 保存新的最高分
		SaveHighScore(kill);
	}

	// 按Enter继续
	LPCTSTR info = _T("按Enter继续");
	settextstyle(20, 0, _T("宋体"));
	outtextxy(swidth - textwidth(info), sheight - textheight(info), info);

	delete[] str;
	delete[] highScoreStr;

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_KEY);
		if (mess.vkcode == 0x0D)
		{
			return;
		}
	}
}

// лӢۡӵ

class BK
{
public:
	BK(IMAGE &img)
		: img(img), y(-sheight)
	{
	}
	void Show()
	{
		if (y == 0)
		{
			y = -sheight;
		}
		y += 4;
		putimage(0, y, &img);
	}

private:
	IMAGE &img;
	int y;
};

class Hero
{
public:
	Hero(IMAGE &img)
		: img(img), HP(SHP)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;
	}
	void Show()
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 4);
		putimage(rect.left, rect.top, &img);
		line(rect.left, rect.top - 5, rect.left + (img.getwidth() / SHP * HP), rect.top - 5);
	}
	void Control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EM_MOUSE))
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();
		}
	}

	bool hurt()
	{
		HP--;
		return (HP == 0) ? false : true;
	}

	RECT &GetRect() { return rect; }

private:
	IMAGE &img;
	RECT rect;

	unsigned int HP;
};

class Enemy
{
public:
	Enemy(IMAGE &img, int x, IMAGE *&boom)
		: img(img), isdie(false), boomsum(0)
	{
		selfboom[0] = boom[0];
		selfboom[1] = boom[1];
		selfboom[2] = boom[2];
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;
	}
	bool Show()
	{

		if (isdie)
		{
			if (boomsum == 3)
			{
				return false;
			}
			putimage(rect.left, rect.top, selfboom + boomsum);
			boomsum++;

			return true;
		}

		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 4;
		rect.bottom += 4;
		putimage(rect.left, rect.top, &img);

		return true;
	}

	void Isdie()
	{
		isdie = true;
	}

	RECT &GetRect() { return rect; }

private:
	IMAGE &img;
	RECT rect;
	IMAGE selfboom[3];

	bool isdie;
	int boomsum;
};

class Bullet
{
public:
	Bullet(IMAGE &img, RECT pr)
		: img(img)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();
	}
	bool Show()
	{
		if (rect.bottom <= 0)
		{
			return false;
		}
		rect.top -= 3;
		rect.bottom -= 3;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	RECT &GetRect() { return rect; }

protected:
	IMAGE &img;
	RECT rect;
};

class EBullet : public Bullet
{
public:
	EBullet(IMAGE &img, RECT pr)
		: Bullet(img, pr)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.bottom;
		rect.bottom = rect.top + img.getheight();
	}
	bool Show()
	{
		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 5;
		rect.bottom += 5;
		putimage(rect.left, rect.top, &img);

		return true;
	}
};

// 跟踪子弹类，继承自EBullet，形成第二层继承
class TrackingBullet : public EBullet
{
public:
	TrackingBullet(IMAGE &img, RECT pr, RECT target)
		: EBullet(img, pr), targetRect(target), speed(3)
	{
	}

	bool Show()
	{
		if (rect.top >= sheight || rect.left < 0 || rect.right > swidth)
		{
			return false;
		}

		// 简单的跟踪逻辑：向目标方向移动
		int targetCenterX = targetRect.left + (targetRect.right - targetRect.left) / 2;
		int bulletCenterX = rect.left + (rect.right - rect.left) / 2;

		// 水平方向移动（跟踪目标）
		if (bulletCenterX < targetCenterX)
			rect.left += 1;
		else if (bulletCenterX > targetCenterX)
			rect.left -= 1;

		// 更新右边界
		rect.right = rect.left + img.getwidth();

		// 垂直方向移动
		rect.top += speed;
		rect.bottom += speed;

		putimage(rect.left, rect.top, &img);
		return true;
	}

private:
	RECT targetRect; // 目标位置
	int speed;		 // 子弹速度
};

// 散射子弹类，继承自TrackingBullet，形成第三层继承
class SpreadBullet : public TrackingBullet
{
public:
	SpreadBullet(IMAGE &img, RECT pr, RECT target, int spreadDirection)
		: TrackingBullet(img, pr, target), direction(spreadDirection)
	{
	}

	bool Show()
	{
		if (rect.top >= sheight || rect.left < 0 || rect.right > swidth)
		{
			return false;
		}

		// 散射方向：-1向左，0直下，1向右
		rect.left += direction * 2;
		rect.right = rect.left + img.getwidth();

		rect.top += 4;
		rect.bottom += 4;

		putimage(rect.left, rect.top, &img);
		return true;
	}

private:
	int direction; // 散射方向
};

// 游戏设置类，用于存储游戏配置
class GameSettings
{
public:
	GameSettings() : difficulty(1),
					 bulletType(0),
					 enableSound(true)
	{
		LoadSettings();
	}

	// 加载游戏设置
	void LoadSettings()
	{
		ifstream inFile("game_settings.txt");
		if (inFile)
		{
			inFile >> difficulty >> bulletType >> enableSound;
			inFile.close();
		}
	}

	// 保存游戏设置
	void SaveSettings()
	{
		ofstream outFile("game_settings.txt");
		if (outFile)
		{
			outFile << difficulty << " " << bulletType << " " << enableSound;
			outFile.close();
		}
	}

	int difficulty;	  // 难度级别（1-3）
	int bulletType;	  // 子弹类型（0-常规，1-跟踪，2-散射）
	bool enableSound; // 是否启用声音
};

bool AddEnemy(vector<Enemy *> &es, IMAGE &enemyimg, IMAGE *boom)
{
	Enemy *e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()), boom);
	for (auto &i : es)
	{
		if (RectDuangRect(i->GetRect(), e->GetRect()))
		{
			delete e;
			return false;
		}
	}
	es.push_back(e);
	return true;
}

bool Play()
{
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;

	// 加载游戏设置
	GameSettings settings;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;
	IMAGE eboom[3];

	loadimage(&heroimg, _T("../images/me1.png"));
	loadimage(&enemyimg, _T("../images/enemy1.png"));
	loadimage(&bkimg, _T("../images/bk2.png"), swidth, sheight * 2);
	loadimage(&bulletimg, _T("../images/bullet1.png"));

	loadimage(&eboom[0], _T("../images/enemy1_down2.png"));
	loadimage(&eboom[1], _T("../images/enemy1_down3.png"));
	loadimage(&eboom[2], _T("../images/enemy1_down4.png"));

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);

	vector<Enemy *> es;
	vector<Bullet *> bs;
	vector<EBullet *> ebs;
	// 新增跟踪子弹和散射子弹容器
	vector<TrackingBullet *> tbs;
	vector<SpreadBullet *> sbs;

	int bsing = 0;

	clock_t hurtlast = clock();

	unsigned long long kill = 0;

	// 根据难度调整敌人数量
	int enemyCount = 5 + settings.difficulty;
	for (int i = 0; i < enemyCount; i++)
	{
		AddEnemy(es, enemyimg, eboom);
	}

	while (is_play)
	{
		bsing++;
		if (bsing % 10 == 0)
		{
			// 根据设置选择不同类型的子弹
			switch (settings.bulletType)
			{
			case 0: // 普通子弹
				bs.push_back(new Bullet(bulletimg, hp.GetRect()));
				break;
			case 1: // 跟踪子弹
				if (!es.empty())
				{
					// 跟踪第一个敌人
					tbs.push_back(new TrackingBullet(bulletimg, hp.GetRect(), es[0]->GetRect()));
				}
				else
				{
					bs.push_back(new Bullet(bulletimg, hp.GetRect()));
				}
				break;
			case 2: // 散射子弹
				if (!es.empty())
				{
					// 创建3个散射子弹，分别向左、中、右方向
					for (int dir = -1; dir <= 1; dir++)
					{
						sbs.push_back(new SpreadBullet(bulletimg, hp.GetRect(), es[0]->GetRect(), dir));
					}
				}
				else
				{
					bs.push_back(new Bullet(bulletimg, hp.GetRect()));
				}
				break;
			default:
				bs.push_back(new Bullet(bulletimg, hp.GetRect()));
			}
		}
		if (bsing == 60)
		{
			bsing = 0;
			for (auto &i : es)
			{
				ebs.push_back(new EBullet(bulletimg, i->GetRect()));
			}
		}

		BeginBatchDraw();

		bk.Show();
		Sleep(2);
		flushmessage();
		Sleep(2);
		hp.Control();

		if (_kbhit())
		{
			char v = _getch();
			if (v == 0x20) // 空格暂停
			{
				Sleep(500);
				while (true)
				{
					if (_kbhit())
					{
						v = _getch();
						if (v == 0x20)
						{
							break;
						}
						else if (v == '1' || v == '2' || v == '3') // 按1-3切换子弹类型
						{
							settings.bulletType = v - '1';
							settings.SaveSettings();

							// 显示当前子弹类型提示
							TCHAR *tipStr = new TCHAR[128];
							switch (settings.bulletType)
							{
							case 0:
								_stprintf_s(tipStr, 128, _T("普通子弹"));
								break;
							case 1:
								_stprintf_s(tipStr, 128, _T("跟踪子弹"));
								break;
							case 2:
								_stprintf_s(tipStr, 128, _T("散射子弹"));
								break;
							}

							settextcolor(YELLOW);
							settextstyle(30, 0, _T("宋体"));
							outtextxy(swidth / 2 - textwidth(tipStr) / 2, sheight / 2, tipStr);
							delete[] tipStr;
							EndBatchDraw();
							Sleep(1000);
						}
					}
					Sleep(16);
				}
			}
		}
		hp.Show();

		// 处理普通子弹
		auto bsit = bs.begin();
		while (bsit != bs.end())
		{
			if (!(*bsit)->Show())
			{
				bsit = bs.erase(bsit);
			}
			else
			{
				bsit++;
			}
		}

		// 处理跟踪子弹
		auto tbsit = tbs.begin();
		while (tbsit != tbs.end())
		{
			if (!(*tbsit)->Show())
			{
				tbsit = tbs.erase(tbsit);
			}
			else
			{
				tbsit++;
			}
		}

		// 处理散射子弹
		auto sbsit = sbs.begin();
		while (sbsit != sbs.end())
		{
			if (!(*sbsit)->Show())
			{
				sbsit = sbs.erase(sbsit);
			}
			else
			{
				sbsit++;
			}
		}

		auto ebsit = ebs.begin();
		while (ebsit != ebs.end())
		{
			if (!(*ebsit)->Show())
			{
				ebsit = ebs.erase(ebsit);
			}
			else
			{
				if (RectDuangRect((*ebsit)->GetRect(), hp.GetRect()))
				{
					if (clock() - hurtlast >= hurttime)
					{
						is_play = hp.hurt();
						hurtlast = clock();
					}
				}
				ebsit++;
			}
		}

		auto it = es.begin();
		while (it != es.end())
		{
			if (RectDuangRect((*it)->GetRect(), hp.GetRect()))
			{
				if (clock() - hurtlast >= hurttime)
				{
					is_play = hp.hurt();
					hurtlast = clock();
				}
			}

			// 检查普通子弹碰撞
			auto bit = bs.begin();
			bool hit = false;
			while (bit != bs.end() && !hit)
			{
				if (RectDuangRect((*bit)->GetRect(), (*it)->GetRect()))
				{
					(*it)->Isdie();
					delete (*bit);
					bs.erase(bit);
					hit = true;
					kill++;
				}
				else
				{
					bit++;
				}
			}

			// 检查跟踪子弹碰撞
			auto tbit = tbs.begin();
			while (tbit != tbs.end() && !hit)
			{
				if (RectDuangRect((*tbit)->GetRect(), (*it)->GetRect()))
				{
					(*it)->Isdie();
					delete (*tbit);
					tbs.erase(tbit);
					hit = true;
					kill += 2; // 跟踪子弹给双倍分数
				}
				else
				{
					tbit++;
				}
			}

			// 检查散射子弹碰撞
			auto sbit = sbs.begin();
			while (sbit != sbs.end() && !hit)
			{
				if (RectDuangRect((*sbit)->GetRect(), (*it)->GetRect()))
				{
					(*it)->Isdie();
					delete (*sbit);
					sbs.erase(sbit);
					hit = true;
					kill += 1; // 散射子弹给额外分数
				}
				else
				{
					sbit++;
				}
			}

			if (!(*it)->Show())
			{
				delete (*it);
				es.erase(it);
				it = es.begin();
			}
			else
			{
				it++;
			}
		}

		// 根据难度调整敌人数量
		for (int i = 0; i < enemyCount - es.size(); i++)
		{
			AddEnemy(es, enemyimg, eboom);
		}

		EndBatchDraw();
	}

	// 释放所有动态分配的内存
	for (auto &bullet : bs)
		delete bullet;
	bs.clear();

	for (auto &tbullet : tbs)
		delete tbullet;
	tbs.clear();

	for (auto &sbullet : sbs)
		delete sbullet;
	sbs.clear();

	for (auto &ebullet : ebs)
		delete ebullet;
	ebs.clear();

	for (auto &enemy : es)
		delete enemy;
	es.clear();

	printf_s("e");
	// 游戏结束保存分数
	SaveHighScore(kill);
	Over(kill);

	return true;
}

int main()
{
	// easyxʼ
	initgraph(swidth, sheight, EW_NOMINIMIZE | EW_SHOWCONSOLE);
	bool is_live = true;
	while (is_live)
	{
		Welcome();
		// Play
		is_live = Play();
	}

	return 0;
}
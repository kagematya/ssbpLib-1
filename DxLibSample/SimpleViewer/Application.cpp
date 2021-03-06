#include "Application.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "ss/SS5ResourceManager.h"
#include "ss/SS5Player.h"
#include "ss/SS5EventListener.h"
#include "Menu.h"
using namespace std;
using namespace ss;


Application::Application()
	: m_ss5ResourceManager()
	, m_ss5Player(nullptr)
	, m_eventListener()
	, m_transform()
	, m_menu(nullptr)
	, m_isPlaying(false)
{
	fill(m_keyInputs.begin(), m_keyInputs.end(), 0);
}

Application::~Application(){

}


/*
	再生手順まとめ

	ssbpファイルを読み込み
	SS5ResourceManagerにssbpファイルを登録
	SS5ResourceManager::createPlayer()でプレーヤーを生成
	SS5Player::play()で再生
		あとは、update(), draw() を呼んでください
*/
void Application::initialize()
{
	/*
	 * ssbpファイルを読み込んで登録する
	 */
	
	//ファイル読み込み
	ifstream ifs("character_template_comipo/character_template1.ssbp", ios::in | ios::binary);
	assert(ifs);
	ifs.seekg(0, ios::end);
	int fileSize = ifs.tellg();			//ファイルサイズ取得
	ifs.seekg(0, ios::beg);

	vector<char> file(fileSize, 0);
	ifs.read(file.data(), file.size());	//ファイル読み込み

	//登録
	m_ss5ResourceManager.regist(
		file.data(),					//ssbpデータ
		file.size(),					//ssbpデータサイズ
		"ssbpRegistName",				//登録名
		"character_template_comipo/",	//テクスチャのあるフォルダを指定
		[&](int cellMapIndex, const string& filename, SsTexWrapMode wrapmode, SsTexFilterMode filtermode){	//テクスチャ読み込みのためのコールバック
			m_eventListener.texturePreloadCallback(cellMapIndex, filename, wrapmode, filtermode);
		}
	);
	

	
	/*
	 * 登録名からプレーヤーを生成する
	 */

	//生成
	m_ss5Player = m_ss5ResourceManager.createPlayer(
		&m_eventListener,					//イベント処理の指定
		"ssbpRegistName"					//登録名
	);



	/*
	 * プレーヤーの設定
	 */
	
	//再生させるにはアニメーション名を指定する
	m_ss5Player->play("character_template_3head/stance");

	//値を変えたい場合は次のようにして設定できます
	m_transform.setPosition(400, 150);			//表示位置
	m_transform.setScale(0.5f, 0.5f);			//スケール
	m_transform.setRotation(0.0f, 0.0f, 0.0f);	//回転

	m_ss5Player->setRootMatrix(m_transform.getMatrix());
	m_ss5Player->setAlpha(1.0);					//透明度



	//メニュー周り
	m_menu.reset(new MenuRoot(m_ss5Player, &m_transform));
	m_isPlaying = true;
}



void Application::finalize()
{
	//プレーヤーの破棄
	m_ss5ResourceManager.destroyPlayer(m_ss5Player);

	//登録解除
	m_ss5ResourceManager.unregist("ssbpRegistName");
}



void Application::update()
{
	char buf[256];
	GetHitKeyStateAll(buf);
	for(int i = 0; i < 256; ++i){
		buf[i] ? m_keyInputs[i]++ : m_keyInputs[i] = 0;		//押し続けたフレーム数になる
	}

	auto inputEdit = [](int &input){
		return static_cast<int>(input == 1 || input>10);	//押した瞬間と長押しだけに反応させたい
	};

	int up = inputEdit(m_keyInputs[KEY_INPUT_UP]);
	int down = inputEdit(m_keyInputs[KEY_INPUT_DOWN]);
	int left = inputEdit(m_keyInputs[KEY_INPUT_LEFT]);
	int right = inputEdit(m_keyInputs[KEY_INPUT_RIGHT]);
	int enter = inputEdit(m_keyInputs[KEY_INPUT_Z]);
	int cancel = inputEdit(m_keyInputs[KEY_INPUT_X]);

	if(m_keyInputs[KEY_INPUT_C] == 1){
		m_isPlaying = !m_isPlaying;		//ポーズ切り替え
	}

	m_menu->action(up, down, left, right, enter, cancel);


	static const int FPS = 60;
	float deltaTime = (m_isPlaying ? (1.0f / FPS) : 0);		//停止中は時間を進ませない

	m_ss5Player->setRootMatrix(m_transform.getMatrix());
	m_ss5Player->update(deltaTime);		//毎フレームのアップデート
}

void Application::draw()
{
	m_ss5Player->draw();				//描画

	stringstream stream;
	stream << "上下左右でメニュー操作。zで決定。xで初期値。cで再生/停止" << endl;
	m_menu->draw(stream);

	string str;
	int y = 10;
	while(getline(stream, str)){
		DrawString(10, y, str.c_str(), GetColor(255, 255, 255));
		y += 16;
	}
}


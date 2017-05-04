#pragma once

#include <memory>
#include "ss/SS5ResourceManager.h"
#include "EventListener.h"

namespace ss{
	class SS5Player;
}


class Application{
public:
	Application();
	~Application();

	void initialize();
	void finalize();

	void update();
	void draw();

private:
	
	ss::SS5ResourceManager m_ss5ResourceManager;	//ssbpファイルの登録先
	
	ss::SS5Player *m_ss5Player;						//ssbpファイルの再生単位
	EventListener m_eventListener;					//SS5Player向けのイベント処理の実装
};


#ifndef FILE_FOO_SEEN
#define FILE_FOO_SEEN
#include<iostream>
#include<fstream>
#include<thread>
#include<functional>
#include<chrono>
#include<queue>
#include<deque>
#include<mutex>
#include<condition_variable>
#include<cstdint>

#include<SFML/System.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

#include<zmq.hpp>

#include<nlohmann/json.hpp>

#include <utility>
#include <libplatform/libplatform.h>
#include <fstream>
#include "v8.h"

#include"GameObject.h"
#include"ThreadMaker.h"
#include"NetworkingClient.h"
#include"Timeline.h"
#include"EventHandler.h"
#include"Projectile.h"
#include"Player.h"
#include"Platform.h"
#include"ReplayHandler.h"
#include"HiddenObjects.h"
#include"v8helpers.h"
#include"ScriptManager.h"
#include"Game.h"

using json = nlohmann::json;
#endif /* !FILE_FOO_SEEN */
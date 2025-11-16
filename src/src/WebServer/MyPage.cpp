#include "../WebServer/MyPage.h"

#ifdef WEBSERVER_MY
#include "../WebServer/ESPEasy_WebServer.h"

void handle_mypage() {
  // Минимальный тестовый ответ
  web_server.send(200, F("text/plain"), F("Hello from MyPage"));
}
#endif
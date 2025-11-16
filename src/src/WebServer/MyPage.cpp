#include "../WebServer/MyPage.h"

#ifdef WEBSERVER_MY

#include "../WebServer/ESPEasy_WebServer.h"
#include "../WebServer/HTML_wrappers.h"
#include "../WebServer/Markup.h"
#include "../WebServer/Markup_Buttons.h"
#include "../WebServer/Markup_Forms.h"
#include "../Globals/ESPEasyWiFiEvent.h"
#include "../Globals/ESPEasy_time.h"

// void handle_mypage() {
//   if (!isLoggedIn()) { return; }   // проверка авторизации

//   TXBuffer.startStream();          // начать формирование HTML
//   sendHeadandTail_stdtemplate(_HEAD);

//   addFormHeader(F("My Custom Page"));
//   addHtml(F("<p>Hello from MyPage!</p>"));

//   sendHeadandTail_stdtemplate(_TAIL);
// }

// #endif // WEBSERVER_MY

void handle_mypage() {
  if (!isLoggedIn()) { return; }   // проверка авторизации

  TXBuffer.startStream();
  sendHeadandTail_stdtemplate(_HEAD);

  addFormHeader(F("My Custom Page"));

  // --- Приветствие ---
  addHtml(F("<p>Hello from MyPage!</p>"));

  // --- Управление реле ---
  addFormSubHeader(F("Relay Control"));
  addButton(F("/?cmd=GPIO,4,1&page=mypage"), F("Relay 1 ON"));
  addButton(F("/?cmd=GPIO,4,0&page=mypage"), F("Relay 1 OFF"));

  sendHeadandTail_stdtemplate(_TAIL);
}

#endif // WEBSERVER_MY
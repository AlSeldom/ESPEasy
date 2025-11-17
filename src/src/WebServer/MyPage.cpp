#include "../WebServer/MyPage.h"

#ifdef WEBSERVER_MY

#include "../WebServer/ESPEasy_WebServer.h"
#include "../WebServer/HTML_wrappers.h"
#include "../WebServer/Markup.h"
#include "../WebServer/Markup_Buttons.h"
#include "../WebServer/Markup_Forms.h"
#include "../Globals/ESPEasyWiFiEvent.h"
#include "../Globals/ESPEasy_time.h"

#include "../../ESPEasy-Globals.h"
#include "../Commands/ExecuteCommand.h"
#include "../Helpers/WebServer_commandHelper.h"   // обязательно подключить

void handle_mypage() {
  if (!isLoggedIn()) { return; }
 

  String cmd = webArg(F("cmd"));
  if (cmd.length() > 0) {
    handle_command_from_web(EventValueSource::Enum::VALUE_SOURCE_WEB_FRONTEND, cmd);
    printToWeb     = false;
    printToWebJSON = false;
  }

  addHtml(F("<form>"));
  html_table_class_normal();

  navMenuIndex = MENU_INDEX_TOOLS;
  TXBuffer.startStream();
  sendHeadandTail_stdtemplate(_HEAD);

  addFormHeader(F("Relay pages"));
  addHtml(F("<br><br>"));   // двойной перенос строки

  addFormSubHeader(F("Relay Control"));
  addHtml(F("<br><br>"));   // двойной перенос строки
  addButton(F("/mypage?cmd=GPIO,4,1"), F("Relay 1 ON"));
  addButton(F("/mypage?cmd=GPIO,4,0"), F("Relay 1 OFF"));

  addHtml(F("<br><br>"));   // двойной перенос строки
  // Если есть вывод команды — покажем его
  if (printWebString.length() > 0) {
    //addHtml(F("<p>Command Output:<br><pre>"));
    addHtml(F("<TR><TD colspan='2'>Command Output<BR><pre style='white-space:pre-wrap; word-wrap:break-word;'>"));
    addHtml(printWebString);
    addHtml(F("</pre>"));
    addHtml(F("</TD></TR>"));
  }

  sendHeadandTail_stdtemplate(_TAIL);
}


#endif // WEBSERVER_MY



//======================================================================
// void handle_mypage() {
//   if (!isLoggedIn()) { return; }   // проверка авторизации

//   TXBuffer.startStream();          // начать формирование HTML
//   sendHeadandTail_stdtemplate(_HEAD);

//   addFormHeader(F("My Custom Page"));
//   addHtml(F("<p>Hello from MyPage!</p>"));

//   sendHeadandTail_stdtemplate(_TAIL);
// } 

// #endif // WEBSERVER_MY
//=========================================================================
// void handle_mypage() {
//   if (!isLoggedIn()) { return; }   // проверка авторизации

//   TXBuffer.startStream();
//   sendHeadandTail_stdtemplate(_HEAD);

//   addFormHeader(F("My Custom Page"));

//   // --- Приветствие ---
//   addHtml(F("<p>Hello from MyPage!</p>"));

//   // --- Управление реле ---
//   addFormSubHeader(F("Relay Control"));
//   addButton(F("/?cmd=GPIO,4,1&page=mypage"), F("Relay 1 ON"));
//   addButton(F("/?cmd=GPIO,4,0&page=mypage"), F("Relay 1 OFF"));

//   sendHeadandTail_stdtemplate(_TAIL);
// }

// #endif // WEBSERVER_MY
// ======================================================================
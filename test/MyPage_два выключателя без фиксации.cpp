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

#include <Arduino.h>
#include "../Globals/MQTT.h"
#include "../ESPEasyCore/ESPEasyRules.h"
#include "../Commands/ExecuteCommand.h"
#include "../DataTypes/EventValueSource.h"
#include "../Globals/Settings.h"
#include "../Helpers/RulesHelper.h"   // форматирование значений
#include "../Globals/Cache.h"          // имена Value (не обязательно, но полезно)

// ========================Logic выключатель без фиксациии =======================================
#include "../Globals/Logging.h"

// глобальные переменные для sw1
bool lastPressed_sw1 = false;
bool relayState_sw1 = false;

// глобальные переменные для sw2
bool lastPressed_sw2 = false;
bool relayState_sw2 = false;

void updateRelayLogic(int uiTaskNr, const char* deviceName, int relayGpio,
                      bool &lastPressed, bool &relayState) {
  if (MQTTclient.connected()) {
    addLog(LOG_LEVEL_INFO, F("MQTT connected → skip local relay logic"));
    return;
  }

  EventStruct ev;
  ev.TaskIndex = uiTaskNr - 1;   // TaskNr → индекс

  String stateStr = formatUserVarNoCheck(&ev, 0);
  bool pressed = (stateStr == F("1"));

  // реагируем только на переход 0 → 1
  if (pressed && !lastPressed) {
    rulesProcessing(String(deviceName) + F("#State=1"));

    relayState = !relayState; // toggle

    String cmd = String("GPIO,") + relayGpio + (relayState ? ",1" : ",0");
    ExecuteCommand(0, EventValueSource::Enum::VALUE_SOURCE_SYSTEM,
                   cmd.c_str(), true, true, true, false);

    addLog(LOG_LEVEL_INFO,
           String(deviceName) + F(" → Toggle relay, new state=") + (relayState ? "ON" : "OFF"));
  }

  lastPressed = pressed;
}

// ========================end Logic===================================

void handle_mypage() {
  if (!isLoggedIn()) { return; }

  String cmd = webArg(F("cmd"));
  if (cmd.length() > 0) {
    handle_command_from_web(EventValueSource::Enum::VALUE_SOURCE_WEB_FRONTEND, cmd);
    printToWeb     = false;
    printToWebJSON = false;
  }

  navMenuIndex = MENU_INDEX_TOOLS;
  TXBuffer.startStream();
  sendHeadandTail_stdtemplate(_HEAD);

  // --- Заголовок страницы ---
  addFormHeader(F("Прямое управление периферией"));
  addHtml(F("<br><br>"));
  // ----Добавим выключатели-----------------------------------------------------------
  
  addFormSubHeader(F("Выключатели"));
  addHtml(F("<br>"));

  // Начало таблицы
  addHtml(F("<table style='width:98%; border-collapse:collapse; border:1px solid gray;'>"));

  // Заголовки
  addHtml(F("<tr style='background-color:#f0f0f0;'>"
          "<th style='width:50%; text-align:center; vertical-align:middle; border:1px solid gray;'>Выключатель</th>"
          "<th style='width:50%; text-align:center; vertical-align:middle; border:1px solid gray;'>Состояние</th>"
          "</tr>"));

  // --- Выключатель 1 ---
  addHtml(F("<tr>"
          "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Выключатель 1</td>"
          "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(13) ? 
        F("<span style='color:green;font-weight:bold;'>ON</span>") :
        F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));

  // --- Выключатель 2 ---
  addHtml(F("<tr>"
          "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Выключатель 2</td>"
          "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(2) ? 
        F("<span style='color:green;font-weight:bold;'>ON</span>") :
        F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));
  // Конец таблицы
  addHtml(F("</table><br><br>"));


  // ----Добавим выключатели-----------------------------------------------------------

  // --- Управление реле ---
  addFormSubHeader(F("Управление реле"));
  addHtml(F("<br>"));

  // Начало таблицы
  addHtml(F("<table style='width:98%; border-collapse:collapse; border:1px solid gray;'>"));

  // Заголовки
  addHtml(F("<tr style='background-color:#f0f0f0;'>"
            "<th style='width:33%; text-align:center; vertical-align:middle; border:1px solid gray;'>Реле</th>"
            "<th style='width:33%; text-align:center; vertical-align:middle; border:1px solid gray;'>Управление</th>"
            "<th style='width:33%; text-align:center; vertical-align:middle; border:1px solid gray;'>Состояние</th>"
            "</tr>"));

  // --- Реле 1 ---
  addHtml(F("<tr>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Relay 1</td>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addButton(F("/mypage?cmd=GPIO,4,1"), F("ON"));
  addButton(F("/mypage?cmd=GPIO,4,0"), F("OFF"));
  addHtml(F("</td><td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(4) ? F("<span style='color:green;font-weight:bold;'>ON</span>")
                        : F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));

  // --- Реле 2 ---
  addHtml(F("<tr>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Relay 2</td>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addButton(F("/mypage?cmd=GPIO,5,1"), F("ON"));
  addButton(F("/mypage?cmd=GPIO,5,0"), F("OFF"));
  addHtml(F("</td><td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(5) ? F("<span style='color:green;font-weight:bold;'>ON</span>")
                        : F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));

  // --- Реле 3 ---
  addHtml(F("<tr>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Relay 3</td>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addButton(F("/mypage?cmd=GPIO,12,1"), F("ON"));
  addButton(F("/mypage?cmd=GPIO,12,0"), F("OFF"));
  addHtml(F("</td><td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(12) ? F("<span style='color:green;font-weight:bold;'>ON</span>")
                          : F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));

  // --- Реле 4 ---
  addHtml(F("<tr>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>Relay 4</td>"
            "<td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addButton(F("/mypage?cmd=GPIO,14,1"), F("ON"));
  addButton(F("/mypage?cmd=GPIO,14,0"), F("OFF"));
  addHtml(F("</td><td style='text-align:center; vertical-align:middle; border:1px solid gray;'>"));
  addHtml(digitalRead(14) ? F("<span style='color:green;font-weight:bold;'>ON</span>")
                          : F("<span style='color:red;font-weight:bold;'>OFF</span>"));
  addHtml(F("</td></tr>"));

  // Конец таблицы
  addHtml(F("</table><br><br>"));

  sendHeadandTail_stdtemplate(_TAIL);
}

#endif // WEBSERVER_MY

// ======================================Кружки вместо ON OFF
// void handle_mypage() {
//   if (!isLoggedIn()) { return; }

//   String cmd = webArg(F("cmd"));
//   if (cmd.length() > 0) {
//     handle_command_from_web(EventValueSource::Enum::VALUE_SOURCE_WEB_FRONTEND, cmd);
//     printToWeb     = false;
//     printToWebJSON = false;
//   }

//   navMenuIndex = MENU_INDEX_TOOLS;
//   TXBuffer.startStream();
//   sendHeadandTail_stdtemplate(_HEAD);

//   // --- Заголовок страницы ---
//   addFormHeader(F("Relay pages"));
//   addHtml(F("<br><br>"));

//   // --- Управление реле ---
//   addFormSubHeader(F("Relay Control"));
//   addHtml(F("<br>"));

//   // Начало таблицы
//   addHtml(F("<table style='width:100%; border-collapse:collapse;'>"));
//   addHtml(F("<tr><th>Реле</th><th>Управление</th><th>Состояние</th></tr>"));

//   // --- Реле 1 ---
//   addHtml(F("<tr><td>Relay 1</td><td>"));
//   addButton(F("/mypage?cmd=GPIO,4,1"), F("ON"));
//   addButton(F("/mypage?cmd=GPIO,4,0"), F("OFF"));
//   addHtml(F("</td><td>"));
//   addHtml(digitalRead(4) 
//           ? F("<span style='color:green;font-size:20px;'>&#9679;</span>")   // зелёный кружок
//           : F("<span style='color:red;font-size:20px;'>&#9679;</span>"));   // красный кружок
//   addHtml(F("</td></tr>"));

//   // --- Реле 2 ---
//   addHtml(F("<tr><td>Relay 2</td><td>"));
//   addButton(F("/mypage?cmd=GPIO,5,1"), F("ON"));
//   addButton(F("/mypage?cmd=GPIO,5,0"), F("OFF"));
//   addHtml(F("</td><td>"));
//   addHtml(digitalRead(5) 
//           ? F("<span style='color:green;font-size:20px;'>&#9679;</span>")
//           : F("<span style='color:red;font-size:20px;'>&#9679;</span>"));
//   addHtml(F("</td></tr>"));

//   // --- Реле 3 ---
//   addHtml(F("<tr><td>Relay 3</td><td>"));
//   addButton(F("/mypage?cmd=GPIO,12,1"), F("ON"));
//   addButton(F("/mypage?cmd=GPIO,12,0"), F("OFF"));
//   addHtml(F("</td><td>"));
//   addHtml(digitalRead(12) 
//           ? F("<span style='color:green;font-size:20px;'>&#9679;</span>")
//           : F("<span style='color:red;font-size:20px;'>&#9679;</span>"));
//   addHtml(F("</td></tr>"));

//   // --- Реле 4 ---
//   addHtml(F("<tr><td>Relay 4</td><td>"));
//   addButton(F("/mypage?cmd=GPIO,13,1"), F("ON"));
//   addButton(F("/mypage?cmd=GPIO,13,0"), F("OFF"));
//   addHtml(F("</td><td>"));
//   addHtml(digitalRead(13) 
//           ? F("<span style='color:green;font-size:20px;'>&#9679;</span>")
//           : F("<span style='color:red;font-size:20px;'>&#9679;</span>"));
//   addHtml(F("</td></tr>"));

//   // Конец таблицы
//   addHtml(F("</table><br><br>"));

//   // --- Вывод результата команды ---
//   if (printWebString.length() > 0) {
//     addFormSubHeader(F("Command Output"));
//     addHtml(F("<pre style='width:98%; background:#f0f0f0; border:1px solid #ccc; "
//               "padding:5px; white-space:pre-wrap; word-wrap:break-word;'>"));
//     addHtml(printWebString);
//     addHtml(F("</pre>"));
//     free_string(printWebString);
//   }

//   sendHeadandTail_stdtemplate(_TAIL);
// }

// ======================================Кружки вместо ON OFF



// ========================Logic выключатель с фиксацией =======================================
// void updateRelayLogic(int uiTaskNr, const char* deviceName, int relayGpio) {
//   // Проверка MQTT
//   if (MQTTclient.connected()) {
//     addLog(LOG_LEVEL_INFO, F("MQTT connected → skip local relay logic"));
//     return; // если MQTT доступен, не выполняем локальную логику
//   }

//   EventStruct ev;
//   ev.TaskIndex = uiTaskNr - 1;   // UI Task Nr → внутренний индекс

//   // Получаем текущее значение Value1 (обычно "State")
//   String stateStr = formatUserVarNoCheck(&ev, 0);
//   bool state = (stateStr == F("1"));

//   if (state) {
//     rulesProcessing(String(deviceName) + F("#State=1"));

//     String cmd = String("GPIO,") + relayGpio + ",1";
//     ExecuteCommand(0, EventValueSource::Enum::VALUE_SOURCE_SYSTEM,
//                    cmd.c_str(), true, true, true, false);

//     addLog(LOG_LEVEL_INFO, String(deviceName) + F(" → Relay ON"));
//   } else {
//     rulesProcessing(String(deviceName) + F("#State=0"));

//     String cmd = String("GPIO,") + relayGpio + ",0";
//     ExecuteCommand(0, EventValueSource::Enum::VALUE_SOURCE_SYSTEM,
//                    cmd.c_str(), true, true, true, false);

//     addLog(LOG_LEVEL_INFO, String(deviceName) + F(" → Relay OFF"));
//   }
// }
// ========================end Logic===================================
#ifndef WEBSERVER_MYPAGE_H
#define WEBSERVER_MYPAGE_H
#pragma once
#include "../WebServer/common.h"

// Объявление твоей страницы
void handle_mypage();
void updateRelayLogic(int uiTaskNr, const char* deviceName, int relayGpio);
//Объявления кнопочных хелперов (если не подключаешь Markup_Buttons.h)
void addWideButtonPlusDescription(const __FlashStringHelper * url,
                                  const __FlashStringHelper * buttonText,
                                  const __FlashStringHelper * description);

void addWideButtonPlusDescription(const String& url,
                                  const String& buttonText,
                                  const String& description);

#endif // WEBSERVER_MYPAGE_H

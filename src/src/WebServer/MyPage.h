#ifndef WEBSERVER_MYPAGE_H
#define WEBSERVER_MYPAGE_H
#pragma once
#include "../WebServer/common.h"

extern bool lastPressed_sw1; // Чтобы они были видны в espeasy_loop.cpp
extern bool relayState_sw1;

extern bool lastPressed_sw2;
extern bool relayState_sw2;

// Объявление твоей страницы
void handle_mypage();
void updateRelayLogic(int uiTaskNr,
                      const char* deviceName,
                      int relayGpio,
                      bool &lastPressed,
                      bool &relayState);
//Объявления кнопочных хелперов (если не подключаешь Markup_Buttons.h)
void addWideButtonPlusDescription(const __FlashStringHelper * url,
                                  const __FlashStringHelper * buttonText,
                                  const __FlashStringHelper * description);

void addWideButtonPlusDescription(const String& url,
                                  const String& buttonText,
                                  const String& description);

#endif // WEBSERVER_MYPAGE_H

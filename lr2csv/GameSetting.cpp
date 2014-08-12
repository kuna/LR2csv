#include "Stdafx.h"
#include "GameSetting.h"

SceneSetting GameSetting::scene;
ScreenSetting GameSetting::screen;
KeySetting GameSetting::key;
PlaySetting GameSetting::play;

void GameSetting::SaveSetting() {
}

void GameSetting::LoadSetting() {
	TCHAR settingXMLPath[256];
	CSVFile::GetAbsolutePath(L"settings.xml", settingXMLPath);
	
	CMarkup xml;
	if (xml.Load( settingXMLPath )) {
		xml.FindElem();
		xml.IntoElem();
		while (xml.FindElem(L"scene")) {
			if (xml.GetAttrib(L"name").compare(L"select") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.select, xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"decide") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.decide, xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"play") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.play, xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"result") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.result, xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"keysetting") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.keysetting, xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"skinselect") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.skinselect, xml.GetData().c_str());
				xml.OutOfElem();
			}
		}

		while (xml.FindElem(L"setting")) {
			if (xml.GetAttrib(L"name").compare(L"screen")==0) {
				xml.IntoElem();
				xml.FindElem(L"width");
				screen.width = _wtoi(xml.GetData().c_str());
				xml.FindElem(L"height");
				screen.height = _wtoi(xml.GetData().c_str());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").compare(L"key")==0) {
				// TODO: save key
			} else if (xml.GetAttrib(L"name").compare(L"play")==0) {
				xml.IntoElem();
				xml.FindElem(L"speed");
				play.speed = _wtoi(xml.GetData().c_str());
				xml.FindElem(L"shutter");
				play.shutter = _wtoi(xml.GetData().c_str());
				xml.OutOfElem();
			}
		}
	}
}
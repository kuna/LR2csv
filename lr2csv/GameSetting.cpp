#include "GameSetting.h"
#include "CSVFile.h"
#include "Markup.h"	// for XML parsing


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
			if (xml.GetAttrib(L"name").CompareNoCase(L"select") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.select, xml.GetData());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").CompareNoCase(L"decide") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.decide, xml.GetData());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").CompareNoCase(L"play") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.play, xml.GetData());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").CompareNoCase(L"result") == 0) {
				xml.IntoElem();
				xml.FindElem();
				wcscpy(scene.result, xml.GetData());
				xml.OutOfElem();
			}
		}

		while (xml.FindElem(L"setting")) {
			if (xml.GetAttrib(L"name").CompareNoCase(L"screen")==0) {
				xml.IntoElem();
				xml.FindElem(L"width");
				screen.width = _wtoi(xml.GetData());
				xml.FindElem(L"height");
				screen.height = _wtoi(xml.GetData());
				xml.OutOfElem();
			} else if (xml.GetAttrib(L"name").CompareNoCase(L"key")==0) {
				// TODO: save key
			} else if (xml.GetAttrib(L"name").CompareNoCase(L"play")==0) {
				xml.IntoElem();
				xml.FindElem(L"speed");
				play.speed = _wtoi(xml.GetData());
				xml.FindElem(L"shutter");
				play.shutter = _wtoi(xml.GetData());
				xml.OutOfElem();
			}
		}
	}
}
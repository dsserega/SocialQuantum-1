#include "Common.h"

#include "GraphicsSettings.h"
#include "tinyxml.h"

using namespace std;

const StrVec kResolutionStr = { "640*480", "1024*768", "1920*1080" };
const StrVec kAntialiasingStr = { "MULTISAMPLE_NONE", "MULTISAMPLE_2", "MULTISAMPLE_4" };
const StrVec kQualityStr = { "Low", "Medium", "Hight", "Custom" };

const char* boolToStr(bool v)
{
	return v ? "true" : "false";
}

bool boolFromStr(const char* v)
{
	return strcmp(v, "true") == 0;
}

const string GraphicsSettings::GRAPHICS_QUALITY = "graphics_quality";
const string GraphicsSettings::RESOLUTION = "resolution";
const string GraphicsSettings::ANTIALIASING = "antialiasing";
const string GraphicsSettings::FULL_SCREEN_MODE = "full_screen_mode";
const string GraphicsSettings::DYNAMIC_LIGHT = "dynamic_lighting";
const string GraphicsSettings::VIEW_DISTANCE = "view_distance";
const string GraphicsSettings::TEXTURE_QUALITY = "texture_quality";
const string GraphicsSettings::SHADOWS_QUALITY = "shadows_quality";

GraphicsSettings::GraphicsSettings()
{
	handlers[ANTIALIASING] = [this](const char* value) { settings.antialiasing = EnumFromString<Antialiasing>(kAntialiasingStr, value); };
	handlers[RESOLUTION] = [this](const char* value) { settings.resolution = EnumFromString<Resolution>(kResolutionStr, value); };
	handlers[FULL_SCREEN_MODE] = [this](const char* value) { settings.fullScreenMode = boolFromStr(value); };
	handlers[DYNAMIC_LIGHT] = [this](const char* value) { settings.dynamicLight = boolFromStr(value); graphics = GQ_CUSTOM; };
	handlers[VIEW_DISTANCE] = [this](const char* value) { settings.viewDistance = atoi(value); };
	handlers[TEXTURE_QUALITY] = [this](const char* value) { settings.textures = EnumFromString<TextureQuality>(kQualityStr, value); graphics = GQ_CUSTOM; };
	handlers[SHADOWS_QUALITY] = [this](const char* value) { settings.shadow = EnumFromString<ShadowsQuality>(kQualityStr, value);  graphics = GQ_CUSTOM; };
	handlers[GRAPHICS_QUALITY] = [this](const char* value) { graphics = EnumFromString<GraphicsQuality>(kQualityStr, value); OnSetQuality(); };

	presets[GQ_LOW] = Settings::LOW();
	presets[GQ_MEDIUM] = Settings::MEDIUM();
	presets[GQ_HIGHT] = Settings::HIGHT();
}

void GraphicsSettings::OnSetQuality()
{
	auto it = presets.find(graphics);

	if (it != presets.end())
	{
		auto preset = it->second;

		settings.dynamicLight = preset.dynamicLight;
		settings.textures = preset.textures;
		settings.shadow = preset.shadow;
	}
}

StrMap GraphicsSettings::ToMap() const
{
	StrMap map;

	map[GRAPHICS_QUALITY] = kQualityStr[graphics];
	map[RESOLUTION] = kResolutionStr[settings.resolution];
	map[ANTIALIASING] = kAntialiasingStr[settings.antialiasing];
	map[FULL_SCREEN_MODE] = boolToStr(settings.fullScreenMode);
	map[DYNAMIC_LIGHT] = boolToStr(settings.dynamicLight);
	map[VIEW_DISTANCE] = std::to_string(settings.viewDistance);
	map[TEXTURE_QUALITY] = kQualityStr[settings.textures];
	map[SHADOWS_QUALITY] = kQualityStr[settings.shadow];

	return map;
}

string GraphicsSettings::ToXML() const
{
	stringstream ss;

	auto map = ToMap();

	ss << "<?xml version=\"1.0\" standalone=\"no\" ?>" << endl
		<< "<settings>" << endl;

	for (auto it : map)
		ss << "<" << it.first << ">" << it.second << "</" << it.first << ">" << endl;

	ss << "</settings>" << endl;

	return ss.str();
}

bool GraphicsSettings::FromXML(const char* xml)
{
	TiXmlDocument doc;

	doc.Parse(xml);

	auto el = doc.FirstChildElement();

	if (el == nullptr)
		return false;

	StrVec params = { RESOLUTION, ANTIALIASING, FULL_SCREEN_MODE, DYNAMIC_LIGHT, VIEW_DISTANCE, ANTIALIASING, SHADOWS_QUALITY, GRAPHICS_QUALITY };

	for (auto prm : params)
	{
		auto child = el->FirstChildElement(prm.c_str());

		if (child == nullptr)
			continue;

		SetValue(prm.c_str(), child->GetText());
	}

	return true;
}

void GraphicsSettings::SetValue(const char* key, const char* value)
{
	auto it = handlers.find(key);
	
	if (it == handlers.end())
		throw std::runtime_error(string("Uknown parameter ") + key);

	it->second(value);
}

std::string GraphicsSettings::GetValue(const char* key)
{
	auto map = ToMap();
	auto it = map.find(key);

	if (it == map.end())
		throw std::runtime_error(string("Uknown parameter ") + key);

	return it->second;;
}

void GraphicsSettings::Save(const char* file, Format format)
{
	fstream fs;

	int mode = format == Bin ? file, ios::out | ios::trunc || ios::binary : ios::out | ios::trunc;

	fs.open(file, mode);

	if(!fs.is_open())
		throw std::runtime_error(string("Can't open file ") + file);

	fs << ToXML();
	fs.close();
}

bool GraphicsSettings::Load(const char * file, Format format)
{
	fstream fs;

	int mode = format == Bin ? file, ios::in || ios::binary : ios::in;

	fs.open(file, mode);

	if (!fs.is_open())
		return false;

	string line;
	string xml;

	while (getline(fs, line))
		xml += line;

	fs.close();

	return FromXML(xml.c_str());
}

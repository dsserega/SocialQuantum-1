#pragma once

#include "Settings.h"

enum Format { Bin, XML };

class GraphicsSettings
{
public:
	GraphicsSettings();

	std::string ToXML() const;

	void SetValue(const char* key, const char* value);
	std::string GetValue(const char* key);

	StrMap ToMap() const;
	void Save(const char* file, Format format);
private:
	void OnSetQuality();
private:
	Settings settings = Settings::MEDIUM();
	GraphicsQuality graphics = GraphicsQuality::GQ_MEDIUM;

	typedef std::function<void(const char*)> Handler;
	std::map<std::string, Handler> handlers;
	std::map<GraphicsQuality, Settings> presets;
private:
	static const std::string GRAPHICS_QUALITY;
	static const std::string RESOLUTION;
	static const std::string ANTIALIASING;
	static const std::string FULL_SCREEN_MODE;
	static const std::string DYNAMIC_LIGHT;
	static const std::string VIEW_DISTANCE;
	static const std::string TEXTURE_QUALITY;
	static const std::string SHADOWS_QUALITY;
};
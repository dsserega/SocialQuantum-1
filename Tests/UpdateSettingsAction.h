#pragma once

class GraphicsSettings;

class UpdateSettingsAction
{
public:
	UpdateSettingsAction(GraphicsSettings* gs, const char* wht, const char* val);

	void Do();
	void Undo();
private:
	std::string current;
	std::string next;
	std::string what;

	GraphicsSettings* settings;
};
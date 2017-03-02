#include "Common.h"

#include "UpdateSettingsAction.h"
#include "GraphicsSettings.h"

using namespace std;

UpdateSettingsAction::UpdateSettingsAction(GraphicsSettings* gs, const char* wht, const char* val) : settings(gs), what(wht), next(val)
{
	
}

void UpdateSettingsAction::Do()
{
	current = settings->GetValue(what.c_str());

	settings->SetValue(what.c_str(), next.c_str());
}

void UpdateSettingsAction::Undo()
{
	settings->SetValue(what.c_str(), current.c_str());
}

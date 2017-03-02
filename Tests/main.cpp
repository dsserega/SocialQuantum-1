#include "Common.h"

#include "GraphicsSettings.h"
#include "UpdateSettingsAction.h"

using namespace std;

typedef function<bool(const StrVec&)> InputHandler;
typedef vector<UpdateSettingsAction> Actions;

StrVec kFormatsStr = { "Bin", "XML" };

StrVec split(const string& str, char delimiter) {
	StrVec internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) 
		internal.push_back(tok);

	return internal;
}

#define CHECK_PARMS(VEC, SZ)\
	if(VEC.size() < SZ)\
		throw std::runtime_error("Invalid parameters")

class InputListener
{
public:
	InputListener(GraphicsSettings* gs) : settings(gs), it(actions.end())
	{
		handlers["change"] = [this](const StrVec& prm)
		{
			CHECK_PARMS(prm, 3);

			auto command = UpdateSettingsAction(settings, prm[1].c_str(), prm[2].c_str());

			actions.erase(it, actions.end());
			actions.push_back(command);

			it = actions.end();
			
			command.Do();

			return true;
		};

		handlers["get"] = [this](const StrVec& prm) 
		{
			CHECK_PARMS(prm, 2);

			cout << settings->GetValue(prm[1].c_str()) << endl; 
			return true; 
		};

		handlers["print"] = [this](const StrVec& prm) 
		{
			cout << settings->ToXML();
			return true; 
		};

		handlers["format"] = [this](const StrVec& prm) 
		{ 
			CHECK_PARMS(prm, 2);

			format = EnumFromString<Format>(kFormatsStr, prm[1].c_str());
			return true; 
		};

		handlers["save"] = [this](const StrVec& prm) 
		{
			CHECK_PARMS(prm, 2);

			settings->Save(prm[1].c_str(), format); 
			return true;  
		};

		handlers["undo"] = [this](const StrVec& prm)
		{
			if (actions.empty())
				throw std::runtime_error("Nothig to Undo");

			if(it == actions.end())
				it = actions.end() - 1;

			it->Undo();
			--it;

			return true;
		};

		handlers["redo"];
		handlers["exit"] = [this](const StrVec& prm) { return false; };
	}

	bool HandleInut(const std::string& line)
	{
		try
		{
			auto params = split(line, ' ');

			CHECK_PARMS(params, 1);

			auto it = handlers.find(params[0]);

			if(it == handlers.end())
				throw std::runtime_error("Unknown commant");

			return it->second(params);
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}

		return true;
	}
private:
	Format format = Format::XML;
	GraphicsSettings* settings;
	
	map<string, InputHandler> handlers;
	Actions actions;
	Actions::iterator it;
};

int main()
{
	auto settings = new GraphicsSettings();
	auto listener = InputListener(settings);

	string input;

	do
	{
		getline(cin, input);
	} while (listener.HandleInut(input));

	delete settings;

	return 1;
}

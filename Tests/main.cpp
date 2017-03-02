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
	InputListener(GraphicsSettings* gs) : settings(gs)
	{
		handlers["change"] = [this](const StrVec& prm)
		{
			CHECK_PARMS(prm, 3);

			auto command = UpdateSettingsAction(settings, prm[1].c_str(), prm[2].c_str());
			auto it = actions.begin() + index;

			actions.erase(it, actions.end());
					
			command.Do();

			++index;

			actions.push_back(command);

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
			auto map = settings->ToMap();
			
			for (auto it : map)
				cout << it.first << "=" << it.second << endl;

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
			if (actions.empty() || index <= 0)
				throw std::runtime_error("Nothig to Undo");

			auto action = actions[--index];
			
			action.Undo();

			return true;
		};

		handlers["redo"] = [this](const StrVec& prm)
		{
			if (index >= actions.size())
				throw std::runtime_error("Nothig to Redo");

			auto action = actions[index++];

			action.Do();

			return true;
		};

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
				throw std::runtime_error("Unknown command");

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
	int index = 0;
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

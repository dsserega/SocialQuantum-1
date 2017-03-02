#pragma once

enum Resolution
{
	R_640x480,
	R_1024x768,
	R_1920x1080
};

enum Antialiasing
{
	MULTISAMPLE_NONE,
	MULTISAMPLE_2,
	MULTISAMPLE_4
};

enum TextureQuality
{
	TQ_LOW, TQ_MEDIUM, TQ_HIGHT
};

enum ShadowsQuality
{
	SQ_LOW, SQ_MEDIUM, SQ_HIGHT
};

enum GraphicsQuality
{
	GQ_LOW, GQ_MEDIUM, GQ_HIGHT, GQ_CUSTOM
};

struct Settings
{
	Resolution resolution = R_640x480;
	Antialiasing antialiasing = MULTISAMPLE_NONE;

	bool fullScreenMode = true;
	bool dynamicLight = false;
	int viewDistance = 500;

	TextureQuality textures = TQ_MEDIUM;
	ShadowsQuality shadow = SQ_MEDIUM;

	Settings()
	{

	}

	Settings(TextureQuality tq, ShadowsQuality sq, bool dl) : textures(tq), shadow(sq), dynamicLight(dl)
	{

	}

	static Settings LOW()
	{
		auto settings = Settings(TQ_LOW, SQ_LOW, false);

		return settings;
	}

	static Settings MEDIUM()
	{
		auto settings = Settings(TQ_MEDIUM, SQ_MEDIUM, false);

		return settings;
	}

	static Settings HIGHT()
	{
		auto settings = Settings(TQ_HIGHT, SQ_HIGHT, true);

		return settings;
	}
};

#pragma once
public ref class Axes
{
	Axes() {
		Init();
	}
	void Init() {
		Factor = 1;
		BufferIndex = 5;
		Active = true;
		Delta = 0.1;
		Last = 0;
	}
	int Factor = 1;
	int BufferIndex = 5;
	bool Active = true;
	float Delta = 0.1;
	float Last = 0;
};


#pragma once
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamliy = -1;
	bool isvalid() {
		return graphicsFamily >= 0 && presentFamliy >= 0;
	}
};
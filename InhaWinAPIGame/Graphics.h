#pragma once

#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include "Mat3.h"

class Graphics
{
public:

private:
	Mat3<float> transform = Mat3<float>::Identity();
};

#pragma once

#include "framework.h"

class GDIPlusManager
{
public:
	// Construct and init when GDIPlusManager is no exist
	GDIPlusManager();
	// Destruct GDIPlusmanager when it's last existing
	~GDIPlusManager();
private:
	// GDI+ API token
	static ULONG_PTR token;
	// Reference count to track
	static int refCount;
};
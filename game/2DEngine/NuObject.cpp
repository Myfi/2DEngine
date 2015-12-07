#include "NuObject.h"

NuObject::NuObject(HWND parent, LPCSTR title, int x, int y)
{
	//name = title;
	name = title;
	reference = CreateWindow(
		TEXT("BUTTON"),                 /* Class Name */
		title,							/* Title */
		WS_VISIBLE | WS_CHILD,          /* Style */
		x, y,							/* Position */
		80, 20,							/* Size */
		parent,							/* Parent */
		(HMENU)ID_DATA,                 /* Menu type */
		NULL,                           /* Instance */
		0);
}

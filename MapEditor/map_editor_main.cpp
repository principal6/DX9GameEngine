#include "DX9MapEditor.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace DX9ENGINE;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	static DX9MapEditor myMapEditor;

	if (DX_SUCCEEDED(myMapEditor.Create(800, 600)))
	{
		myMapEditor.Run();
	}	

	return 0;
}
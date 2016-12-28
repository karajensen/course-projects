#include "ShroomBot.h"

ShroomBot::~ShroomBot()
{
	if (m_pBot)
		Release();
}

void ShroomBot::Create(char const* dllname)
{
	// Load requested dll - m_hModule NULL on failure
	m_hModule = LoadLibrary(dllname);
	if (m_hModule)
	{
		// Get address of CreateInstance function from library - store in function pointer
		FPCREATE fpBotFactory = 0;
		fpBotFactory = (FPCREATE)GetProcAddress( m_hModule, TEXT("CreateInstance") );

		// If successful in getting access to CreateInstance, then create bot in dll memory space
		if (fpBotFactory)
			fpBotFactory(m_hModule,(void **)&m_pBot);
	}

}

void ShroomBot::Release()
{
	if (m_hModule)
	{
		if (m_pBot)
		{
			// Get address of DeleteInstance function from library - store in function pointer
			FPDELETE fpGarbage = 0;
			fpGarbage = (FPDELETE)GetProcAddress( m_hModule, TEXT("DeleteInstance") );

			// delete bot in dll memory space
			fpGarbage( (void **)&m_pBot );
		}
	}
}

LPBOT ShroomBot::Get()
{
	return m_pBot;
}
#include "ShroomLOG.h"


ShroomLOG::ShroomLOG()
: m_strError(), m_strGame(), m_strSave(), m_strStat()
{
	// open an error file
	m_strError.open("..\\log\\error.txt");
}

ShroomLOG::~ShroomLOG()
{
	// make sure every stream is closed
	CLOSE_LOG(ERR);
	CLOSE_LOG(GAME);
	CLOSE_LOG(SAVE);
	CLOSE_LOG(STAT);
}

ShroomLOG::stream_type& ShroomLOG::get(log_type eType)
{
	switch (eType)
	{
		case GAME:
			return m_strGame;
		case SAVE:
			return m_strSave;
		case STAT:
			return m_strStat;
		case ERR:
		default:
			return m_strError;
	};
}



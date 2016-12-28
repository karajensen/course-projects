#ifndef SHROOMLOG_H
#define SHROOMLOG_H

#include <fstream>
#include <iostream>
#include <iomanip>

#include "Singleton.h"

typedef enum log_type {
	_NULL = 0,
	ERR,
	GAME,
	SAVE,
	STAT
} LOG_TYPE;



class ShroomLOG : public Singleton<ShroomLOG>
{
	public:
		typedef std::fstream	stream_type;

		stream_type& get(log_type eType);

		~ShroomLOG();



	private:
		stream_type		m_strError;
		stream_type		m_strGame;
		stream_type		m_strSave;
		stream_type		m_strStat;

		ShroomLOG();


		friend Singleton<ShroomLOG>;

};



std::ostream& LOG(log_type);
void OPEN_LOG(log_type, char const*);
void CLOSE_LOG(log_type);
void FLUSH_LOG(log_type);




inline std::ostream& LOG(log_type eType)
{
	std::fstream &fs = (ShroomLOG::GetInstance()).get(eType);
	if (fs.is_open())
		return fs;
	else
		return std::cerr;
}

inline void OPEN_LOG(log_type eType, char const* szFileName)
{
	((ShroomLOG::GetInstance()).get(eType)).open(szFileName,std::ios_base::out);
}

inline void CLOSE_LOG(log_type eType)
{
	std::fstream &fs = (ShroomLOG::GetInstance()).get(eType);
	if (fs.is_open())
	{
		fs.flush();
		fs.close();
	}
}

inline void FLUSH_LOG(log_type eType)
{
	std::fstream &fs = (ShroomLOG::GetInstance()).get(eType);
	if (fs.is_open())
		fs.flush();
}




#endif
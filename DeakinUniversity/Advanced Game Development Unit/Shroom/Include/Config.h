// * ************************************************************************ //
// * * Config Header                                                      * * //
// * *   : defines custom types and functions for shroom config interface * * //
// * *                                                                    * * //
// * * Disclaimer: This document represents no significant intellectual   * * //
// * * property and my be used freely. The author(s) takes no             * * //
// * * responsibility for any loss or damage arising from the use of      * * //
// * * the computer code contained herein. Where this document is         * * //
// * * submitted as part of an assessment task this header must remain    * * //
// * * intact and the student must make clear indication which parts      * * //
// * * have been added by themselves.                                     * * //
// * *                                                                    * * //
// * ********************************************************************** * //
// * ********************************************************************** * //
// * * Config.h                                                           * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 31/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //

#ifndef CONFIG_H
#define CONFIG_H

#ifndef SAFE_DELETE_DEF
	#define SAFE_DELETE_DEF
	template <typename T> VOID SAFE_DELETE(T* &p) { if (p) { delete p; p = NULL; } }
#endif

#include <stdexcept>
#include <fstream>
#include <iomanip>

typedef struct _config_params
{
	SHORT world_rows,
		  world_cols,
		  shroom_types,
		  shroom_num;

	SHORT *shroom_value,
		  *shroom_count;

	SHORT shroom_magic_value,
		  hunter_health,
		  hunter_cost_step,
		  hunter_cost_turn,
		  hunter_cost_look;

	explicit _config_params(unsigned int const & nArrSize = 0)
		: shroom_value(NULL), shroom_count(NULL)
	{
		if (nArrSize > 0)
			allocate_mem(nArrSize);
	}
	~_config_params()
	{
		deallocate_mem();
	}
	_config_params(_config_params const& cp)
	{
		if (&cp == this)
			return;

		if (cp.shroom_types > 0)
		{
			deallocate_mem();
			allocate_mem(cp.shroom_types);
			copy_mem(cp);
		}
	}

	_config_params& operator=(_config_params const& cp)
	{
		if (&cp != this)
		{
			deallocate_mem();
			allocate_mem(cp.shroom_types);
			copy_mem(cp);
		}
		return *this;
	}

	void resize(unsigned int const& nArrSize)
	{
		deallocate_mem();
		if (nArrSize > 0)
			allocate_mem(nArrSize);
	}

	private:
		void deallocate_mem()
		{
			SAFE_DELETE(shroom_value);
			SAFE_DELETE(shroom_count);
		}

		void allocate_mem(unsigned int nArrSize)
		{
			shroom_value = new SHORT[nArrSize];
			shroom_count = new SHORT[nArrSize];
			
			if (!shroom_value || !shroom_count)
			{
				SAFE_DELETE(shroom_value);
				SAFE_DELETE(shroom_count);
			}
		}

		void copy_mem(_config_params const& cp)
		{
			if ( (shroom_types != cp.shroom_types) || shroom_count == 0 )
			{
				throw std::runtime_error("Cannot copy CONFIGPARAMS: incompatible sizes");
			}
			for (SHORT i = 0; i < shroom_types; ++i)
			{
				shroom_count[i] = cp.shroom_count[i];
				shroom_value[i] = cp.shroom_value[i];
			}
		}


		friend std::ostream& operator<<(std::ostream& os, _config_params & rcp)
		{
			os << std::setw(20) << std::left << "SHROOM_TYPES" << ": " << rcp.shroom_types << "\n";

			os << std::setw(20) << std::left << "SHROOM_VALUE" << ":";
			for (WORD i = 0; i < rcp.shroom_types; ++i)
				os << " " << rcp.shroom_value[i];
			os << "\n";

			os << std::setw(20) << std::left << "SHROOM_COUNT" << ":";
			for (WORD i = 0; i < rcp.shroom_types; ++i)
				os << " " << rcp.shroom_count[i];
			os << "\n";

			os << std::setw(20) << std::left << "SHROOM_NUM"			<< ": " << rcp.shroom_num			<< "\n";
			os << std::setw(20) << std::left << "SHROOM_MAGIC_VALUE"	<< ": " << rcp.shroom_magic_value	<< "\n";
			os << std::setw(20) << std::left << "HUNTER_HEALTH"			<< ": " << rcp.hunter_health			<< "\n";
			os << std::setw(20) << std::left << "HUNTER_COST_STEP"		<< ": " << rcp.hunter_cost_step		<< "\n";
			os << std::setw(20) << std::left << "HUNTER_COST_TURN"		<< ": " << rcp.hunter_cost_turn		<< "\n";
			os << std::setw(20) << std::left << "HUNTER_COST_LOOK"		<< ": " << rcp.hunter_cost_look		<< "\n";
			os << std::setw(20) << std::left << "WORLD_ROWS"			<< ": " << rcp.world_rows			<< "\n";
			os << std::setw(20) << std::left << "WORLD_COLS"			<< ": " << rcp.world_cols			<< "\n";

			return os;
		}
} CONFIG_PARAMS;


#endif
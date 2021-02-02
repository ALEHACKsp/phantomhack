#include <Windows.h>
#include "d3d.h"
#include "MemMgr.h"
#include "offsets.h"
#include "xstring.h"
#include "globals.h"

D3DVIEWPORT9 vpt;

LPDIRECT3DTEXTURE9 txGen = NULL;
LPDIRECT3DTEXTURE9 txInv = NULL;
LPDIRECT3DTEXTURE9 txHead = NULL;
LPDIRECT3DTEXTURE9 txHeadInv = NULL;


D3DLOCKED_RECT d3dlr;
D3DLOCKED_RECT d3dlr1;
LPD3DXFONT pFont = NULL;

static bool gotMethods = false;
LPDIRECT3DTEXTURE9 cPink = 0;
static DWORD whbClient = 0;
static DWORD whLocalBase = 0;

extern pDrawIndexedPrimitive oDrawIndexedPrimitive;

inline bool CheckVertices(UINT NumVertices, UINT primCount, UINT startIndex, int team, bool* isHead)
{
	if (g_MapName == "de_dust2" || g_MapName == "de_shortdust")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1761 && primCount == 2681 && startIndex == 0) ||
				(NumVertices == 1677 && primCount == 2713 && startIndex == 0))
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2118 && primCount == 3354 && startIndex == 0) ||
				(NumVertices == 3763 && primCount == 5532 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 3692 && primCount == 5699 && startIndex == 0) ||
				(NumVertices == 2663 && primCount == 3856 && startIndex == 0) ||
				(NumVertices == 3225 && primCount == 4751 && startIndex == 0) ||
				(NumVertices == 3742 && primCount == 5672 && startIndex == 0))
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 3932 && primCount == 5551 && startIndex == 0) ||
				(NumVertices == 4536 && primCount == 5963 && startIndex == 0) ||
				(NumVertices == 984 && primCount == 1466 && startIndex == 0) ||
				(NumVertices == 1006 && primCount == 1560 && startIndex == 0) ||
				(NumVertices == 3854 && primCount == 6693 && startIndex == 0) ||
				(NumVertices == 3845 && primCount == 5545 && startIndex == 0) ||
				(NumVertices == 1718 && primCount == 1914 && startIndex == 0) ||
				(NumVertices == 5008 && primCount == 6772 && startIndex == 0) ||
				(NumVertices == 5781 && primCount == 8207 && startIndex == 0))
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_austria" || g_MapName == "ar_baggage" || g_MapName == "de_overpass")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2113 && primCount == 3391 && startIndex == 0) ||
				(NumVertices == 2157 && primCount == 3461 && startIndex == 0) ||
				(NumVertices == 2151 && primCount == 3459 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1410 && primCount == 2352 && startIndex == 0) ||
				(NumVertices == 2239 && primCount == 3700 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "cs_agency")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2778 && primCount == 4244 && startIndex == 0) ||
				(NumVertices == 1459 && primCount == 2566 && startIndex == 0) ||
				(NumVertices == 993 && primCount == 1722 && startIndex == 0) ||
				(NumVertices == 2957 && primCount == 4234 && startIndex == 0) ||
				(NumVertices == 2223 && primCount == 3672 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 4327 && primCount == 6905 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 1646 && primCount == 2714 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 924 && primCount == 1632 && startIndex == 0) ||
				(NumVertices == 1197 && primCount == 2256 && startIndex == 0) ||
				(NumVertices == 927 && primCount == 1626 && startIndex == 0) ||
				(NumVertices == 266 && primCount == 348 && startIndex == 0) ||
				(NumVertices == 1053 && primCount == 1968 && startIndex == 0) ||
				(NumVertices == 646 && primCount == 1248 && startIndex == 0) ||
				(NumVertices == 200 && primCount == 276 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2056 && primCount == 3436 && startIndex == 0) ||
				(NumVertices == 3368 && primCount == 5329 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "ar_monastery")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2113 && primCount == 3391 && startIndex == 0) ||
				(NumVertices == 2157 && primCount == 3461 && startIndex == 0) ||
				(NumVertices == 2151 && primCount == 3459 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1410 && primCount == 2352 && startIndex == 0) ||
				(NumVertices == 2239 && primCount == 3700 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1214 && primCount == 2292 && startIndex == 0) ||
				(NumVertices == 1215 && primCount == 2286 && startIndex == 0) ||
				(NumVertices == 1624 && primCount == 2936 && startIndex == 0) ||
				(NumVertices == 1291 && primCount == 2437 && startIndex == 0) ||
				(NumVertices == 1345 && primCount == 2488 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1525 && primCount == 2525 && startIndex == 0) ||
				(NumVertices == 4159 && primCount == 6494 && startIndex == 0) ||
				(NumVertices == 1273 && primCount == 2113 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 360 && primCount == 554 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_train")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2539 && primCount == 3527 && startIndex == 0) ||
				(NumVertices == 4084 && primCount == 4697 && startIndex == 0) ||
				(NumVertices == 2963 && primCount == 3792 && startIndex == 0) ||
				(NumVertices == 3137 && primCount == 3948 && startIndex == 0) ||
				(NumVertices == 3245 && primCount == 4158 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2924 && primCount == 3540 && startIndex == 0) ||
				(NumVertices == 4533 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4422 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4666 && primCount == 6482 && startIndex == 0) ||
				(NumVertices == 4510 && primCount == 6422 && startIndex == 0) ||
				(NumVertices == 4320 && primCount == 6226 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1214 && primCount == 2292 && startIndex == 0) ||
				(NumVertices == 1215 && primCount == 2286 && startIndex == 0) ||
				(NumVertices == 1624 && primCount == 2936 && startIndex == 0) ||
				(NumVertices == 1291 && primCount == 2437 && startIndex == 0) ||
				(NumVertices == 1345 && primCount == 2488 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1525 && primCount == 2525 && startIndex == 0) ||
				(NumVertices == 4159 && primCount == 6494 && startIndex == 0) ||
				(NumVertices == 1273 && primCount == 2113 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 360 && primCount == 554 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_safehouse" || g_MapName == "ar_dizzy" || g_MapName == "de_bank" || g_MapName == "gd_rialto")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1917 && primCount == 3004 && startIndex == 0) ||
				(NumVertices == 1883 && primCount == 2986 && startIndex == 0) ||
				(NumVertices == 1835 && primCount == 2898 && startIndex == 0) ||
				(NumVertices == 1834 && primCount == 2898 && startIndex == 0) ||
				(NumVertices == 1980 && primCount == 3074 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 3650 && primCount == 4030 && startIndex == 0) ||
				(NumVertices == 4697 && primCount == 6093 && startIndex == 0) ||
				(NumVertices == 5109 && primCount == 6606 && startIndex == 0) ||
				(NumVertices == 5038 && primCount == 6402 && startIndex == 0) ||
				(NumVertices == 5134 && primCount == 6641 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 5799 && primCount == 7310 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 924 && primCount == 1632 && startIndex == 0) ||
				(NumVertices == 1197 && primCount == 2256 && startIndex == 0) ||
				(NumVertices == 927 && primCount == 1626 && startIndex == 0) ||
				(NumVertices == 266 && primCount == 348 && startIndex == 0) ||
				(NumVertices == 1053 && primCount == 1968 && startIndex == 0) ||
				(NumVertices == 646 && primCount == 1248 && startIndex == 0) ||
				(NumVertices == 200 && primCount == 276 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2056 && primCount == 3436 && startIndex == 0) ||
				(NumVertices == 3368 && primCount == 5329 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_cache" || g_MapName == "de_cbble")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1645 && primCount == 2568 && startIndex == 0) ||
				(NumVertices == 1601 && primCount == 2502 && startIndex == 0) ||
				(NumVertices == 1611 && primCount == 2522 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1258 && primCount == 1950 && startIndex == 0) ||
				(NumVertices == 3630 && primCount == 5404 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_shipped")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1967 && primCount == 2720 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2482 && primCount == 3414 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 5299 && primCount == 7167 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1380 && primCount == 2493 && startIndex == 0) ||
				(NumVertices == 1314 && primCount == 2367 && startIndex == 0) ||
				(NumVertices == 1996 && primCount == 2906 && startIndex == 0) ||
				(NumVertices == 1183 && primCount == 2045 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 934 && primCount == 1525 && startIndex == 0) ||
				(NumVertices == 7236 && primCount == 8208 && startIndex == 0) ||
				(NumVertices == 991 && primCount == 1525 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_inferno")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1967 && primCount == 2720 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2482 && primCount == 3414 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 5299 && primCount == 7167 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1106 && primCount == 2018 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2536 && primCount == 4196 && startIndex == 0) ||
				(NumVertices == 1914 && primCount == 3210 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "cs_assault")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2778 && primCount == 4244 && startIndex == 0) ||
				(NumVertices == 1459 && primCount == 2566 && startIndex == 0) ||
				(NumVertices == 993 && primCount == 1722 && startIndex == 0) ||
				(NumVertices == 2957 && primCount == 4234 && startIndex == 0) ||
				(NumVertices == 2223 && primCount == 3672 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 4327 && primCount == 6905 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 1646 && primCount == 2714 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_mirage")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1967 && primCount == 2720 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 5299 && primCount == 7167 && startIndex == 0) ||
				(NumVertices == 2482 && primCount == 3414 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 3692 && primCount == 5699 && startIndex == 0) ||
				(NumVertices == 2663 && primCount == 3856 && startIndex == 0) ||
				(NumVertices == 3225 && primCount == 4751 && startIndex == 0) ||
				(NumVertices == 3742 && primCount == 5672 && startIndex == 0))
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 3932 && primCount == 5551 && startIndex == 0) ||
				(NumVertices == 4536 && primCount == 5963 && startIndex == 0) ||
				(NumVertices == 984 && primCount == 1466 && startIndex == 0) ||
				(NumVertices == 1006 && primCount == 1560 && startIndex == 0) ||
				(NumVertices == 3854 && primCount == 6693 && startIndex == 0) ||
				(NumVertices == 3845 && primCount == 5545 && startIndex == 0) ||
				(NumVertices == 1718 && primCount == 1914 && startIndex == 0) ||
				(NumVertices == 5008 && primCount == 6772 && startIndex == 0) ||
				(NumVertices == 5781 && primCount == 8207 && startIndex == 0)) //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "ar_shoots")
	{
			if (team == CSGO_TEAM_T)	//place here CT offsets
			{
				if ((NumVertices == 1645 && primCount == 2568 && startIndex == 0) ||
					(NumVertices == 1601 && primCount == 2502 && startIndex == 0) ||
					(NumVertices == 1611 && primCount == 2522 && startIndex == 0))	//CTheads
				{
					*isHead = true;
					return true;
				}
				else if ((NumVertices == 1258 && primCount == 1950 && startIndex == 0) ||
					(NumVertices == 3630 && primCount == 5404 && startIndex == 0) ||
					(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
				{
					*isHead = false;
					return true;
				}
				else
					return false;

			}

			else if (team == CSGO_TEAM_CT)	//place here T offsets
			{
				if ((NumVertices == 1380 && primCount == 2493 && startIndex == 0) ||
					(NumVertices == 1314 && primCount == 2367 && startIndex == 0) ||
					(NumVertices == 1996 && primCount == 2906 && startIndex == 0) ||
					(NumVertices == 1183 && primCount == 2045 && startIndex == 0)) //T-heads
				{
					*isHead = true;
					return true;
				}
				else if ((NumVertices == 934 && primCount == 1525 && startIndex == 0) ||
					(NumVertices == 7236 && primCount == 8208 && startIndex == 0) ||
					(NumVertices == 991 && primCount == 1525 && startIndex == 0))  //T-body
				{
					*isHead = false;
					return true;
				}
				else
					return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_sugarcane")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
				if ((NumVertices == 1645 && primCount == 2568 && startIndex == 0) ||
					(NumVertices == 1601 && primCount == 2502 && startIndex == 0) ||
					(NumVertices == 1611 && primCount == 2522 && startIndex == 0))	//CTheads
				{
					*isHead = true;
					return true;
				}
				else if ((NumVertices == 1258 && primCount == 1950 && startIndex == 0) ||
					(NumVertices == 3630 && primCount == 5404 && startIndex == 0) ||
					(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
				{
					*isHead = false;
					return true;
				}
				else
					return false;

		}

			else if (team == CSGO_TEAM_CT)	//place here T offsets
			{
				if ((NumVertices == 1380 && primCount == 2493 && startIndex == 0) ||
					(NumVertices == 1314 && primCount == 2367 && startIndex == 0) ||
					(NumVertices == 1996 && primCount == 2906 && startIndex == 0) ||
					(NumVertices == 1183 && primCount == 2045 && startIndex == 0)) //T-heads
				{
					*isHead = true;
					return true;
				}
				else if ((NumVertices == 934 && primCount == 1525 && startIndex == 0) ||
					(NumVertices == 7236 && primCount == 8208 && startIndex == 0) ||
					(NumVertices == 991 && primCount == 1525 && startIndex == 0))  //T-body
				{
					*isHead = false;
					return true;
				}
				else
					return false;
			}
			else
				return false;
	}
	else if (g_MapName == "cs_office")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2778 && primCount == 4244 && startIndex == 0) ||
				(NumVertices == 1459 && primCount == 2566 && startIndex == 0) ||
				(NumVertices == 993 && primCount == 1722 && startIndex == 0) ||
				(NumVertices == 2957 && primCount == 4234 && startIndex == 0) ||
				(NumVertices == 2223 && primCount == 3672 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 4327 && primCount == 6905 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0) ||
				(NumVertices == 1646 && primCount == 2714 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1925 && primCount == 3314 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1671 && primCount == 2876 && startIndex == 0) ||
				(NumVertices == 1462 && primCount == 2338 && startIndex == 0) ||
				(NumVertices == 2447 && primCount == 4436 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "cs_insertion")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2539 && primCount == 3527 && startIndex == 0) ||
				(NumVertices == 4084 && primCount == 4697 && startIndex == 0) ||
				(NumVertices == 2963 && primCount == 3792 && startIndex == 0) ||
				(NumVertices == 3137 && primCount == 3948 && startIndex == 0) ||
				(NumVertices == 3245 && primCount == 4158 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2924 && primCount == 3540 && startIndex == 0) ||
				(NumVertices == 4533 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4422 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4666 && primCount == 6482 && startIndex == 0) ||
				(NumVertices == 4510 && primCount == 6422 && startIndex == 0) ||
				(NumVertices == 4320 && primCount == 6226 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "cs_italy")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1645 && primCount == 2568 && startIndex == 0) ||
				(NumVertices == 1601 && primCount == 2502 && startIndex == 0) ||
				(NumVertices == 1611 && primCount == 2522 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1258 && primCount == 1950 && startIndex == 0) ||
				(NumVertices == 3630 && primCount == 5404 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1106 && primCount == 2018 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2536 && primCount == 4196 && startIndex == 0) ||
				(NumVertices == 1914 && primCount == 3210 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_canals")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2539 && primCount == 3527 && startIndex == 0) ||
				(NumVertices == 4084 && primCount == 4697 && startIndex == 0) ||
				(NumVertices == 2963 && primCount == 3792 && startIndex == 0) ||
				(NumVertices == 3137 && primCount == 3948 && startIndex == 0) ||
				(NumVertices == 3245 && primCount == 4158 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2924 && primCount == 3540 && startIndex == 0) ||
				(NumVertices == 4533 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4422 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4666 && primCount == 6482 && startIndex == 0) ||
				(NumVertices == 4510 && primCount == 6422 && startIndex == 0) ||
				(NumVertices == 4320 && primCount == 6226 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_lake")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2539 && primCount == 3527 && startIndex == 0) ||
				(NumVertices == 4084 && primCount == 4697 && startIndex == 0) ||
				(NumVertices == 2963 && primCount == 3792 && startIndex == 0) ||
				(NumVertices == 3137 && primCount == 3948 && startIndex == 0) ||
				(NumVertices == 3245 && primCount == 4158 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2924 && primCount == 3540 && startIndex == 0) ||
				(NumVertices == 4533 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4422 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4666 && primCount == 6482 && startIndex == 0) ||
				(NumVertices == 4510 && primCount == 6422 && startIndex == 0) ||
				(NumVertices == 4320 && primCount == 6226 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_nuke")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 2539 && primCount == 3527 && startIndex == 0) ||
				(NumVertices == 4084 && primCount == 4697 && startIndex == 0) ||
				(NumVertices == 2963 && primCount == 3792 && startIndex == 0) ||
				(NumVertices == 3137 && primCount == 3948 && startIndex == 0) ||
				(NumVertices == 3245 && primCount == 4158 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 2924 && primCount == 3540 && startIndex == 0) ||
				(NumVertices == 4533 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4422 && primCount == 6319 && startIndex == 0) ||
				(NumVertices == 4666 && primCount == 6482 && startIndex == 0) ||
				(NumVertices == 4510 && primCount == 6422 && startIndex == 0) ||
				(NumVertices == 4320 && primCount == 6226 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1395 && primCount == 2406 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 640 && primCount == 1136 && startIndex == 0) ||
				(NumVertices == 4410 && primCount == 6191 && startIndex == 0) ||
				(NumVertices == 3164 && primCount == 4728 && startIndex == 0) ||
				(NumVertices == 2112 && primCount == 2998 && startIndex == 0) ||
				(NumVertices == 622 && primCount == 1104 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (g_MapName == "de_stmarc")
	{
		if (team == CSGO_TEAM_T)	//place here CT offsets
		{
			if ((NumVertices == 1645 && primCount == 2568 && startIndex == 0) ||
				(NumVertices == 1601 && primCount == 2502 && startIndex == 0) ||
				(NumVertices == 1611 && primCount == 2522 && startIndex == 0))	//CTheads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 1258 && primCount == 1950 && startIndex == 0) ||
				(NumVertices == 3630 && primCount == 5404 && startIndex == 0) ||
				(NumVertices == 2052 && primCount == 2966 && startIndex == 0))	//CTbody
			{
				*isHead = false;
				return true;
			}
			else
				return false;

		}

		else if (team == CSGO_TEAM_CT)	//place here T offsets
		{
			if ((NumVertices == 1380 && primCount == 2493 && startIndex == 0) ||
				(NumVertices == 1314 && primCount == 2367 && startIndex == 0) ||
				(NumVertices == 1996 && primCount == 2906 && startIndex == 0) ||
				(NumVertices == 1183 && primCount == 2045 && startIndex == 0)) //T-heads
			{
				*isHead = true;
				return true;
			}
			else if ((NumVertices == 934 && primCount == 1525 && startIndex == 0) ||
				(NumVertices == 7236 && primCount == 8208 && startIndex == 0) ||
				(NumVertices == 991 && primCount == 1525 && startIndex == 0))  //T-body
			{
				*isHead = false;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

void __fastcall _whdip(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	bool ch_en, ch_sepinv, ch_oh, ch_headc;
	WaitForSingleObject(hWHMutex, INFINITE);
	ch_en = g_ChamsEnabled;
	ch_sepinv = g_ChamsSepInvisCol;
	ch_oh = g_bOnlyHead;
	ch_headc = g_bHeadDifCol;
	ReleaseMutex(hWHMutex);
	if (ch_en && !g_EngineClient->IsTakingScreenshot())
	{
		int myteam;
		bool do_wh = false;
		bool is_head = false;

		WaitForSingleObject(hGlobalsMutex, INFINITE);
			myteam = g_myTeam;
		ReleaseMutex(hGlobalsMutex);

		LPDIRECT3DVERTEXBUFFER9 Stream_Data;
		
		UINT offset = 0;
		UINT stride = 0;

		if (pDev->GetStreamSource(0, &Stream_Data, &offset, &stride) == S_OK)
			Stream_Data->Release();

		if (stride == 32)
			do_wh = CheckVertices(NumVertices, primCount, startIndex, myteam, &is_head);

		if (ch_oh && !is_head)
			return;

		if (do_wh)
		{
			
				pDev->SetRenderState(D3DRS_ZENABLE, false);
				pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				if (ch_headc)
				{
					if (is_head)
						pDev->SetTexture(0, txHeadInv);
					else
						pDev->SetTexture(0, txInv);
				}
				else
					pDev->SetTexture(0, txInv);

				if (ch_sepinv)
				{
					oDrawIndexedPrimitive(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
					pDev->SetRenderState(D3DRS_ZENABLE, true);
					pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
					if (ch_headc)
					{
						if (is_head)
							pDev->SetTexture(0, txHead);
						else
							pDev->SetTexture(0, txGen);
					}
					else
						pDev->SetTexture(0, txGen);
				}
		}
	}
}

void __fastcall _whEndScene(LPDIRECT3DDEVICE9 pDev)
{
	WaitForSingleObject(hWHMutex, INFINITE);
	if (txGen)
	{
		txGen->Release();
		txGen = NULL;
	}

	if (txInv)
	{
		txInv->Release();
		txInv = NULL;
	}

	if (txHead)
	{
		txHead->Release();
		txHead = NULL;
	}

	if (txHeadInv)
	{
		txHeadInv->Release();
		txHeadInv = NULL;
	}
	GenerateTexture(pDev, &txGen, D3DCOLOR_ARGB(255, g_cGen[0], g_cGen[1], g_cGen[2]));
	GenerateTexture(pDev, &txInv, D3DCOLOR_ARGB(255, g_cInv[0], g_cInv[1], g_cInv[2]));
	GenerateTexture(pDev, &txHead, D3DCOLOR_ARGB(255, g_cHead[0], g_cHead[1], g_cHead[2]));
	GenerateTexture(pDev, &txHeadInv, D3DCOLOR_ARGB(255, g_cInvHead[0], g_cInvHead[1], g_cInvHead[2]));
	ReleaseMutex(hWHMutex);

}

void __fastcall _whReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (txGen)
	{
		txGen->Release();
		txGen = NULL;
	}

	if (txInv)
	{
		txInv->Release();
		txInv = NULL;
	}

	if (txHead)
	{
		txHead->Release();
		txHead = NULL;
	}

	if (txHeadInv)
	{
		txHeadInv->Release();
		txHeadInv = NULL;
	}
}
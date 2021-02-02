//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef RECIPIENTFILTER_H
#define RECIPIENTFILTER_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Purpose: Generic interface for routing messages to users
//-----------------------------------------------------------------------------
class IRecipientFilter
{
public:
	virtual			~IRecipientFilter() {}

	virtual bool	IsReliable(void) const = 0;
	virtual bool	IsInitMessage(void) const = 0;

	virtual int		GetRecipientCount(void) const = 0;
	virtual int		GetRecipientIndex(int slot) const = 0;
};



#include "..\sdk.hpp"

//-----------------------------------------------------------------------------
// Purpose: A generic filter for determining whom to send message/sounds etc. to and
//  providing a bit of additional state information
//-----------------------------------------------------------------------------
class CRecipientFilter : public IRecipientFilter
{
public:
	CRecipientFilter();
	virtual 		~CRecipientFilter();

	virtual bool	IsReliable(void) const;
	virtual bool	IsInitMessage(void) const;

	virtual int		GetRecipientCount(void) const;
	virtual int		GetRecipientIndex(int slot) const;

public:

	void			CopyFrom(const CRecipientFilter& src);

	void			Reset(void);

	void			MakeInitMessage(void);

	void			MakeReliable(void);

	void			AddAllPlayers(void);
	void			AddRecipient(const C_BasePlayer *player);
	void			RemoveAllRecipients(void);
	void			RemoveRecipient(C_BasePlayer *player);
	void			RemoveRecipientByPlayerIndex(int playerindex);


	void			UsePredictionRules(void);
	bool			IsUsingPredictionRules(void) const;

	bool			IgnorePredictionCull(void) const;
	void			SetIgnorePredictionCull(bool ignore);



private:

	bool				m_bReliable;
	bool				m_bInitMessage;
	CUtlVector< int >	m_Recipients;

	// If using prediction rules, the filter itself suppresses local player
	bool				m_bUsingPredictionRules;
	// If ignoring prediction cull, then external systems can determine
	//  whether this is a special case where culling should not occur
	bool				m_bIgnorePredictionCull;
};



//-----------------------------------------------------------------------------
// Purpose: Simple class to create a filter for all players ( unreliable )
//-----------------------------------------------------------------------------
class CBroadcastRecipientFilter : public CRecipientFilter
{
public:
	CBroadcastRecipientFilter(void)
	{
		AddAllPlayers();
	}
};

//-----------------------------------------------------------------------------
// Purpose: Simple class to create a filter for all players ( reliable )
//-----------------------------------------------------------------------------
class CReliableBroadcastRecipientFilter : public CBroadcastRecipientFilter
{
public:
	CReliableBroadcastRecipientFilter(void)
	{
		MakeReliable();
	}
};

//-----------------------------------------------------------------------------
// Purpose: Simple class to create a filter for all players except for one ( unreliable )
//-----------------------------------------------------------------------------
class CBroadcastNonOwnerRecipientFilter : public CRecipientFilter
{
public:
	CBroadcastNonOwnerRecipientFilter(C_BasePlayer *player)
	{
		AddAllPlayers();
		RemoveRecipient(player);
	}
};


#endif 
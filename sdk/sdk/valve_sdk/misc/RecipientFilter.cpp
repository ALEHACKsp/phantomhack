#include "RecipientFilter.h"

#include "..\csgostructs.hpp"
#include "..\sdk.hpp"
#include "recipientfilter.h"
#include "..\interfaces\IPrediction.hpp"


static IPrediction g_RecipientFilterPredictionSystem;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CRecipientFilter::CRecipientFilter()
{
	Reset();
}

CRecipientFilter::~CRecipientFilter()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : src - 
//-----------------------------------------------------------------------------
void CRecipientFilter::CopyFrom(const CRecipientFilter& src)
{
	m_bReliable = src.IsReliable();
	m_bInitMessage = src.IsInitMessage();

	m_bUsingPredictionRules = src.IsUsingPredictionRules();
	m_bIgnorePredictionCull = src.IgnorePredictionCull();

	int c = src.GetRecipientCount();
	for (int i = 0; i < c; ++i)
	{
		m_Recipients.AddToTail(src.GetRecipientIndex(i));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CRecipientFilter::Reset(void)
{
	m_bReliable = false;
	m_bInitMessage = false;
	m_Recipients.RemoveAll();
	m_bUsingPredictionRules = false;
	m_bIgnorePredictionCull = false;
}

void CRecipientFilter::MakeReliable(void)
{
	m_bReliable = true;
}

bool CRecipientFilter::IsReliable(void) const
{
	return m_bReliable;
}

int CRecipientFilter::GetRecipientCount(void) const
{
	return m_Recipients.m_Size;
}

int	CRecipientFilter::GetRecipientIndex(int slot) const
{
	if (slot < 0 || slot >= GetRecipientCount())
		return -1;

	return m_Recipients[slot];
}

void CRecipientFilter::AddRecipient(const C_BasePlayer *player)
{
	if (!player)
		return;

	int index = player->EntIndex();
	// Already in list
	if (m_Recipients.HasElement(index))
		return;

	m_Recipients.AddToTail(index);
}

void CRecipientFilter::AddAllPlayers(void)
{
	m_Recipients.RemoveAll();

	int i;
	for (i = 1; i <= g_GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (!pPlayer)
		{
			continue;
		}

		AddRecipient(pPlayer);
	}
}

void CRecipientFilter::RemoveAllRecipients(void)
{
	m_Recipients.RemoveAll();
}

void CRecipientFilter::RemoveRecipient(C_BasePlayer *player)
{
	if (player)
	{
		int index = player->EntIndex();

		// Remove it if it's in the list
		m_Recipients.FindAndRemove(index);
	}
}

void CRecipientFilter::RemoveRecipientByPlayerIndex(int playerindex)
{
	if (playerindex >= 1 && playerindex <= 64)
		return;

	m_Recipients.FindAndRemove(playerindex);
}

bool CRecipientFilter::IsInitMessage(void) const
{
	return m_bInitMessage;
}

void CRecipientFilter::MakeInitMessage(void)
{
	m_bInitMessage = true;
}

void CRecipientFilter::UsePredictionRules(void)
{
	if (m_bUsingPredictionRules)
		return;

	m_bUsingPredictionRules = true;

	// Cull list now, if needed
	if (GetRecipientCount() == 0)
		return;

	C_BasePlayer *pPlayer = g_LocalPlayer;

	if (pPlayer)
	{
		RemoveRecipient(pPlayer);
	}
}

bool CRecipientFilter::IsUsingPredictionRules(void) const
{
	return m_bUsingPredictionRules;
}

bool CRecipientFilter::IgnorePredictionCull(void) const
{
	return m_bIgnorePredictionCull;
}

void CRecipientFilter::SetIgnorePredictionCull(bool ignore)
{
	m_bIgnorePredictionCull = ignore;
}

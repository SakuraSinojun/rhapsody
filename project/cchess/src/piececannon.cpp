
/*
 *  PieceCannon.cpp
 *
 *  Cannon Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Cannon pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"


CPieceCannon::CPieceCannon(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_CANNON;
	board->AddPiece(this);
	m_x = 1;
	m_y = 2;
}

CPieceCannon::~CPieceCannon()
{
}

int CPieceCannon::MoveTest(int x, int y)
{

	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
			
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}

	//������б���������...	
	if(m_x != x && m_y != y)
	{
		return PM_UNREACHABLE;
	}

	int i;
	int min, max;
	CPiece * piece;
	int count ;
	
	//ͳ���м�������
	count = 0;
	if(m_x != x)
	{
		//�����ƶ�
		min = x;
		max = x;
		
		if(x < m_x)
		{
			max = m_x;
		}else{
			min = m_x;
		}
		
		for(i=min+1; i<max; i++)
		{
			piece = m_board->GetPiece(i, y);
			if (piece != NULL)
			{
				count ++;
			}
		}
	}else{	
		//�����ƶ�
		min = y;
		max = y;
		
		if(y < m_y)
		{
			max = m_y;
		}else{
			min = m_y;
		}
		
		for(i=min+1; i<max; i++)
		{
			piece = m_board->GetPiece(x, i);
			if (piece != NULL)
			{
				count ++;
			}
		}
	}
	
	if(count > 1)
	{
		//�򲻵�....
		return PM_UNREACHABLE;
	}
	
	piece = m_board->GetPiece(x, y);

	//���������...
	if(piece == NULL)
	{
		if (count == 1)
		{
			return PM_UNREACHABLE;
		}else{
			return PM_MOVE;
		}
	}

	//������Լ�������...
	if(piece->GetCamp() == this->GetCamp())
	{
		return PM_UNREACHABLE;
	}
	
	//����һ���Ӳ��ܳ�...
	if(count == 1)
	{
		return PM_CAPTURE;
	}
	
	return PM_UNREACHABLE;
	
}






















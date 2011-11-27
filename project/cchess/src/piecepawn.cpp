
/*
 *  PiecePawn.cpp
 *
 *  Pawn Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Pawn pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"


CPiecePawn::CPiecePawn(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_PAWN;
	board->AddPiece(this);
	m_x = 0;
	m_y = 3;
	m_direction = 1;
}

CPiecePawn::~CPiecePawn()
{
}


int CPiecePawn::MoveTest(int x, int y)
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
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	if ((cx != 1 && cy != 1 ))
	{	
		//ֻ׼��һ��....
		return PM_UNREACHABLE;
	}
	
	if (x != m_x)
	{
		//������
		
		if (m_direction == 1)
		{
			//������
		
			if (m_y < 5)
			{
				//û�����ػ�...
				return PM_UNREACHABLE;
			}
		}else{
			//������
			
			if (m_y > 4)
			{
				//û����
				return PM_UNREACHABLE;
			}
		}
	}else{
		//������.
		
		if( y-m_y != m_direction)
		{
			return PM_UNREACHABLE;
		}
	}
	
	CPiece * piece = m_board->GetPiece(x, y);
	if(piece == NULL)
	{
		return PM_MOVE;
	}
	if(piece->GetCamp() == this->GetCamp())
	{
		return PM_UNREACHABLE;
	}
	return PM_CAPTURE;

}



















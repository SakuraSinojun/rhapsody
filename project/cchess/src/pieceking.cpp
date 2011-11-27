
/*
 *  PieceKing.cpp
 *
 *  King Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about King pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"

#include <windows.h>

CPieceKing::CPieceKing(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_KING;
	board->AddPiece(this);
	m_x = 3;
	m_y = 0;
}

CPieceKing::~CPieceKing()
{
}

int CPieceKing::MoveTest(int x, int y)
{
	CPiece * piece = NULL;
	CPiece * p1;
	int i;
	int ey;
	int min, max;
	bool flag;
	
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	if ( x!=m_x && y!=m_y )
	{
		//����б����...
		return PM_UNREACHABLE;
	}
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	
	
	if ((cx != 1 && cy != 1 ))
	{	
		//���˺ö��....
		
		if(cx>1)
		{
			//MessageBox(NULL, "�����������..", "", MB_OK);
			return PM_UNREACHABLE;
		}
		
		//����Ҫ�����̳ԶԷ��Ͻ���.~
		piece = GetPiece(x, y);
		if(piece == NULL || piece->GetTagID() != TAG_KING || piece->GetCamp() == this->GetCamp())
		{
			//Ҫ����Ҫ����?
			return PM_UNREACHABLE;
		}
		
		min = y;
		max = y;
		flag = false;
		if (y > m_y)
		{
			min = m_y;
		}else{
			max = m_y;
		}
		for(i=min+1; i<max; i++)
		{
			if (GetPiece(x, i) != NULL)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			return PM_CAPTURE;
		}
		
		//MessageBox(NULL, "��Ϊ���Ӳ������!", "", MB_OK);
		return PM_UNREACHABLE;
		
	}

	
	if (		(m_y<3 && y>=3)
		||	(m_y>6 && y<=6)
		||	(x < 3)
		||	(x > 5)
	)
	{
		//��������Խ���̾Ͳ����ߵ��������������...
		return PM_UNREACHABLE;
	}
	
	
	if (y == m_y)
	{
		//�ҶԷ���˧..
		if (y<3)
		{
			ey = 7;
		}else{
			ey = 0;
		}
		piece = NULL;
		for(i=0; i<3; i++)
		{
			p1 = m_board->GetPiece(x, ey);
			if(p1 != NULL)
			{
				if(p1->GetTagID() == TAG_KING && p1->GetCamp()!=this->GetCamp())
				{
					piece = p1;
					break;
				}
			}
			ey ++;
		}
		
		flag = false;
		if (piece != NULL)
		{
			//��Ȼ����������....
			min = ey;
			max = ey;
			if (y > ey)
			{
				max = y;
			}else{
				min = y;
			}
			for(i=min+1; i<max; i++)
			{
				if (GetPiece(x, i) != NULL)
				{
					flag = true;
					break;
				}
			}
			if(!flag)
			{
				return PM_UNREACHABLE;
			}
		}
		
	}
	
	
	//�����������..
	
	piece = m_board->GetPiece(x, y);
	
	if(piece == NULL)
	{
		//�յ�...
		return PM_MOVE;
	}
	
	if(piece->GetCamp() == this->GetCamp())
	{
		//�Լ���...
		return PM_UNREACHABLE;
	}
	
	//����...
	return PM_CAPTURE;
	
}






















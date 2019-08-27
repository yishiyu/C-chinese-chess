#include "board.h"
#include <QtDebug>

//马的走法生成
void Board::KnightMove( int p){
	int SideTag = 16 + side * 16;
    int NextPosition;

	for (int i = 0; i < 8; i++) {
		NextPosition = p + KnightDir[i];

		//下一位置合法（且马脚没有被蹩）
		if (KnightPosition[NextPosition] && !board[p + KnightCheck[i]]) {

			//目标位置无己方子
			if (!(board[NextPosition] & SideTag))
				SaveMove(p, NextPosition);
		}
	}
}

//象相的走法生成
void Board::BishopMove(int p){
	int SideTag = 16 + side * 16;
	int NextPosition;

	for (int i = 0; i < 4; i++) {
		NextPosition = p + BishopDir[i];

		//下一位置合法（且象眼没有被赌）
		if (BishopPosition[NextPosition] && !board[p + BishopCheck[i]]) {

			//目标位置无己方子
			if (!(board[NextPosition] & SideTag))
				SaveMove(p, NextPosition);
		}
	}
}

//帅将的走法生成s
void Board::KingMove(int p){
	int SideTag = 16 + side * 16;
    int NextPosition;

	for (int i = 0; i < 4; i++) {
		NextPosition = p + KingDir[i];
		
		//下一个位置合法
		if (KingPosition[NextPosition]) {
			//目标位置无己方子
			if (!(board[NextPosition] & SideTag))
				SaveMove(p, NextPosition);
		}
	}

}

//仕士的走法生成
void Board::AdvisorMove( int p){
	int SideTag = 16 + side * 16;
	 int NextPosition;

	for (int i = 0; i < 4; i++) {
		NextPosition = p + AdvisorDir[i];

		//下一个位置合法
		if (AdvisorPosition[NextPosition]) {
			//目标位置无己方子
			if (!(board[NextPosition] & SideTag))
				SaveMove(p, NextPosition);
		}
	}
}

//车的走法生成
void Board::RookMove( int p){
	int SideTag = 16 + side * 16;
	 int NextPosition;

	//遍历四个方向
	for (int i = 0; i < 4; i++) {
		NextPosition = p;

		for (int j = 0; j < 9; j++) {
			NextPosition = NextPosition + RookDir[i];

			//下一位置是否合法
			if (!RookPosition[NextPosition])
				break;

			//下一位置是否有子
			if (!board[NextPosition]) {
				//不吃子走法
				SaveMove(p,NextPosition);
				continue;
			}
			else {
				//下一位置是自己方的子
				if (board[NextPosition] & SideTag) {
					break;
				}
				else {
					//吃子走法
					SaveMove(p, NextPosition);
					break;
				}
			}
		}

	}
}

//炮的走法生成
void Board::CannonMove( int p){
	int SideTag = 16 + side * 16;
	int OverTag = 0;
	 int NextPosition;
	

	//遍历四个方向
	for (int i = 0; i < 4; i++) {
		NextPosition = p;
		OverTag = 0;

		for (int j = 0; j < 9; j++) {
			NextPosition = NextPosition + RookDir[i];

			//下一位置是否合法
			if (!CannonPosition[NextPosition])
				break;

			//下一位置无子
			if (!board[NextPosition]) {

				//未翻山
				if (!OverTag) {
					//不吃子走法
					SaveMove(p, NextPosition);
					continue;
				}
				
			}
			//下一位置有子（若已翻山则处理完后推出循环）
			else {
				//未翻山
				if (!OverTag) {
					OverTag = 1;
				}
				else {
					//下一位置是对面的子
					if (!(board[NextPosition] & SideTag)) {
						//吃子走法
						SaveMove(p, NextPosition);
					}
					break;
				}

			}
		}

	}
}

//兵卒的走法生成（处理时使用不同方式）
void Board::SoldierMove( int p){
	//side同时作为走法数组下标 0=>红棋 1=>黑棋
	int SideTag = 16 + side * 16;
	 int NextPosition;

	//遍历三个方向
	for (int i = 0; i < 3; i++) {
		NextPosition = p + SoldierDir[side][i];

		//下一位置合法
		if (SoldierPostion[side][NextPosition]) {
			if (!(board[NextPosition] & SideTag))
				SaveMove(p,NextPosition);
		}
	}
}


//生成走法
void Board::GenAllMove(){
	int Position;
	int sideTag = 16 + 16 * side;
	int piece;
	InitGen();

	for (int i = 0; i < 16; i++) {
        piece = sideTag + i;
        //piece = sideTag + GenOrder[i];
		Position = pieces[piece];

		if (Position == 0) {
			continue;
		}
			

        switch (i){
        //switch (GenOrder[i]){
		case 0:		//帅
			KingMove(Position);
			break;
		case 1:		//仕
		case 2:	
			AdvisorMove(Position);
			break;
		case 3:		//相
		case 4:
			BishopMove(Position);
			break;
		case 5:		//马
		case 6:
			KnightMove(Position);
			break;
		case 7:		//车
		case 8:
			RookMove(Position);
			break;
		case 9:		//炮
		case 10:
			CannonMove(Position);
			break;
		case 11:	//兵
		case 12:
		case 13:
		case 14:
		case 15:
			SoldierMove(Position);
			break;
		}

	}

}
void Board::InitGen() {
	MoveNum[Depth] = 0;
}
void Board::SaveMove( int from,  int to){

    int temp[2];
	temp[0] = board[from];
	temp[1] = board[to];

	//假设执行
	board[to] = board[from];
	board[from] = 0;
	pieces[temp[0]] = to;
	pieces[temp[1]] = 0;
	
    if (!check(side)) {
		MoveArray[Depth][MoveNum[Depth]].from = from;
		MoveArray[Depth][MoveNum[Depth]].to = to;
        MoveArray[Depth][MoveNum[Depth]].capture = temp[1];
        MoveNum[Depth]++;
	}
	
	//撤销假设
	board[from] = temp[0];
	board[to] = temp[1];
	pieces[temp[0]] = from;
	pieces[temp[1]] = to;

}

//将军检测 （ISide为被检测方）
int Board::check(int ISide){

    //不灵敏的子
    //炮 车 兵
    //灵敏的子
    //马
    //更正的子
    //车 炮 兵

	//将帅位置
    int RedKing = pieces[16];
    int BlackKing = pieces[32];
    int PosAdd;
    bool result = false;
    int SideTag = 32 - 16 * ISide;		//对面方的标志
	
    //这个沙雕判定会导致不去吃对面的将
    //胜负已定
    //if (!RedKing || !BlackKing)
    //    return true;
    //到底要不要加上这个判定......
    //试了试感觉不加上也可以，可以正确判定

	//检测将帅是否照面
	//双方将帅在同一列
	if ((BlackKing % 16) == (RedKing % 16)) {

		//假设双方直接照面
        result = true;
		//依次检测每一行是否有子
		for (int i = RedKing - 16; i != BlackKing; i -= 16) {
			if (board[i]) {
				//没有直接照面 将result置为0
                result = false;
				break;
			}
		}
		//双方将帅照面
		if(result)
            return true;
	}

	//获取被检测将位置
	 int Position = pieces[16 + 16 * ISide];
	
	//马检测
	{
		//敌对方马（角色值）
		 int dangerous[2] = {SideTag + 5, SideTag + 6};

		//检测有威胁的位置是否有马
		for (int i = 0; i < 8; i++) {
			if ((board[Position + KnightDir[i]] == dangerous[0]) || (board[Position + KnightDir[i]] == dangerous[1])) {
				//马脚没有被蹩
				if (!board[Position + KnightAttackCheck[i]])
                    return true;
			}
		}
	}

	//车检测
	{	
		//敌对方车（位置值）
        int dangerous[2] = { pieces[SideTag + 7], pieces[SideTag + 8] };

		//对两个车进行将军检测（行检测和列检测）
		for (int j = 0; j < 2; j++) {

            //车被吃掉则跳过（woc原来错在这儿，失掉一个子之后另一个子的判定会失效）
			if (!dangerous[j])
                continue;
                //break;

			//列检测
			if ((Position % 16) == (dangerous[j] % 16)) {
                result = true;

				PosAdd = (dangerous[j]>Position) ? 16 : -16;
				for (int i = Position + PosAdd; i != dangerous[j]; i += PosAdd) {

                    //车和将之间有子隔着
					if (board[i]) {
                        result = false;
						break;
					}

				}
				if (result)
                    return true;
			}

			//行检测
			if ((Position / 16) == (dangerous[j] / 16)) {
                result = true;

				PosAdd = (dangerous[j] > Position) ? 1 : -1;
				for (int i = Position + PosAdd; i != dangerous[j]; i += PosAdd) {
					if (board[i]) {
                        result = false;
						break;
					}
				}
				if (result)
                    return true;
			}
		}

	}

	//炮检测
	{
		//敌对方炮（位置值）
        int dangerous[2] = { pieces[SideTag + 9], pieces[SideTag + 10] };
        int OverFlag = 0;
		for (int j = 0; j < 2; j++) {

            //炮被吃掉则跳过（woc原来错在这儿，失掉一个子之后另一个子的判定会失效）
			if (!dangerous[j])
                continue;
                //break;

			//列检测
            if ((Position % 16) == (dangerous[j] % 16)) {
                OverFlag = 0;
				PosAdd = (dangerous[j] > Position) ? 16 : -16;
				for (int i = Position + PosAdd; i != dangerous[j]; i += PosAdd) {
					if (board[i]) {
						OverFlag++;
					}
				}
				if (OverFlag==1)
                    return true;
			}

			//行检测
			if ((Position / 16) == (dangerous[j] / 16)) {
                OverFlag = 0;
				PosAdd = (dangerous[j] > Position) ? 1 : -1;
				for (int i = Position + PosAdd; i != dangerous[j]; i += PosAdd) {
					if (board[i]) {
						OverFlag++;
					}
				}
				if (OverFlag==1)
                    return true;
			}
		}
	}

	//卒检测
	{
		//敌对方卒（位置值）
		 int dangerous[5] = { pieces[SideTag + 11],pieces[SideTag + 12],
									   pieces[SideTag + 13], pieces[SideTag + 14],
									   pieces[SideTag + 15] };
		for (int j = 0; j < 5; j++) {
            //（woc原来错在这儿，失掉一个子之后另一个子的判定会失效）
			if (!dangerous[j])
                continue;
                //break;

			for (int i = 0; i < 3; i++) {
                if (Position == dangerous[j] + SoldierDir[ISide][i])
                    return true;
			}
		}

	}

    return false;
}
/*
* 事先估计没有实现
*/

//局面评价函数
short Board::Eval() {
	//双方分数
	short BlackValue = 0;
	short RedValue = 0;

	//红色方分数统计
	for (int i = 16; i < 32; i++) {
		if (pieces[i] >= 0)
			RedValue += PieceToValue[0][Locate[i]][pieces[i]];
	}

	//黑色方分数统计
	for (int i = 32; i < 48; i++) {
		if (pieces[i] >= 0)
			BlackValue += PieceToValue[1][Locate[i]][pieces[i]];
	}

    //默认为电脑方为黑棋，后期改进的时候需要考虑电脑方为红棋的情况
    //适用于搜索树和分离式剪枝算法的返回值
    return BlackValue - RedValue;
    //return RedValue - BlackValue;
    //适用于剪枝算法的评价返回值
    //return side?(RedValue - BlackValue):(BlackValue - RedValue);
}


/*
* 多重棋子价值、灵活性分数未实现
*/


//走法执行函数
bool Board::MakeMove(move m) {
    int captrue = board[m.to];

    //设置置换表参数
    PresentStation = PresentStation ^ (PieceNum[board[m.from]]&BoardNum[m.from]);
    PresentStation = PresentStation ^ (PieceNum[board[m.from]]&BoardNum[m.to]);
    if(captrue){
        PresentStation = PresentStation ^ (PieceNum[captrue]&BoardNum[m.to]);
    }
    PresentStation = PresentStation ^ SideNum;


	//设置走法栈
	MoveStack[StackTop].from = m.from;
	MoveStack[StackTop].to = m.to;
	MoveStack[StackTop].capture = board[m.to];
	StackTop++;

	//设置棋子数组
	if (captrue > 0)
		pieces[captrue] = 0;
	pieces[board[m.from]] = m.to;

	//设置棋盘数组
	board[m.to] = board[m.from];
	board[m.from] = 0;

	ChangeSide();
	return captrue == (side ? 32 : 16);
}

void Board::UnMakeMove(){

	StackTop--;
    int from = MoveStack[StackTop].from;
    int dest = MoveStack[StackTop].to;
    int capture = MoveStack[StackTop].capture;


    //设置置换表参数
    PresentStation = PresentStation ^ (PieceNum[board[dest]]&BoardNum[from]);
    PresentStation = PresentStation ^ (PieceNum[board[dest]]&BoardNum[dest]);
    if(capture){
        PresentStation = PresentStation ^ (PieceNum[capture]&BoardNum[dest]);
    }
    PresentStation = PresentStation ^ SideNum;


	//设置棋盘数组
	board[from] = board[dest];
	board[dest] = capture;

	//设置棋子数组
	if (capture > 0)
		pieces[capture] = dest;
	pieces[board[from]] = from;
    ChangeSide();
}

//搜索算法入口
int Board::BoardSearch(int depth){
    //改为吃子后清空置换表
    //ClearZobrist();

    //执行树搜索
    //return MaxSearch(depth);

    //执行阿尔法贝塔剪枝搜索
    int score = AlphaBetaSearch(depth, -MaxValue, MaxValue);
    if(BestMove.capture)
        ClearZobrist();
    return score;

    //执行分离形式阿尔法贝塔剪枝搜索
    //return AlphaSearch(depth, -MaxValue, MaxValue);
}


//树搜索算法
int Board::MaxSearch(int depth) {

	if (depth == 0) {
		return Eval();
	}

	int best = -MaxValue;
	move mv;
	int value = 0;

	//在当前层生成所有走法
	GenAllMove();
	int Num = MoveNum[Depth];

	for (int i = 0; i < Num; i++) {
		mv = MoveArray[Depth][i];

		//切换到下一搜索层
		Depth++;

		MakeMove(mv);
		value = MinSearch(depth - 1);
		UnMakeMove();

		//返回到上一搜索层
		Depth--;

		if (value > best) {
			best = value;
			//如果当前层是第一层，则生成直接最佳走法
			if (depth == MAXDEPTH) {
                BestMove = mv;
            }
		}
	}

	return best;
}

int Board::MinSearch(int depth){
	if (depth == 0) {
		return Eval();
	}

	int best = MaxValue;
	move mv;
	int value = 0;

	//在当前层生成所有走法
	GenAllMove();
	int Num = MoveNum[Depth];

	for (int i = 0; i < Num; i++) {
		mv = MoveArray[Depth][i];

		//切换到下一搜索层
		Depth++;

		MakeMove(mv);
		value = MaxSearch(depth - 1);
		UnMakeMove();

		//返回到上一搜索层
		Depth--;

		if (value < best) {
			best = value;
			//如果当前层是第一层，则生成直接最佳走法
			if (depth == MAXDEPTH) {
                BestMove = mv;
			}
		}
	}

    return best;
}


//阿尔法贝塔剪枝算法(已加入置换表）
int Board::AlphaBetaSearch(int depth, int alpha, int beta){
    if (depth == 0) {
        return Eval();
    }

    move mv;
    int value = ReadHashTable();
    if(value != SCORENAN){
        return value;
    }

    //在当前层生成所有走法
    GenAllMove();
    int Num = MoveNum[Depth];

    for (int i = 0; i < Num; i++) {
        mv = MoveArray[Depth][i];

        //切换到下一搜索层
        Depth++;

        MakeMove(mv);
        value = -AlphaBetaSearch(depth-1,-beta,-alpha);
        UnMakeMove();

        //返回到上一搜索层
        Depth--;

        if(value>=beta){
            SaveHashTable(beta);
            return beta;
        }

        if(value>alpha){
            alpha = value;
            SaveHashTable(alpha);
            if(depth==MAXDEPTH)
                BestMove = mv;
        }
    }

    return alpha;
}

//分离形式的阿尔法贝塔剪枝
//寻找当前层最大值
int Board::AlphaSearch(int depth, int alpha, int beta){
    if (depth == 0) {
        return Eval();
    }

    move mv;
    int value = 0;

    //在当前层生成所有走法
    GenAllMove();
    int Num = MoveNum[Depth];

    for (int i = 0; i < Num; i++) {
        mv = MoveArray[Depth][i];

        //切换到下一搜索层
        Depth++;

        MakeMove(mv);
        value = BetaSearch(depth-1,alpha,beta);
        UnMakeMove();

        //返回到上一搜索层
        Depth--;

        if(value>=beta)
            return value;

        if(value>alpha){
            alpha = value;
            if(depth==MAXDEPTH)
                BestMove = mv;
        }
    }

    return alpha;
}
//寻找当前层最小值
int Board::BetaSearch(int depth, int alpha, int beta){
    if (depth == 0) {
        return Eval();
    }

    move mv;
    int value = 0;

    //在当前层生成所有走法
    GenAllMove();
    int Num = MoveNum[Depth];

    for (int i = 0; i < Num; i++) {
        mv = MoveArray[Depth][i];

        //切换到下一搜索层
        Depth++;

        MakeMove(mv);
        value = AlphaSearch(depth-1,alpha,beta);
        UnMakeMove();

        //返回到上一搜索层
        Depth--;

        if(value<=alpha)
            return value;

        if(value<beta){
            beta = value;
            if(depth==MAXDEPTH)
                BestMove = mv;
        }
    }

    return beta;
}


//置换表相关函数
int Board::ReadHashTable(){
    //查找
    std::hash_map<long,station>::iterator temp = HashTable.find(PresentStation);

    //找到相关信息
    if(temp!=HashTable.end()){
        if(temp->second.depth<=Depth)
            return temp->second.score;
    }
    return SCORENAN;
}

void Board::SaveHashTable(int value){
    station temp;
    temp.score = value;
    temp.depth = Depth;
    HashTable[PresentStation] = temp;
}


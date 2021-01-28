
#include <cstdio>

using namespace std;

void print(int x)
{
	if (x == 0)
		printf("R ");
	if (x == 1)
		printf("Ri ");
	if (x == 2)
		printf("R R ");
	if (x == 3)
		printf("L ");
	if (x == 4)
		printf("Li ");
	if (x == 5)
		printf("L L ");
	if (x == 6)
		printf("B ");
	if (x == 7)
		printf("Bi ");
	if (x == 8)
		printf("B B ");
	if (x == 9)
		printf("D ");
	if (x == 10)
		printf("Di ");
	if (x == 11)
		printf("D D ");
	if (x == 12)
		printf("F ");
	if (x == 13)
		printf("Fi ");
	if (x == 14)
		printf("F F ");
	if (x == 15)
		printf("U ");
	if (x == 16)
		printf("Ui ");
	if (x == 17)
		printf("U U ");
}

int power12[100], power8[100], power3[100], power2[100];
void prepare_power()
{
	power12[0] = power8[0] = power3[0] = power2[0] = 1;
	for (int i = 1;i < 100;++i)
	{
		power12[i] = power12[i - 1] * 12, power8[i] = power8[i - 1] * 8;
		power3[i] = power3[i - 1] * 3, power2[i] = power2[i - 1] * 2;
	}
}

#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline", "fast-math", "unroll-loops", "no-stack-protector")
#pragma GCC diagnostic error "-fwhole-program"
#pragma GCC diagnostic error "-fcse-skip-blocks"
#pragma GCC diagnostic error "-funsafe-loop-optimizations"
#include <set>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>

vector<int> Answer, Answer2;

void print_Answer()
{
	for (int i = 0;i < Answer.size();++i)
		print(Answer[i]);
	for (int i = 0;i < Answer2.size();++i)
		print(Answer2[i]);
}

namespace convenience
{
	int corpos[8][18], edgpos[12][18];
	//corpos -- positions of corner-cubes
	//edepos -- positions of edge-cubes
	int cordir[6600/*3^8*/][18], edgdir[(2 << 12) + 5/*2^12*/][18];
	//cordir -- directions of corner-cubes
	//edgdir -- directions of edge-cubes
	pair<int, int> cor_rec[8][3], edg_rec[12][2];
	//record[i][j]表示角块的第i个位置，方向为j的时候，主色所在的下标是<record[i][j].first, record[i][j].second>
	char cube[7][10], temp[7][10];
	
	//下标(x, y)：传统表示法中，第x个面的第y个小面 
	struct dim_3
	{
		int x1, y1, x2, y2, x3, y3;
		void give(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3)//用法：all_cor[i].give(x1, y1, x2, y2, x3, y3) 
		{
			x1 = _x1, y1 = _y1, x2 = _x2, y2 = _y2, x3 = _x3, y3 = _y3;
		}
	};
	dim_3 all_cor[8];//all_cor[i]记录第i个角的三个小面下标(x1, y1), (x2, y2), (x3, y3)
	struct dim_2
	{
		int x1, y1, x2, y2;
		void give(int _x1, int _y1, int _x2, int _y2)//用法：all_edg[i].give(x1, y1, x2, y2) 
		{
			x1 = _x1, y1 = _y1, x2 = _x2, y2 = _y2;
		}
	};
	dim_2 all_edg[12];//all_edg[i]记录第i个棱的两个小面下标(x1, y1), (x2, y2)
	int tedgpos[12][18], tcorpos[8][18];
	void copy(char a[7][10],char b[7][10])
	{//a=b
    		for(int i=1; i<=6; i++)
        		for(int j=1; j<=9; j++)
            			a[i][j]=b[i][j];
	}
    	void prepare_R()
	{
    		edgpos[9][0]=1;
    		edgpos[1][0]=10;
    		edgpos[10][0]=5;
    		edgpos[5][0]=9;
    		
    		corpos[1][0]=2;
    		corpos[2][0]=6;
    		corpos[6][0]=5;
    		corpos[5][0]=1;
	}
	void prepare_Ri()
	{
    		edgpos[9][1]=5;
    		edgpos[1][1]=9;
    		edgpos[10][1]=1;
    		edgpos[5][1]=10;
    		
    		corpos[1][1]=5;
    		corpos[5][1]=6;
    		corpos[6][1]=2;
    		corpos[2][1]=1;
	}
	void prepare_RR()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][2]=edgpos[edgpos[i][0]][0];
    		for(int i=0;i<=7;i++)
        		corpos[i][2]=corpos[corpos[i][0]][0];
	}
	void prepare_L()
	{
    		edgpos[8][3]=7;
    		edgpos[3][3]=8;
   		edgpos[11][3]=3;
    		edgpos[7][3]=11;
    		
    		corpos[0][3]=4;
    		corpos[3][3]=0;
    		corpos[7][3]=3;
    		corpos[4][3]=7;
	}
	void prepare_Li()
	{
    		edgpos[8][4]=3;
    		edgpos[3][4]=11;
    		edgpos[11][4]=7;
    		edgpos[7][4]=8;
    		
    		corpos[0][4]=3;
    		corpos[3][4]=7;
    		corpos[7][4]=4;
    		corpos[4][4]=0;
	}
	void prepare_LL()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][5]=edgpos[edgpos[i][3]][3];
    		for(int i=0;i<=7;i++)
      			corpos[i][5]=corpos[corpos[i][3]][3];
	}
	void prepare_B()
	{
		edgpos[2][6]=11;
		edgpos[11][6]=6;
		edgpos[6][6]=10;
    		edgpos[10][6]=2;
    		
    		corpos[2][6]=3;
    		corpos[3][6]=7;
   		corpos[7][6]=6;
    		corpos[6][6]=2;
	}
	void prepare_Bi()
	{
    		edgpos[2][7]=10;
    		edgpos[10][7]=6;
    		edgpos[6][7]=11;
    		edgpos[11][7]=2;
    		
    		corpos[2][7]=6;
    		corpos[3][7]=2;
    		corpos[7][7]=3;
    		corpos[6][7]=7;
	}
	void prepare_BB()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][8]=edgpos[edgpos[i][6]][6];
    		for(int i=0;i<=7;i++)
        		corpos[i][8]=corpos[corpos[i][6]][6];
	}
	void prepare_D()
	{
    		edgpos[4][9]=5;
    		edgpos[5][9]=6;
    		edgpos[6][9]=7;
    		edgpos[7][9]=4;
    		
    		corpos[4][9]=5;
    		corpos[5][9]=6;
    		corpos[6][9]=7;
    		corpos[7][9]=4;
	}
	void prepare_Di()
	{
    		edgpos[4][10]=7;
    		edgpos[7][10]=6;
    		edgpos[6][10]=5;
    		edgpos[5][10]=4;
    		
    		corpos[4][10]=7;
    		corpos[7][10]=6;
    		corpos[6][10]=5;
    		corpos[5][10]=4;
	}
	void prepare_DD()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][11]=edgpos[edgpos[i][9]][9];
    		for(int i=0;i<=7;i++)
        		corpos[i][11]=corpos[corpos[i][9]][9];
	}
	void prepare_F()
	{
	    	edgpos[0][12]=9;
    		edgpos[9][12]=4;
    		edgpos[4][12]=8;
    		edgpos[8][12]=0;
    		
    		corpos[0][12]=1;
    		corpos[1][12]=5;
    		corpos[5][12]=4;
    		corpos[4][12]=0;
	}
	void prepare_Fi()
	{
    		edgpos[0][13]=8;
    		edgpos[8][13]=4;
    		edgpos[4][13]=9;
    		edgpos[9][13]=0;
    		
    		corpos[0][13]=4;
    		corpos[4][13]=5;
    		corpos[5][13]=1;
    		corpos[1][13]=0;
	}
	void prepare_FF()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][14]=edgpos[edgpos[i][12]][12];
    		for(int i=0;i<=7;i++)
        		corpos[i][14]=corpos[corpos[i][12]][12];
	}
	void prepare_Ui()
	{
    		edgpos[0][16]=1;
    		edgpos[1][16]=2;
    		edgpos[2][16]=3;
    		edgpos[3][16]=0;
    		
    		corpos[0][16]=1;
    		corpos[1][16]=2;
    		corpos[2][16]=3;
    		corpos[3][16]=0;
	}
	void prepare_U()
	{
    		edgpos[0][15]=3;
    		edgpos[3][15]=2;
    		edgpos[2][15]=1;
    		edgpos[1][15]=0;
    		
    		corpos[0][15]=3;
    		corpos[3][15]=2;
    		corpos[2][15]=1;
    		corpos[1][15]=0;
	}
	void prepare_UU()
	{
    		for(int i=0;i<=11;i++)
        		edgpos[i][17]=edgpos[edgpos[i][15]][15];
    		for(int i=0;i<=7;i++)
        		corpos[i][17]=corpos[corpos[i][15]][15];
	}
	void r_R()
	{
		copy(temp,cube);
		cube[1][3]=temp[6][3];
		cube[1][6]=temp[6][6];
		cube[1][9]=temp[6][9];
		cube[5][3]=temp[1][3];
		cube[5][6]=temp[1][6];
		cube[5][9]=temp[1][9];
		cube[2][1]=temp[5][9];
		cube[2][4]=temp[5][6];
		cube[2][7]=temp[5][3];
		cube[6][3]=temp[2][7];
		cube[6][6]=temp[2][4];
		cube[6][9]=temp[2][1];
		
		cube[4][1]=temp[4][7];
		cube[4][2]=temp[4][4];
		cube[4][3]=temp[4][1];
		cube[4][6]=temp[4][2];
		cube[4][9]=temp[4][3];
		cube[4][8]=temp[4][6];
		cube[4][7]=temp[4][9];
		cube[4][4]=temp[4][8];
	}
	void r_Ri()
	{
   		r_R();
		r_R();
  		r_R();
	}
	void r_L()
	{
		copy(temp,cube);
		cube[1][1]=temp[5][1];
		cube[1][4]=temp[5][4];
		cube[1][7]=temp[5][7];
		cube[6][1]=temp[1][1];
		cube[6][4]=temp[1][4];
		cube[6][7]=temp[1][7];
		cube[2][3]=temp[6][7];
		cube[2][6]=temp[6][4];
		cube[2][9]=temp[6][1];
		cube[5][1]=temp[2][9];
		cube[5][4]=temp[2][6];
		cube[5][7]=temp[2][3];

		cube[3][3]=temp[3][1];
		cube[3][6]=temp[3][2];
		cube[3][9]=temp[3][3];
		cube[3][8]=temp[3][6];
		cube[3][7]=temp[3][9];
		cube[3][4]=temp[3][8];
		cube[3][1]=temp[3][7];
		cube[3][2]=temp[3][4];
	}
	void r_Li()
	{
		r_L();
		r_L();
		r_L();
	}
	void r_B()
	{
		copy(temp,cube);
		cube[5][1]=temp[4][3];
		cube[5][2]=temp[4][6];
		cube[5][3]=temp[4][9];
		cube[3][7]=temp[5][1];
		cube[3][4]=temp[5][2];
		cube[3][1]=temp[5][3];
		cube[6][9]=temp[3][7];
		cube[6][8]=temp[3][4];
		cube[6][7]=temp[3][1];
		cube[4][3]=temp[6][9];
		cube[4][6]=temp[6][8];
		cube[4][9]=temp[6][7];
		
		cube[2][1]=temp[2][7];
		cube[2][2]=temp[2][4];
		cube[2][3]=temp[2][1];
		cube[2][6]=temp[2][2];
		cube[2][9]=temp[2][3];
		cube[2][8]=temp[2][6];
		cube[2][7]=temp[2][9];
		cube[2][4]=temp[2][8];
	}
	void r_Bi()
	{
		r_B();
		r_B();
		r_B();
	}
	void r_D()
	{
		copy(temp,cube);
		cube[1][7]=temp[3][7];
		cube[1][8]=temp[3][8];
		cube[1][9]=temp[3][9];
		cube[4][7]=temp[1][7];
		cube[4][8]=temp[1][8];
		cube[4][9]=temp[1][9];
		cube[2][7]=temp[4][7];
		cube[2][8]=temp[4][8];
		cube[2][9]=temp[4][9];
		cube[3][9]=temp[2][9];
		cube[3][8]=temp[2][8];
		cube[3][7]=temp[2][7];
		
		cube[6][1]=temp[6][7];
		cube[6][4]=temp[6][8];
		cube[6][7]=temp[6][9];
		cube[6][8]=temp[6][6];
		cube[6][9]=temp[6][3];
		cube[6][6]=temp[6][2];
		cube[6][3]=temp[6][1];
		cube[6][2]=temp[6][4];
	}
	void r_Di()
	{
		r_D();
		r_D();
		r_D();
	}
	void r_F()
	{
		copy(temp, cube);
		cube[1][3] = temp[1][1];
		cube[1][6] = temp[1][2];
		cube[1][9] = temp[1][3];
		cube[1][2] = temp[1][4];
		cube[1][5] = temp[1][5];
		cube[1][8] = temp[1][6];
		cube[1][1] = temp[1][7];
		cube[1][4] = temp[1][8];
		cube[1][7] = temp[1][9];
	        
		cube[4][1] = temp[5][7];
		cube[4][4] = temp[5][8];
		cube[4][7] = temp[5][9];
		cube[6][3] = temp[4][1];
		cube[6][2] = temp[4][4];
		cube[6][1] = temp[4][7];
		cube[3][3] = temp[6][1];
		cube[3][6] = temp[6][2];
		cube[3][9] = temp[6][3];
		cube[5][9] = temp[3][3];
		cube[5][8] = temp[3][6];
		cube[5][7] = temp[3][9];
	}
	void r_Fi()
	{
		r_F();
		r_F();
		r_F();
	}
	void r_U()
	{
		copy(temp, cube);
		cube[5][3] = temp[5][1];
		cube[5][6] = temp[5][2];
		cube[5][9] = temp[5][3];
		cube[5][2] = temp[5][4];
		cube[5][5] = temp[5][5];
		cube[5][8] = temp[5][6];
		cube[5][1] = temp[5][7];
		cube[5][4] = temp[5][8];
		cube[5][7] = temp[5][9];
	
		cube[1][3] = temp[4][3];
		cube[1][2] = temp[4][2];
		cube[1][1] = temp[4][1];
		cube[3][3] = temp[1][3];
		cube[3][2] = temp[1][2];
		cube[3][1] = temp[1][1];
		cube[2][3] = temp[3][3];
		cube[2][2] = temp[3][2];
		cube[2][1] = temp[3][1];
		cube[4][3] = temp[2][3];
		cube[4][2] = temp[2][2];
		cube[4][1] = temp[2][1];
	}
	void r_Ui()
	{
		r_U();
		r_U();
		r_U();
	}
	inline void r_RR()
	{
		r_R(), r_R();
	}
	inline void r_LL()
	{
		r_L(), r_L();
	}
	inline void r_BB()
	{
		r_B(), r_B();
	}
	inline void r_DD()
	{
		r_D(), r_D();
	}
	inline void r_FF()
	{
		r_F(), r_F();
	}
	inline void r_UU()
	{
		r_U(), r_U();
	}
	void rotate(int x)
	{
		if(x==0) r_R();
    		if(x==1) r_Ri();
    		if(x==2) r_RR();
    		if(x==3) r_L();
    		if(x==4) r_Li();
    		if(x==5) r_LL();
    		if(x==6) r_B();
    		if(x==7) r_Bi();
    		if(x==8) r_BB();
    		if(x==9) r_D();
    		if(x==10) r_Di();
    		if(x==11) r_DD();
    		if(x==12) r_F();
    		if(x==13) r_Fi();
    		if(x==14) r_FF();
    		if(x==15) r_U();
    		if(x==16) r_Ui();
    		if(x==17) r_UU();
	}
	void rec_prepare()
	{
		cor_rec[0][0]=make_pair(5,7);
    		cor_rec[0][1]=make_pair(1,1);
  		cor_rec[0][2]=make_pair(3,3);
    		cor_rec[1][0]=make_pair(5,9);
    		cor_rec[1][1]=make_pair(1,3);
    		cor_rec[1][2]=make_pair(4,1);
    		cor_rec[2][0]=make_pair(5,3);//
    		cor_rec[2][1]=make_pair(2,1);//
    		cor_rec[2][2]=make_pair(4,3);//
    		cor_rec[3][0]=make_pair(5,1);
    		cor_rec[3][1]=make_pair(2,3);
    		cor_rec[3][2]=make_pair(3,1);
    		cor_rec[4][0]=make_pair(6,1);
    		cor_rec[4][1]=make_pair(1,7);
    		cor_rec[4][2]=make_pair(3,9);
    		cor_rec[5][0]=make_pair(6,3);
    		cor_rec[5][1]=make_pair(1,9);
    		cor_rec[5][2]=make_pair(4,7);
    		cor_rec[6][0]=make_pair(6,9);
    		cor_rec[6][1]=make_pair(2,7);//
    		cor_rec[6][2]=make_pair(4,9);
    		cor_rec[7][0]=make_pair(6,7);
    		cor_rec[7][1]=make_pair(2,9);//
    		cor_rec[7][2]=make_pair(3,7);
    		
    		edg_rec[0][0]=make_pair(5,8);
    		edg_rec[0][1]=make_pair(1,2);
    		edg_rec[1][0]=make_pair(5,6);
    		edg_rec[1][1]=make_pair(4,2);
    		edg_rec[2][0]=make_pair(5,2);
    		edg_rec[2][1]=make_pair(2,2);//
    		edg_rec[3][0]=make_pair(5,4);
    		edg_rec[3][1]=make_pair(3,2);
    		edg_rec[4][0]=make_pair(6,2);
    		edg_rec[4][1]=make_pair(1,8);
    		edg_rec[5][0]=make_pair(6,6);//
    		edg_rec[5][1]=make_pair(4,8);
    		edg_rec[6][0]=make_pair(6,8);
    		edg_rec[6][1]=make_pair(2,8);
    		edg_rec[7][0]=make_pair(6,4);
    		edg_rec[7][1]=make_pair(3,8);
    		
    		edg_rec[8][0]=make_pair(3,6);
    		edg_rec[8][1]=make_pair(1,4);
    		edg_rec[9][0]=make_pair(4,4);
    		edg_rec[9][1]=make_pair(1,6);
    		edg_rec[10][0]=make_pair(4,6);
    		edg_rec[10][1]=make_pair(2,4);
    		edg_rec[11][0]=make_pair(3,4);
    		edg_rec[11][1]=make_pair(2,6);
	}
	void all_prepare()
	{
		all_cor[0].give(5,7,1,1,3,3);
    		all_cor[1].give(5,9,1,3,4,1);
    		all_cor[2].give(5,3,2,1,4,3);
    		all_cor[3].give(5,1,2,3,3,1);
    		all_cor[4].give(6,1,1,7,3,9);
    		all_cor[5].give(6,3,1,9,4,7);
    		all_cor[6].give(6,9,2,7,4,9);
    		all_cor[7].give(6,7,2,9,3,7);
    		
    		all_edg[0].give(5,8,1,2);
    		all_edg[1].give(5,6,4,2);
    		all_edg[2].give(5,2,2,2);
    		all_edg[3].give(5,4,3,2);
    		all_edg[4].give(6,2,1,8);
    		all_edg[5].give(6,6,4,8);
    		all_edg[6].give(6,8,2,8);
    		all_edg[7].give(6,4,3,8);
    		all_edg[8].give(1,4,3,6);
    		all_edg[9].give(1,6,4,4);
    		all_edg[10].give(4,6,2,4);
    		all_edg[11].give(2,6,3,4);
	}
	void prepare()
	{
		for (int c = 0;c <= 17;++c)
		{
			for (int i = 0;i <= 7;++i)
				corpos[i][c] = i;
			for (int i = 0;i <= 11;++i)
				edgpos[i][c] = i;
		}
		prepare_R(), prepare_Ri(), prepare_RR();
		prepare_L(), prepare_Li(), prepare_LL();
		prepare_B(), prepare_Bi(), prepare_BB();
		prepare_D(), prepare_Di(), prepare_DD();
		prepare_F(), prepare_Fi(), prepare_FF();
		prepare_U(), prepare_Ui(), prepare_UU();
		rec_prepare(), all_prepare();
		for (int c = 0;c <= 17;++c)
		{
			int input[100], output[100];
			for (int S = 0;S < power3[8];++S)
			{
				for (int i = 0;i < 8;++i)
					input[i] = (S / power3[i]) % 3;//input[i]是当前在第i个位置的方块的方向
				memset(cube, 0, sizeof(cube));
				for (int i = 0;i < 8;++i)
					cube[cor_rec[i][input[i]].first][cor_rec[i][input[i]].second] = 1;
				rotate(c);
				for (int i = 0;i < 8;++i)
				{
					for (int &d = output[i] = 0;d <= 2;++d)
						if (cube[cor_rec[i][d].first][cor_rec[i][d].second])
							break;
				}
				int &Ret(cordir[S][c] = 0);
				for (int i = 0;i < 8;++i)
					Ret += power3[i] * output[i];//output[i]是经过第c种旋转之后，当前在第i个位置的方块的方向 
			}
			for (int S = 0;S < power2[12];++S)
			{
				for (int i = 0;i < 12;++i)
					input[i] = (S / power2[i]) % 2;
				memset(cube, 0, sizeof(cube));
				for (int i = 0;i < 12;++i)
					cube[edg_rec[i][input[i]].first][edg_rec[i][input[i]].second] = 1;
				rotate(c);
				for (int i = 0;i < 12;++i)
					for (int &d = output[i] = 0;d <= 1;++d)
						if (cube[edg_rec[i][d].first][edg_rec[i][d].second])
							break;
				int &Ret(edgdir[S][c] = 0);
				for (int i = 0;i < 12;++i)
					Ret += power2[i] * output[i];
			}
		}
	}
}

int rec_trans_edgpos[20800/*12^4*/][18];

namespace Hash
{
	const int MOD((1 << 24) + 5);
	const int Max_Size(40350);
	int Head[MOD], Total, Val[Max_Size][18];
	inline int &query(int Now, int c)
	{
		int &ft = Head[Now];
		if (ft == 0)
			ft = ++Total, memset(Val[ft], -1, sizeof(Val[ft]));
		return Val[ft][c];
	}
};

void prepare_trans()
{
	memset(rec_trans_edgpos, -1, sizeof(rec_trans_edgpos));
}

inline int trans_corpos(int Now, int c)
{
	int &Ret(Hash::query(Now, c));
	if (Ret != -1)
		return Ret;
	Ret = 0;
	for (int i = 0;i < 8;++i)
		Ret += convenience::corpos[(Now / power8[i]) % 8][c] * power8[i];
	return Ret;
}

inline int trans_edgpos(int Now, int c)
{
	int &Ret(rec_trans_edgpos[Now][c]);
	if (Ret != -1)
		return Ret;
	Ret = 0;
	for (int i = 0;i < 4;++i)
		Ret += convenience::edgpos[(Now / power12[i]) % 12][c] * power12[i];
	return Ret;
}

inline int trans_cordir(int Now, int c)
{
	return convenience::cordir[Now][c];
}

inline int trans_edgdir(int Now, int c)
{
	return convenience::edgdir[Now][c];
}

struct state
{
	int corpos, toppos, botpos, midpos;//original 
	int cordir, edgdir;//current
	inline bool operator==(const state &b) const
	{
		if (corpos != b.corpos)
			return false;
		if (toppos != b.toppos)
			return false;
		if (botpos != b.botpos)
			return false;
		if (midpos != b.midpos)
			return false;
		if (cordir != b.cordir)
			return false;
		if (edgdir != b.edgdir)
			return false;
		return true;
	}
	inline void rotate(int c)
	{
		corpos = trans_corpos(corpos, c);
		toppos = trans_edgpos(toppos, c), botpos = trans_edgpos(botpos, c), midpos = trans_edgpos(midpos, c);
		cordir = trans_cordir(cordir, c);
		edgdir = trans_edgdir(edgdir, c);
	}
	inline int get_midposdir() const 
	{
		int Ret(midpos << 4);
		for (int i = 0, pos;i < 4;++i)
			pos = (midpos / power12[i]) % 12, Ret += (((edgdir >> pos) & 1) << i);
		return Ret;
	}
	inline int get_tbpos() const
	{
		int Ret(0);
		for (int i = 0;i < 4;++i)
			Ret += ((toppos / power12[i]) % 12) * power8[i];
		for (int i = 0;i < 4;++i)
			Ret += ((botpos / power12[i]) % 12) * power8[4 + i];
		return Ret;
	}
};
state Beg, End;

namespace convenience
{
	inline vector<char> original(const dim_3 &u)
	{
		vector<char> Ret;
		Ret.push_back(cube[u.x1][5]), Ret.push_back(cube[u.x2][5]), Ret.push_back(cube[u.x3][5]);
		sort(Ret.begin(), Ret.end());
		return Ret;
	}
	inline vector<char> extract(const dim_3 &u)
	{
		vector<char> Ret;
		Ret.push_back(cube[u.x1][u.y1]), Ret.push_back(cube[u.x2][u.y2]), Ret.push_back(cube[u.x3][u.y3]);
		sort(Ret.begin(), Ret.end());
		return Ret;
	}
	inline vector<char> original(const dim_2 &u)
	{
		vector<char> Ret;
		Ret.push_back(cube[u.x1][5]), Ret.push_back(cube[u.x2][5]);
		sort(Ret.begin(), Ret.end());
		return Ret;
	}
	inline vector<char> extract(const dim_2 &u)
	{
		vector<char> Ret;
		Ret.push_back(cube[u.x1][u.y1]), Ret.push_back(cube[u.x2][u.y2]);
		sort(Ret.begin(), Ret.end());
		return Ret;
	}
	inline int where(int x)
	{
		if (x == 5 || x == 6)
			return 0;//顶面或底面 
		if (x == 1 || x == 2)
			return 1;//前面或后面
		if (x == 3 || x == 4)
			return 2;//左面或右面 
	}
	char green, red, white, yellow, orange, blue;
	char cor_ref(const vector<char> &u)
	{
		if (u[0] == green || u[1] == green || u[2] == green)
			return green;
		else
			return blue;
	}
	char edg_ref(const vector<char> &u)
	{
		if (u[0] == green || u[0] == blue)
			return u[0];
		if (u[1] == green || u[1] == blue)
			return u[1];
		if (u[0] == white || u[0] == yellow)
			return u[0];
		if (u[1] == white || u[1] == yellow)
			return u[1];
	}
	void initialization()
	{
		for (int i = 1;i <= 6;++i)
			scanf("%s", cube[i] + 1);
		green = cube[5][5], red = cube[1][5], white = cube[4][5];
		yellow = cube[3][5], orange = cube[2][5], blue = cube[6][5];
		
		End.corpos = End.toppos = End.botpos = End.midpos = 0;
		for (int i = 0;i < 8;++i)
			End.corpos += i * power8[i];
		for (int i = 0;i < 4;++i)
		{
			End.toppos += i * power12[i];
			End.botpos += (4 + i) * power12[i];
			End.midpos += (8 + i) * power12[i];
		}
		End.cordir = End.edgdir = 0;
		
		Beg.corpos = Beg.toppos = Beg.botpos = Beg.midpos = 0;
		for (int i = 0, pos;i < 8;++i)
		{
			for (pos = 0;pos < 8;++pos)
				if (original(all_cor[i]) == extract(all_cor[pos]))
					break;
			Beg.corpos += pos * power8[i];
		}
		for (int i = 0, pos;i < 4;++i)
		{
			for (pos = 0;pos < 12;++pos)
				if (original(all_edg[i]) == extract(all_edg[pos]))
					break;
			Beg.toppos += pos * power12[i];
			for (pos = 0;pos < 12;++pos)
				if (original(all_edg[4 + i]) == extract(all_edg[pos]))
					break;
			Beg.botpos += pos * power12[i];
			for (pos = 0;pos < 12;++pos)
				if (original(all_edg[8 + i]) == extract(all_edg[pos]))
					break;
			Beg.midpos += pos * power12[i];
		}
		
		char reference;
		for (int i = 0, dir;i < 8;++i)
		{
			reference = cor_ref(extract(all_cor[i]));
			if (cube[all_cor[i].x1][all_cor[i].y1] == reference)
				dir = where(all_cor[i].x1);
			if (cube[all_cor[i].x2][all_cor[i].y2] == reference)
				dir = where(all_cor[i].x2);
			if (cube[all_cor[i].x3][all_cor[i].y3] == reference)
				dir = where(all_cor[i].x3);
			Beg.cordir += dir * power3[i];
		}
		for (int i = 0, dir;i < 12;++i)
		{
			reference = edg_ref(extract(all_edg[i]));
			if (cube[all_edg[i].x1][all_edg[i].y1] == reference)
				dir = where(all_edg[i].x1);
			if (cube[all_edg[i].x2][all_edg[i].y2] == reference)
				dir = where(all_edg[i].x2);
			if (i < 8)
				Beg.edgdir += (dir != 0) * power2[i];
			else
				Beg.edgdir += (dir != 2) * power2[i];
		}
	}
}

state done[100];

namespace stage_1
{
	inline bool right(const state &u)
	{
		if (u.cordir != End.cordir)
			return false;
		if (u.edgdir != End.edgdir)
			return false;
		for (int i = 0;i < 4;++i)
			if ((u.midpos / power12[i]) % 12 < 8)
				return false;
		return true;
	}
	inline int trans_midposdir(int Now, int c)
	{
		int midpos, edgdir = 0;
		midpos = (Now >> 4);
		for (int i = 0, pos;i < 4;++i)
			pos = (midpos / power12[i]) % 12, edgdir += (((Now >> i) & 1) << pos);
		midpos = trans_edgpos(midpos, c), edgdir = trans_edgdir(edgdir, c);
		int Ret(midpos << 4);
		for (int i = 0, pos;i < 4;++i)
			pos = (midpos / power12[i]) % 12, Ret += (((edgdir >> pos) & 1) << i);
		return Ret;
	}
	int dis_cordir[6600/*3^8*/], dis_edgdir[(2 << 12) + 5/*2^12*/], dis_mid[12 * 12 * 12 * 12 * (1 << 4) + (1 << 4) + 5];
	void BFS(int type)
	{
		int *Dist;
		queue<int> Q;
		if (type == 0)
			Q.push(End.cordir), (Dist = dis_cordir)[End.cordir] = 1;
		if (type == 1)
			Q.push(End.edgdir), (Dist = dis_edgdir)[End.edgdir] = 1;
		if (type == 2)
			Q.push(End.get_midposdir()), (Dist = dis_mid)[End.get_midposdir()] = 1;
		for (int u, v;Q.empty() == false;)
		{
			u = Q.front(), Q.pop();
			for (int c = 0;c <= 17;++c)
			{
				if (type == 0)
					v = trans_cordir(u, c);
				if (type == 1)
					v = trans_edgdir(u, c);
				if (type == 2)
					v = trans_midposdir(u, c);
				if (!Dist[v])
					Dist[v] = Dist[u] + 1, Q.push(v);
			}
		}
	}
	int limit;
	inline int estimate(const state &u)
	{
		return max(max(dis_cordir[u.cordir], dis_edgdir[u.edgdir]), dis_mid[u.get_midposdir()]);
	}
	bool IDA(const state &u, int now_dis)
	{
		if (now_dis + estimate(u) - 1 > limit)
			return false;
		for (int i = 1;i <= now_dis;++i)
			if (done[i] == u)
				return false;
		done[now_dis + 1] = u;
		if (right(u))
			return true;
		state v;
		for (int c = 0;c < 18;++c)
		{
			(v = u).rotate(c);
			if (IDA(v, now_dis + 1))
			{
				Answer.insert(Answer.begin(), c);
				return true;
			}
		}
		return false;
	}
	void work()
	{
		BFS(0), BFS(1), BFS(2);
		for (limit = 1;;++limit)
			if (IDA(Beg, 0))
				break;
		for (int i = 0;i < Answer.size();++i)
			Beg.rotate(Answer[i]);
	}
}

namespace stage_2
{
	int operation[6];
	void prepare_operation()
	{
		operation[0] = 16;
		operation[1] = 10;
		operation[2] = 2;
		operation[3] = 5;
		operation[4] = 14;
		operation[5] = 8;
	}
	void prepare_operation2()
	{
		operation[0] = 15;
		operation[1] = 9;
		operation[2] = 2;
		operation[3] = 5;
		operation[4] = 14;
		operation[5] = 8;
	}
	inline bool right(const state &u)
	{
		return u == End;
	}
	inline int trans_tbpos(int Now, int c)
	{
		int toppos = 0, botpos = 0;
		for (int i = 0;i < 4;++i)
			toppos += ((Now >> (3 * i)/*Now / power8[i]*/) % 8) * power12[i];
		Now >>= 12;//Now /= power8[4]
		for (int i = 0;i < 4;++i)
			botpos += ((Now >> (3 * i)/*Now / power8[i]*/) % 8) * power12[i];
		toppos = trans_edgpos(toppos, c);
		botpos = trans_edgpos(botpos, c);
		int Ret(0);
		for (int i = 0;i < 4;++i)
			Ret += ((toppos / power12[i]) % 12) * power8[i];
		for (int i = 0;i < 4;++i)
			Ret += ((botpos / power12[i]) % 12) * power8[4 + i];
		return Ret;
	}
	int dis_corpos[16777216 + 5/*8^8*/], dis_tbpos[16777216 + 5/*8^8*/], dis_midpos[21000 + 5/*12^4*/];
	void BFS(int type)
	{
		int *Dist;
		queue<int> Q;
		if (type == 0)
			Q.push(End.corpos), (Dist = dis_corpos)[End.corpos] = 1;
		if (type == 1)
			Q.push(End.get_tbpos()), (Dist = dis_tbpos)[End.get_tbpos()] = 1;
		if (type == 2)
			Q.push(End.midpos), (Dist = dis_midpos)[End.midpos] = 1;
		for (int u, v;Q.empty() == false;)
		{
			u = Q.front(), Q.pop();
			for (int i = 0, c;i < 6;++i)
			{
				c = operation[i];
				if (type == 0)
					v = trans_corpos(u, c);
				if (type == 1)
					v = trans_tbpos(u, c);
				if (type == 2)
					v = trans_edgpos(u, c);
				if (!Dist[v])
					Dist[v] = Dist[u] + 1, Q.push(v);
			}
		}
	}
	int limit;
	inline int estimate(const state &u)
	{
		return max(max(dis_corpos[u.corpos], dis_tbpos[u.get_tbpos()]), dis_midpos[u.midpos]);
	}
	bool IDA(const state &u, int now_dis)
	{
		if (now_dis + estimate(u) - 1 > limit)
			return false;
		for (int i = 1;i <= now_dis;++i)
			if (done[i] == u)
				return false;
		done[now_dis + 1] = u;
		if (right(u))
			return true;
		state v;
		for (int i = 0, c;i < 6;++i)
		{
			(v = u).rotate(c = operation[i]);
			if (IDA(v, now_dis + 1))
			{
				Answer2.insert(Answer2.begin(), c);
				return true;
			}
		}
		return false;
	}
	void work()
	{
		prepare_operation(), BFS(0), BFS(1), BFS(2);
		prepare_operation2();
		for (limit = 1;;++limit)
			if (IDA(Beg, 0))
				break;
	}
}

int main()
{
	prepare_power();
	prepare_trans();
	convenience::prepare(), convenience::initialization();
	stage_1::work(), stage_2::work(), print_Answer();
	return 0;
}

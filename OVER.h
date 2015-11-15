	
	/*Copyright (C) <2015>  <DEVELOPER:SUMEDH KALE>

	This file is part of CRIC ADDICT.

	CRIC ADDICT is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
        (at our option) any later version.

        CRIC ADDICT is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with CRIC ADDICT.  If not, see <http://www.gnu.org/licenses/>.
	
        For more details contact SUMEDH KALE<kaless14.comp@coep.ac.in>
	*/
#define max 15
typedef struct best{
	float rg;
	int w;
}best;
typedef struct overs{
	int on;
	char ov[max];
}overs;
typedef struct over{
	int n;
	char oveR[max];
	int dcount;//number of dot ballss
	int bcount; 
}OVER;
typedef struct teamscore{
	int total;//team total
	int wickets;//team wickets
	float crr;//current runrate
	int projscr;//projected score of team
	int target;//target for team to win
	int win;//tells if a team wins
}teamscr;
typedef struct batsman{
	int bpos;//batting position
	int ho;//how is batsman out
	int onstrike;//is batsman onstrike
	float b;//balls played
	float r, high;//runs made, highest(best score)
	int fours,sixes;//fours, sixes 
	float SR;//strike rate
}batsman; 
typedef struct bowler{
	best best;//best bowling figures
	int bfl;
	float ovrc, rg;//overs bowled, runs given
	int m, w;//maidens,wicktes
	float eco;//economy
	float ballc;
}bowler;
typedef struct extras{
	int wides, noballs; // extras conceded: wides and noballs
}extras;
typedef struct partnership{
	int scr;//partnership score
	int balls;//number of balls from which partnership score is made
}pship;
typedef struct swap{
	int p1, p2;
}swap1;


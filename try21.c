 
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
#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include"1list.h"
#include<time.h>
#include<math.h>
overs *OA, *OB;
swap1 wswapA[16];
swap1 wswapB[16];
node *SA, *SB, *SS;
int sA = 0, sB = 0;
WINDOW *tabasc, *tbbasc, *tabosc, *tbbosc, *ho, *win1,*obo;
int totalballs;
float rrb = 0.0;
int teamtotal, target;
int alldown = 0;
int ingf = 0;
int bwlpos = 0, btcount = 2, batpos = 1;
WINDOW *wteamA, *wteamB, *owps, *scrbrd;
WINDOW *scr, *batm, *bowl, *curr, *extra, *thisover, *prevover, *pf, *stat;
FILE *fpA, *fpB, *teamd;
int tsA, tsB, token = 0, stadium, tossr;
WINDOW *tsquadA, *tsquadB;
WINDOW *win;
list l1, l2, lL1, lL2;
time_t tt;
list *lA, *lB;
int tok = 0;
node *p11, *q11, *bata, *batb, *bowller;
int ovc = 0;
float over = 0.0;
OVER *ovr;
teamscr *tscrA, *tscrB;
pship *psA, *psB;
extras *exA, *exB;
int res = 1, choice1, tossflag = 0, tAbat, tBbat, ser = 1;
node *q, *r, *t, *bttm, *bltm;
char ball_input;
int overno = 0;
int batflag = 0;
char *oveR;
int z;
profile *pr;

void initov(overs *p, int n){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < 12; j++){
			p[i].ov[j] = -1;
		}
	}
}

//INIALIZE FUCNTION, TO SWAP BACK PLAYERS WHO ARE ALREADY SWAPPED
void initswap(swap1 a[16]){
	int i;
	for(i = 0 ;i < 16; i++)
		a[i].p1 = a[i].p2 = -1; 
}

//FUNTION FOR INITIALISING A LIST
void init(list *l) {
	l->head = l->tail = NULL;
	l->length = 0;
}
	
//FUCTION FOR INSERTING IN A LIST
void insert(list *l, char *Attr, char *Fname, char *Lname, int pos) {
	node *p, *tmp;
	int i;
	if(pos < 0 || pos > l->length)		
		return;
	tmp = (node *)malloc(sizeof(node));
	if(tmp == NULL){
		wprintw(win, "!!! MALLLOC FAILED !!!\n");
		wrefresh(win);
		napms(1000);
		endwin();
		exit(0);
	}
	strcpy(tmp->attr, Attr);
	strcpy(tmp->fname, Fname);
	strcpy(tmp->lname, Lname);
	tmp->prof = (profile *)malloc(sizeof(struct info));
	if(tmp->prof == NULL){
		wprintw(win, "!!! MALLLOC FAILED !!!\n");
		wrefresh(win);
		napms(1000);
		exit(0);
	}
	tmp->prof->matches = 1;
	tmp->prof->batsmn.r = tmp->prof->batsmn.b = tmp->prof->batsmn.fours = tmp->prof->batsmn.sixes = 0;
	tmp->prof->batsmn.high = 0.0;
	tmp->prof->batsmn.SR = 0.0;
	tmp->prof->bowlr.m = tmp->prof->bowlr.w = tmp->prof->bowlr.bfl = 0;
	tmp->prof->bowlr.rg = tmp->prof->bowlr.eco = tmp->prof->bowlr.ovrc = 0.0;
	tmp->prof->bowlr.best.rg = tmp->prof->bowlr.ballc = 0.0;
	tmp->prof->bowlr.best.w = 0;	
	tmp->next = tmp->prev = NULL;
	if(l->head == NULL) {
		tmp->prof->batsmn.bpos = 0;
		l->head = l->tail = tmp;
		l->length = 1;
		return;
	}
	tmp->prof->batsmn.bpos = pos;
	p = l->head;
	for(i = 0; i < pos - 1; i++)
		p = p->next;
	tmp->prev = p;	
	p->next = tmp;
	if(pos == l->length)
		l->tail = tmp;
	(l->length)++;
}

//FUNCITON FOR TRAVERSING A LIST
void traverse1(list *l){
	lB = l;	
	node *p = l->head;
	int i = 1;
	while(p!=NULL) {
		wprintw(win, "%d. %s %s %s\n", i++, p->attr, p->fname, p->lname);
		wrefresh(win);
		p = p->next;
	}
	wrefresh(win);
	
}
void traverse(list *L) {
	node *p = L->head;
	if(token == 0)
		lA = L;
	int i = 1;
	while(p!=NULL) {
		wprintw(win, "%d. %s %s %s\n", i++, p->attr, p->fname, p->lname);
		wrefresh(win);
		p = p->next;
	}
	wrefresh(win);
	
}

//FUNCTION FOR PRINTING OPTIONS	
int printmenu() {
	int choice;
	mvwprintw(win, 23, 0, "1. SWAP (PRESS 1) \t"\
			      "2. SAVE AND PROCEED (PRESS 2)\n");
	mvwprintw(win, 25, 0, "ENTER THE 'OPTION NUMBER' AND PRESS 'ENTER'");	
	mvwprintw(win, 27, 0, "YOUR CHOICE: ");
	wrefresh(win);
	wscanw(win, "%d", &choice);		
	return choice;
}

//FUNCTONS FOR SWAPPING NODES IN A LIST
void swap(list *l, int posi, int posj){
	node *p, *q;
	profile *ptmp;
	node *tmp;
	int i, j;
	tmp = (node *)malloc(sizeof(struct node));
	if(tmp == NULL){
		wprintw(win, "!!! MALLLOC FAILED !!!\n");
		wrefresh(win);
		napms(1000);
		endwin();
		exit(0);
	}
	tmp->prof = NULL;
	p = q = l->head;
	for(i = 0; i < (posi - 1); i++)
		p = p->next;
	for(j = 0; j < (posj - 1); j++)
		q = q->next;
	if(p == q)
		return;
	strcpy(tmp->attr, p->attr);
	strcpy(tmp->fname, p->fname);
	strcpy(tmp->lname, p->lname);
	strcpy(p->attr, q->attr);
	strcpy(p->fname, q->fname);
	strcpy(p->lname, q->lname);
	strcpy(q->attr, tmp->attr);
	strcpy(q->fname, tmp->fname);
	strcpy(q->lname, tmp->lname);
	ptmp = p->prof;
	p->prof = q->prof;
	q->prof = ptmp;
	free(tmp);
}

//NEXT THREE FUNCTIONS NAMELY 'drawmenu','screen1' and 'sc1' WORK TOGETHER FOR DISPLAYING AND SCROLLING A MENU 
void drawmenu(int item, int label){	
	int c, MENUMAX;
	if( label == 1){
		MENUMAX = 3;
		char menu[3][21] = {		
			"PLAY", 
			"PLAYER PROFILES", 
			"EXIT"
			};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				attron(A_REVERSE);	
			mvaddstr(15 + (c * 2), 50, menu[c]);
			attroff(A_REVERSE);		
		}
		attron(A_BOLD | A_UNDERLINE);
		mvaddstr(10, 45, "CRIC ADDICT");
		attroff(A_BOLD | A_UNDERLINE);
		mvaddstr(25, 28, "> USE 'ARROW' KEYS TO MOVE 'UP' AND 'DOWN'.\n\n\t\t\t    > PRESS 'Enter' TO SELECT DESIRED OPTION");
		curs_set(0);
		refresh();
	} 
	if(label == 2){
		MENUMAX = 8;
		char menu[8][30]={
			"INDIA", 
			"SOUTH AFRICA", 
			"AUSTRALIA", 
			"ENGLAND", 
			"WESTINDIES", 
			"NEWZEALAND", 
			"PAKISTAN", 
			"SRILANKA"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(wteamA, A_REVERSE);	
			mvwaddstr(wteamA, 4 + (c * 2), 3, menu[c]);
			wattroff(wteamA, A_REVERSE);		
		}
		wrefresh(wteamA);
	}
	if(label == 3){
		MENUMAX = 8;
		char menu[8][30]={
			"INDIA", 
			"SOUTH AFRICA", 
			"AUSTRALIA", 
			"ENGLAND", 
			"WESTINDIES", 
			"NEWZEALAND", 
			"PAKISTAN", 
			"SRILANKA"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(wteamB, A_REVERSE);	
			mvwaddstr(wteamB, 4 + (c * 2), 3, menu[c]);
			wattroff(wteamB, A_REVERSE);		
		}
	wrefresh(wteamB);
	}
	if(label == 4){
		MENUMAX = 2;
		char menu[2][10]={
			"DAY", 
			"NIGHT"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 2 + c, 0, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 5){
		MENUMAX = 4;
		char menu[4][12]={
			"WARM", 
			"COOL", 
			"HUMID", 
			"OVERCAST"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 7 + c, 0, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 6){
		MENUMAX = 8;
		char menu[8][80]={
			"Melbourne Cricket Ground, MELBOURNE, AUSTRALIA", 
			"Lord's Cricket Ground, LONDON, ENGLAND", 
			"Eden Park, AUCKLAND, NEWZEALAND", 
			"Feroz Shah Kotla Ground, NEWDELHI, INDIA", 
			"New Wanderers Stadium, JOHANNESBURG, SOUTH AFRICA", 
			"Kensington Oval, BRIDGETOWN, WESTINDIES", 
			"Pallekele International Stadium, SRILANKA ", 
			"National Stadium, KARACHI, PAKISTAN", };
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 2 + c, 80, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 7){
		MENUMAX = 7;
		char menu[7][10]={
			"NORMAL", 
			"HARD", 
			"DRY", 
			"DAMP", 
			"GREEN", 
			"DUSTY", 
			"WET"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 14 + c, 0, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 8){
		MENUMAX = 2;
		char menu[2][8]={
			"HEADS", 
			"TAILS"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 12 + c, 72, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 9){
		MENUMAX = 2;
		char menu[2][8]={
			"BATTING", 
			"BOWLING"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(owps, A_REVERSE);	
			mvwaddstr(owps, 21 + c, 72, menu[c]);
			wattroff(owps, A_REVERSE);
		}
	wrefresh(owps);
	}
	if(label == 10){
		MENUMAX = 4;
		char menu[4][22]={
			"CAUGHT AND BOWLED", 
			"CATCH OUT", 
			"BOWLED", 
			"RUNOUT"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(ho, A_REVERSE);	
			mvwaddstr(ho, 5 + c, 0, menu[c]);
			wattroff(ho, A_REVERSE);
		}
	wrefresh(ho);
	}
	if(label == 11){
		MENUMAX = 8;
		char menu[8][30]={
			"INDIA", 
			"SOUTH AFRICA", 
			"AUSTRALIA", 
			"ENGLAND", 
			"WESTINDIES", 
			"NEWZEALAND", 
			"PAKISTAN", 
			"SRILANKA"};
		for(c = 0; c < MENUMAX; c++){
			if(c == item)
				wattron(pf, A_REVERSE);	
			mvwaddstr(pf, 4 + (c * 2), 3, menu[c]);
			wattroff(pf, A_REVERSE);		
		}
	wrefresh(pf);
	}
	
	
}
int sc1(int label){
	int key, menuitem = 0, MENUMAX;
	keypad(stdscr, TRUE);
	noecho();
	if(label == 1)
		MENUMAX = 3;
	if((label == 2) || (label == 3) )
		MENUMAX = 8;
	if(label == 4)
		MENUMAX = 2;
	if(label == 5)
		MENUMAX = 4;
	if(label == 6)
		MENUMAX = 8;
	if(label == 7)
		MENUMAX = 7;
	if(label == 8)
		MENUMAX = 2;
	if(label == 9)
		MENUMAX = 2;
	if(label == 10)
		MENUMAX = 4;
	if(label == 11)
		MENUMAX = 8;
	do
	{
		key = getch();
		switch(key){
			case KEY_DOWN:
				menuitem++;
				if(menuitem > MENUMAX-1) 
					menuitem = 0;
				break;
			case KEY_UP:
				menuitem--;
				if(menuitem < 0) 
					menuitem = MENUMAX - 1;
				break;
			default:
				break;
		}
		drawmenu(menuitem, label);
		refresh();
	} while(key != '\n');
	echo();		
	return menuitem;
}
void screen(int label){
	int menuitem = 0;
	drawmenu(menuitem, label);
}

//FUNCTION FOR SELECTING TEAMS
int teamselect(int tlabel){
	int halfx, x, y, k;
	char teamA[] ="SELECT TEAM A";
	char teamB[] ="SELECT TEAM B";
	getmaxyx(stdscr, y, x);
	halfx = x / 2;
	if(tlabel == 2){
		wteamA = newwin(y - 1, halfx, 1, 0);
		mvwaddstr(wteamA, 2, (halfx - strlen(teamA)) / 2, teamA);
		mvwaddstr(wteamA,35, 0, "> USE 'ARROW' KEYS TO MOVE 'UP' AND 'DOWN'\n> PRESS 'Enter' TO SELECT DESIRED OPTION");
		screen(2);
		k = sc1(2);
		wrefresh(wteamA);
		//return i;
	}
	if(tlabel == 3){
		wteamB = newwin(y - 1, halfx, 1, halfx);
		mvwaddstr(wteamB, 2, (halfx - strlen(teamB)) / 2, teamB);
		mvwaddstr(wteamB, 35, 0, "> USE 'ARROW' KEYS TO MOVE 'UP' AND 'DOWN'\n> PRESS 'Enter' TO SELECT DESIRED OPTION");
		screen(3);
		k = sc1(3);
		wrefresh(wteamB);
		//return j;
	}
	return k;	
}

//FUCNTION TO PRINT A ERROR MESSAGE IF TEAM SELECTED ARE SAME AND MAKE USER TO SELECT TEAMS AGAIN
void errorwin(){
	WINDOW *error;
	error = subwin(stdscr, 4, 50, 27, 0);
	if(error == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	wattron(error,A_BOLD);
	wprintw(error, "YOU ARE A TRUE CRICKET FAN :P\nLOL ! HOW CAN SAME TEAMS PLAY A MATCH?\nPLEASE SELECT TEAMS AGAIN!");
	wattroff(error,A_BOLD);
	wrefresh(error);
	napms(8000);
	delwin(error);
}

//IF A PLAYER IS NOT IN TOP 11 OUT OF 15 PLAYERS IN A TEAM SQUAD, THEN DONT SELECT HIM FOR A MATCH
void makematcheszero(list *l){
	node *d;
	d = l->head;
	int i = 1;
	while(i <= 11){
		d = d->next;
		i++;
	}
	while(d){
		d->prof->matches = 0;
		d = d->next;
	}
}

//MAKE SQUAD AFTER THE USER SELECTS DESIRED TEAMS
void makesquad(char *fname, char *tn){
	if(token == 1)
		win = tsquadB = newwin(0, 0, 0, 0);
	else 
		win = tsquadA = newwin(0, 0, 0, 0);
	if(win == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	node pl;
	int i, choice, inpos, posi, posj;
	inpos = 0;
	fpA = fopen(fname, "r");
	if(token == 0){
		init(&l1);
		l1.initlen = 15;
		for(i = 0;i < (l1.initlen);i++){
			fscanf(fpA, "%s %s %s", pl.attr, pl.fname, pl.lname);
			insert(&l1, pl.attr, pl.fname, pl.lname, inpos);
			inpos++;				
		}
	}
	if(token == 1){
		init(&l2);
		l2.initlen = 15;
		for(i = 0;i < (l2.initlen);i++){
			fscanf(fpA, "%s %s %s", pl.attr, pl.fname, pl.lname);
			insert(&l2, pl.attr, pl.fname, pl.lname, inpos);
			inpos++;				
		}
	}
	while(1) {
		wprintw(win, "TEAM '%s' SQUAD\n\n", tn);
		wprintw(win, "BAT:BATSMEN   \t"\
				"ALL:ALL-ROUNDER \t"\
				"FBOW:FAST-BOWLER\t"\
				"SBOW:SPIN-BOWLER\n\n");
		wattron(win, A_BOLD);
		wprintw(win,"NOTE: TOP ELEVEN PLAYERS WILL BE SELECTED FOR THE MATCH\n\n");
		wattroff(win, A_BOLD);
		wrefresh(win);
		if(token == 0)
			traverse(&l1);
		if(token == 1)
			traverse1(&l2);
		wrefresh(win);		
		choice = printmenu();
		switch(choice) {
			case 1: //SWAP 
				wprintw(win, "\nENTER THE POSITION NUMBERS OF PLAYERS YOU WANT TO SWAP SEPARATED BY SPACE and PRESS ENTER\n");
				wrefresh(win);
				wscanw(win, "%d %d", &posi, &posj);
				if(token == 0){
					wswapA[sA].p1 = posi;
					wswapA[sA].p2 = posj;
					sA++;
				}
				if(token == 1){
					wswapB[sB].p1 = posi;
					wswapB[sB].p2 = posj;
					sB++; 
				}
				if(token == 0)
					swap(&l1, posi, posj);
				else
					swap(&l2, posi, posj);
				wclear(win);
				break;
			case 2: //FINALISE TOP ELEVEN OF SQUAD
				break;
			default:
				break;
					
		}
		if(choice == 2){
			if(token == 0)
				makematcheszero(&l1);
			else
				makematcheszero(&l2);
			break;
		}
		else
			continue;

		
	}
}

//FUNCTION TO MAKE THE PLAYER PROFILES AND EDIT IT ON THE REQUIRED FILE ( MAKE DATABASE )
node *makestatsl(char *r, node *S, list *l, int key){
	node *p;	
	if(key == 0){
		p = l->head;
	}
	WINDOW *tmp = newwin(0, 0, 0, 0);
	if(tmp == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	teamd = fopen(r, "r");
	S = (node *)malloc(sizeof(struct node) * 16);
	if(S == NULL){
		printw( "!!! MALLLOC FAILED !!!\n");
		refresh();
		napms(1000);
		endwin();
		exit(0);
	}
	int i = 0;
	for(i = 0; i < 15; i++){
		S[i].prof = (profile *)malloc(sizeof(struct info));
		if(S[i].prof == NULL){
			printw( "!!! MALLLOC FAILED !!!\n");
			refresh();
			napms(1000);
			endwin();			
			exit(0);
		}
		S[i].next = S[i].prev = NULL;
		fscanf(teamd, "%s %s %s %d %f %f %f %d %d %f %f %f %d %d %f %f %d", S[i].attr, S[i].fname, S[i].lname, &S[i].prof->matches, &S[i].prof->batsmn.r, &S[i].prof->batsmn.b, &S[i].prof->batsmn.high, &S[i].prof->batsmn.fours, &S[i].prof->batsmn.sixes, &S[i].prof->batsmn.SR, &S[i].prof->bowlr.ovrc, &S[i].prof->bowlr.rg, &S[i].prof->bowlr.m, &S[i].prof->bowlr.w, &S[i].prof->bowlr.eco,&S[i].prof->bowlr.best.rg, &S[i].prof->bowlr.best.w);
		if(key == 0){
			S[i].prof->matches += p->prof->matches;
			S[i].prof->batsmn.r += p->prof->batsmn.r;
			S[i].prof->batsmn.b += p->prof->batsmn.b;
			if(S[i].prof->batsmn.high < p->prof->batsmn.r)
				S[i].prof->batsmn.high = p->prof->batsmn.r;
			S[i].prof->batsmn.fours += p->prof->batsmn.fours;
			S[i].prof->batsmn.sixes += p->prof->batsmn.sixes;
			if(S[i].prof->batsmn.b != 0)		
				S[i].prof->batsmn.SR = (S[i].prof->batsmn.r / S[i].prof->batsmn.b)*100;
			S[i].prof->bowlr.ovrc += p->prof->bowlr.ovrc;
			S[i].prof->bowlr.rg += p->prof->bowlr.rg;
			S[i].prof->bowlr.m += p->prof->bowlr.m;
			S[i].prof->bowlr.w += p->prof->bowlr.w;
			if(p->prof->bowlr.ovrc > 0.00){				
				S[i].prof->bowlr.eco = (S[i].prof->bowlr.rg / S[i].prof->bowlr.ovrc);
				if(S[i].prof->bowlr.best.w <= p->prof->bowlr.w){
					if(S[i].prof->bowlr.best.w == p->prof->bowlr.w){
						if(S[i].prof->bowlr.best.rg > p->prof->bowlr.rg)
							S[i].prof->bowlr.best.rg = p->prof->bowlr.rg;
					}
					else{
						S[i].prof->bowlr.best.w = p->prof->bowlr.w;
						S[i].prof->bowlr.best.rg = p->prof->bowlr.rg;
					}
				}
			}
			p = p->next;
		}
	}
	if(key == 0){
		freopen(r, "w", teamd);
		for(i = 0 ;i < 15; i++){
			fprintf(teamd, "%s %s %s %d %.0f %.0f %.0f %d %d %.2f %.1f %.0f %d %d %.2f %.0f %d\n", S[i].attr, S[i].fname, S[i].lname, S[i].prof->matches, S[i].prof->batsmn.r, S[i].prof->batsmn.b, S[i].prof->batsmn.high, S[i].prof->batsmn.fours, S[i].prof->batsmn.sixes, S[i].prof->batsmn.SR, S[i].prof->bowlr.ovrc, S[i].prof->bowlr.rg, S[i].prof->bowlr.m, S[i].prof->bowlr.w, S[i].prof->bowlr.eco, S[i].prof->bowlr.best.rg, S[i].prof->bowlr.best.w);
		}
	}
	getch();
	fclose(teamd);
	return S;
}

//FUCTION WHICH RETURNS REQUIRED FILE NAME AFTER USER SELECTS HIS DESIRED TEAMS
char *TEAMname(int a, int b){
	char *r;	
	char *teamname;
	switch(a){
		case 0: teamname = "team_ind.txt";
			r = "INDIA";
			break;
		case 1: teamname = "team_rsa.txt";
			r = "SOUTH AFRICA";
			break;
		case 2:	teamname = "team_aus.txt";
			r = "AUSTRALIA";
			break;
		case 3: teamname = "team_eng.txt";
			r = "ENGLAND";
			break;
		case 4: teamname = "team_wind.txt";
			r = "WEST INDIES";
			break;
		case 5: teamname = "team_nzl.txt";
			r = "NEWZEALAND";
			break;
		case 6: teamname = "team_pak.txt";
			r = "PAKISTAN";
			break;
		case 7: teamname = "team_sl.txt";
			r = "SRILANKA";
			break;
		default:
			break;
	}
	if(b == 0)
		makesquad(teamname, r);
	return teamname;	 
}

//FUNCTION FOR SELECTING 'WEATHER CONDITIONS, PITCH TYPE, VENUE(STADUIM) AT WHICH MATCH IS TO BE PLAYED AND NUMBER FOR OVERS'. IT ALSO DISPLAYS 'GUIDELINES FOR THE TOSS ACCODING TO THE WEATHR CONDITIONS AND PITCH TYPE SELECTION BY USER'
void owpsf(){
	owps = newwin(0, 0, 0, 0);
	if(owps == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		napms(1000);
		endwin();
		exit(0);
	}
	ovr = (OVER *)malloc(sizeof(struct over));
	oveR = ovr->oveR;
	if(ovr == NULL){
		wprintw(owps, "!!! MALLLOC FAILED !!!\n");
		wrefresh(owps);
		napms(1000);
		endwin();
		exit(0);
	}
	int l, m;
	mvaddstr(38, 70, "> USE 'ARROW' KEYS TO MOVE 'UP' AND 'DOWN'.\n\t\t\t\t\t\t\t\t\t> PRESS 'Enter' TO SELECT THE DESIRED OPTION");
	wprintw(owps, "SELECT TIME OF DAY:");
	screen(4);
	sc1(4);
	wprintw(owps, "\n\nSELECT WEATHER CONDITIONS :");
	screen(5);
	l = sc1(5);
	mvwprintw(owps, 0, 80, "SELECT VENUE (STADIUM) :");
	screen(6);
	stadium = sc1(6);	
	mvwprintw(owps, 12, 0,"SELECT PITCH TYPE :");
	screen(7);
	m = sc1(7);
	mvwprintw(owps, 20, 40, "ENTER THE NUMBER OF OVERS : ");
	curs_set(1);
	wscanw(owps, "%d", &ovr->n);
	curs_set(0);
	totalballs = ovr->n * 6;
	OA = (overs *)malloc(sizeof(struct overs) * ovr->n);
	OB = (overs *)malloc(sizeof(struct overs) * ovr->n);
	if(OA == NULL || OB == NULL){
		wprintw(owps, "!!! MALLLOC FAILED !!!\n");
		wrefresh(owps);
		napms(1000);
		endwin();
		exit(0);
	}
	initov(OA,ovr->n);
	initov(OB,ovr->n);
	wmove(owps, 30, 0);
	wattron(owps, A_UNDERLINE | A_BOLD);
	wprintw(owps, "GUIDELINES FOR THE TOSS :\n");
	wattroff(owps, A_UNDERLINE | A_BOLD);
	switch(l){
		case 3:
			if((m == 0)||(m == 1)||(m == 2)||(m == 5))
				wprintw(owps, "It's OVERCAST!The WICKET looks a good BATTING pitch.\nThe ball will come on to bat nicely.\nSo it's time to get some runs on the board!");
			if(m == 3)
				wprintw(owps, "It's OVERCAST!The pitch is DAMP.\nAn ODD ball might bounce as well, and hence BOWLERS can extract something out of this pitch.\nYou may opt to BOWL first!"); 
			if(m == 4)				
				wprintw(owps, "It's OVERCAST & a GREEN pitch !\nIt's a heaven for BOWLERS!The ball will swing and nip around.\nif you win the toss, BOWL first!");
			if(m == 6)
				wprintw(owps, "It's OVERCAST! The pitch is WET.\nThe ball will stay LOW, and hence the pitch will helps BOWLERS.\nYou may opt to BOWL first!");
			break;
		case 0: case 1:
			if((m == 0)||(m == 1)||(m == 2)||(m == 5))
				wprintw(owps, "The WICKET looks a good bating pitch.\nThe ball will come on to the bat nicely.\nSo it's time to get some runs on the board!");
			if((m == 3)||(m == 4)||(m == 6))
				wprintw(owps, "The ball might not bounce as expected.\nThe BOWLERS can surely extract something out of the pitch.\nSo give them a chance first!"); 
			break;
		case 2:
			if((m == 0)||(m == 1)||(m == 2)||(m == 5))
    				wprintw(owps, "It's HUMID! The deck looks a good bating pitch.\nThe ball will come on to bat nicely.\nSo it's time to get some runs on the board !");
			if((m == 3)||(m == 4)||(m == 6))
				wprintw(owps, "It's HUMID! The ball might just hold back after bouncing from the deck.\nThe pitch type will assist BOWLERS.\nSo you may give them a chance first !"); 
			break;
		default:
			break;
	}
	wattron(owps,A_REVERSE);
	mvwaddstr(owps, 38, 110,"> PRESS ENTER TO PROCEED\n");
	wattroff(owps,A_REVERSE);
	touchline(owps,38,2);
	wrefresh(owps);
	napms(750);
	getch();
}

//FUNCTION FOR DISPLAYING 'TEAM NAME' WHEREVER REQUIRED AND ALSO TO OPEN DESIRED 'TEAM'S DATABASE FILE'
char *whichteam(int a, int b){
	char *r;
	switch(a){
		case 0: if(b == 0)
				r = "INDIA";
			else
				r = "Steam_ind.txt";
			break; 
		case 1: if(b == 0)
				r = "SOUTH AFRICA";
			else
				r = "Steam_rsa.txt";
			break;
		case 2: if(b == 0)
				r = "AUSTRALIA";
			else
				r = "Steam_aus.txt";
			break;
		case 3: if(b == 0)
				r = "ENGLAND";
			else
				r = "Steam_eng.txt";
			break;
		case 4: if(b == 0)
				r = "WEST INDIES";
			else
				r = "Steam_wind.txt";
			break;
		case 5: if(b == 0)
				r = "NEWZEALAND";
			else
				r = "Steam_nzl.txt";
			break;
		case 6: if(b == 0)	
				r = "PAKISTAN";
			else
				r = "Steam_pak.txt";
			break;
		case 7: if(b == 0)
				r = "SRILANKA";
			else
				r = "Steam_sl.txt";
			break;
		default: 
			break;	
	}
	return r;
}

/*FUNCTION FOR DISPLAYING TOSS AND ITS OUTCOME. 
USES RANDOM FUNCTION FOR GENERATING RANDOM OUTCOMES OF TOSS*/
void toss(){
	long int k, l;
	int y, x, tossr, i;
	wclear(owps);
	mvaddstr(38, 20, "> USE 'ARROW' KEYS TO MOVE 'UP' AND 'DOWN'.\n\t\t    > PRESS 'Enter' TO SELECT THE DESIRED OPTION");
	getmaxyx(owps, y, x);
	wattron(owps, A_BOLD);
	mvwprintw(owps, 4, 55, "'%s' vs '%s'", whichteam(tsA, 0), whichteam(tsB, 0));
	mvwaddstr(owps, (y/2) - 10, (x/2) - 2, " 'TOSS' ");
	mvwprintw(owps, (y/2) - 8, (x/2) - 25, "'%s' TO CHOOSE:", whichteam(tsA, 0));
	wattroff(owps, A_BOLD);
	screen(8);
	tossr = sc1(8);
	srandom(time(0));
	k = random();
	if((k % 2) == 0)
		res = 0;
	for(i = 0;i < 5;i++){
		wmove(owps, 16, 69 + i);
		wprintw(owps, ".");
		wrefresh(owps);
		napms(250);
	}
	wdeleteln(owps);
	wrefresh(owps);
	if( tossr == res ){// USER WINS THE TOSS 
		wattron(owps, A_BOLD);
		if(res == 0)
			mvwprintw(owps, 17, 47, "COIN TOSS RESULT:\t HEADS");
		else
			mvwprintw(owps, 17, 47, "COIN TOSS RESULT:\t TAILS");
		wattroff(owps, A_BOLD);
		mvwprintw(owps, 19, 61, "%s 'WON' THE TOSS.", whichteam(tsA, 0));
		mvwprintw(owps, 21, 55, "PLEASE CHOOSE:");
		screen(9);
		choice1 = sc1(9);
		if(choice1 == 0){
			tAbat = 1;
			tBbat = 0;
		}
		else{
			tBbat = 1;
			tAbat = 0;
		}			
		tossflag = 1;
		wrefresh(owps);	
	}
	else{ //USER LOSSES THE TOSS
		wattron(owps, A_BOLD);
		if(res == 0)
			mvwprintw(owps, 17, 47, "COIN TOSS RESULT :\tHEADS");
		else
			mvwprintw(owps, 17, 47, "COIN TOSS RESULT :\tTAILS");
		wattroff(owps, A_BOLD);
		mvwprintw(owps, 19, 55, "%s 'LOST' THE TOSS.", whichteam(tsA, 0));
		srandom(time(&tt));
		l = random();
		if((l % 2) == 0)
			ser = 0;

		//THE USER LOSES TOSS AND IT'S RANDOMLY DECIDED WHETHER OPPONENT TEAM WILL BOWL OR BAT
		if(ser == 0){
			mvwprintw(owps, 21, 53, "'%s' CHOSE TO 'BOWL' FIRST.", whichteam(tsB, 0));
			tAbat = 1;
			tBbat = 0;
		}
		else{
			mvwprintw(owps, 21, 53, "'%s' CHOSE TO 'BAT' FIRST.", whichteam(tsB, 0));
			tBbat = 1;
			tAbat = 0;	
		} 
		wrefresh(owps);
	}
	wattron(owps,A_REVERSE);
	mvwaddstr(owps,39,110,"> PRESS 'ENTER' TO PROCEED\n");
	wattroff(owps,A_REVERSE);
	wrefresh(owps);
	getch();
}

//FUNCTION TO GET NEXT BATSMAN ON STRIKE WHEN PREVIOUS BATSMAN IS OUT
node *getbatsman(int pos){
	batpos++;
	profile *ptmp;
	node *tmp;
	ptmp = t->prof;
	ptmp->batsmn.bpos = batpos;
	mvwprintw(batm, 2 + pos, 2, "%s %s %s", t->attr, t->fname, t->lname);
	mvwprintw(batm, 2 + pos, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", ptmp->batsmn.r, ptmp->batsmn.b, ptmp->batsmn.fours, ptmp->batsmn.sixes, ptmp->batsmn.SR);
	tmp = t;
	t = t->next;
	touchline(batm, 2 + pos, 1);
	touchwin(batm);
	wrefresh(batm);
	return tmp;
}

//FUCNTION TO SELECT BOWLER TO BOWL THE NEXT OVER	
node *getbowler(){
	node *tmp;
	WINDOW *getbwl = newwin(0, 0, 0, 0);
	if(getbwl == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	int i = 0, k = 1;
	node *f = bltm;
	while(i<11){
		wprintw(getbwl, "%d. %s %s %s\n", i+1, f->attr, f->fname, f->lname);
		wrefresh(getbwl);	
		f = f->next;
		i++;
	}
	echo();
	int pos;
	wprintw(getbwl, "\nENTER THE 'POSITION NUMBER' OF BOWLER AND PRESS 'ENTER'\n");
	wrefresh(getbwl);
	above:f = bltm;
	wscanw(getbwl, "%d", &pos);
	curs_set(0);
	if(bwlpos == pos){// CHECK WHETHER BOWLER WHO BOWLED THE PREVIOUS OVER AND NEWLY SELECTED BOWLER IS SAME
		wprintw(getbwl, "\nTHIS BOWLER JUST BOWLED AN OVER, PLEASE SELECT ANOTHER BOWLER");
		wrefresh(getbwl);
		napms(4000);
		wdeleteln(getbwl);
		wmove(getbwl, 13, 0);
		curs_set(1);
		wdelch(getbwl);
		wrefresh(getbwl);
		goto above;
	}
	while(k<pos){
		f = f->next;
		k++;
	}
	tmp = f;
	delwin(getbwl);
	touchwin(scrbrd);
	bwlpos = pos;
	wmove(bowl, 2, 0);
	wdeleteln(bowl);
	mvwprintw(bowl, 2, 2, "%s %s %s", f->attr, f->fname, f->lname);
	mvwprintw(bowl, 2, 42, "\t %.0f  \t\t %d\t\t %.0f    \t   %d\t\t  %.2f", f->prof->bowlr.ovrc, f->prof->bowlr.m, f->prof->bowlr.rg, f->prof->bowlr.w, f->prof->bowlr.eco);
	wrefresh(bowl);
	wrefresh(scrbrd);
	f = NULL;	
	return tmp;
}

//FUNCTION TO INCREMENT THE OVERS OF AN INNING
float checkover(int a){
	if(a == 0){
		over = over + 0.1;
		ovc++;
		if(ovc == 6){
			over = over + 0.4;
			ovc = 0;
		}
	}
	if(a == 1){
		;	
	}
	return over;
}

//FUNCTION FOR INCREMENTING THE PARAMETERS WHICH ARE REQIURED TO BE INCREMENTED AFTER EVERY BALL eg.CURRENT RUNRATE OF TEAM, OVER COUNT OF BOWLER, RUNS/BALLS OF A PARTNERSHIP ETC.
void incrpara(teamscr *tscr, pship *ps){
	bowller->prof->bowlr.ballc += 1;
	rrb++;
	(ovr->bcount)++;
	bowller->prof->bowlr.ovrc = bowller->prof->bowlr.ovrc + 0.1;
	if(ovr->bcount == 6){
		tscr->crr = tscr->total / (rrb / 6);
		bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg/ (bowller->prof->bowlr.ballc / 6));
		bowller->prof->bowlr.ovrc = bowller->prof->bowlr.ovrc + 0.4;
		checkover(0);
	}
	else{
		tscr->crr = tscr->total / (rrb / 6);
		bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg / (bowller->prof->bowlr.ballc / 6));
		checkover(0);
	}	
	if(ovr->dcount == 6)
		bowller->prof->bowlr.m += 1;
	ps->balls++;
	tscr->projscr = tscr->crr * ovr->n;
}

//FUNCTION TO INITIALIZE BATIING SCORECARD OF A TEAM 
void initbatsc(WINDOW *w){
	node *f;
	f = bttm;
	int k = 0;
	mvwprintw(w,3,0,"BATSMAN");
	mvwprintw(w,3,58,"RUNS(BALLS)\t   FALL of WICKET");
	while(k < 11){
		mvwprintw(w, 5 + k, 0, "%s %s", f->fname, f->lname);
		f = f->next;
		k++;
	}
}

//FUNCTION TO TELL HOW BATSMAN IS OUT
int howout(WINDOW *w, node *p, node *q, int c, int d){	
	int i = 0, a, pos;
	ho = newwin(0, 0, 0, 0);
	if(ho == NULL){
		addstr("Unable to allocate memory for new window.");
		refresh();
		getch();
		endwin();
		exit(0);
	}
	waddstr(ho, "\n\nHOW IS THE BATSMAN OUT?\n");
	wrefresh(ho);
	node *f;
	f = bltm;
	screen(10);
	a = sc1(10);
	p->prof->batsmn.ho = a;
	wprintw(ho, "\n");
	if( a == 1 || a == 3){
		while(i < 11){
			mvwprintw(ho, 11 + i, 0, "%d.%s %s %s\n", i+1, f->attr, f->fname, f->lname);
			f = f->next;
			i++;
		}
		wrefresh(ho);
		i = 1;
		f = bltm;
		if(a == 1)
			waddstr(ho, "ENTER THE POSITION NUMBER OF THE PLAYER WHO HAS TAKEN THE CATCH AND PRESS ENTER\n");
		else
			waddstr(ho, "RUNOUT BY ? ENTER THE POSITION NUMBER OF PLAYER AND PRESS ENTER\n");
		wrefresh(ho);
		wscanw(ho, "%d", &pos);
		if(bwlpos == pos){
			if(a == 1)
				goto t;
		}
		while(i < pos){
			f = f->next;
			i++;
		}
		if(a == 1){
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 30, "c %s  b %s", f->lname, q->lname);
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 60, "%.0f(%.0f)\t\t%d - %d", p->prof->batsmn.r, p->prof->batsmn.b, c, d);
		}		
		else{
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 30, "runout (%s)", f->lname); 
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 60, "%.0f(%.0f)\t\t%d - %d", p->prof->batsmn.r, p->prof->batsmn.b, c, d);
			return 1;
		}
	}
	else{
		if(a == 0){
			t:mvwprintw(w, 3 + p->prof->batsmn.bpos, 30, "c&b %s", q->lname);
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 60, "%.0f(%.0f)\t\t%d - %d", p->prof->batsmn.r, p->prof->batsmn.b, c, d);
		}		
		else{
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 30, "b %s", q->lname);
			mvwprintw(w, 3 + p->prof->batsmn.bpos, 60, "%.0f(%.0f)\t\t%d - %d", p->prof->batsmn.r, p->prof->batsmn.b, c, d);
		}
	}
	return 0;
}

/* 
   FUNCTION TO GET NEXTBALL INPUT, AND TO UPDATE SCOREBOARD.
   BATSMAN ROTATE STRIKE IF ONE RUN, THREE RUNS ARE SCORED AND AT THE END OF OVER
*/
void nextball(char ball, teamscr *tscr, extras *ex, pship *ps){
	int ifro;
	switch(ball){
		case '0'://BALL INPUT : 0 RUNS
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.b += 1; 
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 2, 42);
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
			}
			(ovr->dcount)++;
			incrpara(tscr, ps);
			if(ingf == 1)
				totalballs--;
			break;
		case '1'://BALL INPUT : 1 RUNS
			tscr->total += 1;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.r += 1;
				bata->prof->batsmn.b += 1;
				bata->prof->batsmn.onstrike = 0;
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 2, 42);
				goto down;
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.r += 1;
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.onstrike = 0;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
				goto down1;
			}
			down:if(bata->prof->batsmn.onstrike == 0)
				batb->prof->batsmn.onstrike = 1;
			down1:if(batb->prof->batsmn.onstrike == 0)
				bata->prof->batsmn.onstrike = 1;
				
			bowller->prof->bowlr.rg += 1;
			ps->scr += 1;
			incrpara(tscr, ps);
			if(ingf == 1){
				target -= 1;
				totalballs--;
			}
		  	break;
		case '2'://BALL INPUT : 2 RUNS
			tscr->total += 2;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.r += 2;
				bata->prof->batsmn.b += 1;
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 2, 42);
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.r += 2;
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
			}
			bowller->prof->bowlr.rg += 2;
			ps->scr += 2;
			incrpara(tscr, ps);
			if(ingf == 1){
				target -= 2;
				totalballs--;
			}
			break;
		case '3'://BALL INPUT : 3 RUNS
			tscr->total += 3;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.r += 3;
				bata->prof->batsmn.b += 1;
				bata->prof->batsmn.onstrike = 0;
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 2, 42);
				goto d;
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.r += 3;
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.onstrike = 0;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
				goto d1;
			}
			d:if(bata->prof->batsmn.onstrike == 0)
				batb->prof->batsmn.onstrike = 1;
			d1:if(batb->prof->batsmn.onstrike == 0)
				bata->prof->batsmn.onstrike = 1;
			bowller->prof->bowlr.rg += 3;
			ps->scr += 3;
			incrpara(tscr, ps);
			if(ingf == 1){
				target -= 3;
				totalballs--;
			}
			break;
		case '4'://BALL INPUT : 4 RUNS
			tscr->total += 4;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.r += 4;
				bata->prof->batsmn.b += 1;
				bata->prof->batsmn.fours += 1;
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 3, 42);
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.r += 4;
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.fours += 1;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
			}
			bowller->prof->bowlr.rg += 4;
			ps->scr += 4;
			incrpara(tscr, ps);
			if(ingf == 1){
				target -= 4;
				totalballs--;
			}
			break;
		case '6'://BALL INPUT : 6 RUNS
			tscr->total += 6;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.r += 6;
				bata->prof->batsmn.b += 1;
				bata->prof->batsmn.sixes += 1;
				bata->prof->batsmn.SR = ((bata->prof->batsmn.r/bata->prof->batsmn.b) * 100);
				mvwprintw(batm, 2, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", bata->prof->batsmn.r, bata->prof->batsmn.b, bata->prof->batsmn.fours, bata->prof->batsmn.sixes, bata->prof->batsmn.SR);
				touchline(batm, 2, 42);
			}
			if(batb->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.r += 6;
				batb->prof->batsmn.b += 1;
				batb->prof->batsmn.sixes += 1;
				batb->prof->batsmn.SR = ((batb->prof->batsmn.r/batb->prof->batsmn.b) * 100);
				mvwprintw(batm, 3, 42, "\t %.0f    \t\t %.0f\t\t %d    \t   %d\t\t  %.2f", batb->prof->batsmn.r, batb->prof->batsmn.b, batb->prof->batsmn.fours, batb->prof->batsmn.sixes, batb->prof->batsmn.SR);
				touchline(batm, 3, 42);
			}
			bowller->prof->bowlr.rg += 6;
			ps->scr += 6;
			incrpara(tscr, ps);
			if(ingf == 1){
				target -= 6;
				totalballs--;
			}			
			break;
		case 'o': case 'O'://BALL INPUT : WICKET
			tscr->wickets += 1;
			if(tscr->wickets == 10)
				alldown = 1;
			if(bata->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.high = bata->prof->batsmn.r;
				if(ingf == 0){
					if((tBbat == 1) && (tAbat == 0))
						ifro = howout(tbbasc, bata, bowller, tscr->wickets, tscr->total);
					if((tAbat == 1) && (tBbat == 0))//else
						ifro = howout(tabasc, bata, bowller, tscr->wickets, tscr->total);
				}
				else if(ingf == 1){
					if((tBbat == 1) && (tAbat == 0))
						ifro = howout(tabasc, bata, bowller, tscr->wickets, tscr->total);
					if((tAbat == 1) && (tBbat == 0))
						ifro = howout(tbbasc, bata, bowller, tscr->wickets, tscr->total);

				}
				wmove(batm, 2, 0);
				wdeleteln(batm);
				winsertln(batm);
				bata = getbatsman(0);
				wrefresh(scrbrd);
				bata->prof->batsmn.onstrike = 1;
			}
			if(batb->prof->batsmn.onstrike == 1){
				bata->prof->batsmn.high = bata->prof->batsmn.r;
				if(ingf == 0){
					if((tBbat == 1) && (tAbat == 0))
						ifro = howout(tbbasc, batb, bowller, tscr->wickets, tscr->total);
					if((tAbat == 1) && (tBbat == 0))//else
						ifro = howout(tabasc, batb, bowller, tscr->wickets, tscr->total);
				}
				else if(ingf == 1){
					if((tBbat == 1) && (tAbat == 0))
						ifro = howout(tabasc, batb, bowller, tscr->wickets, tscr->total);
					if((tAbat == 1) && (tBbat == 0))
						ifro = howout(tbbasc, batb, bowller, tscr->wickets, tscr->total);

				}
				wmove(batm, 3, 0);
				wdeleteln(batm);
				winsertln(batm);
				batb = getbatsman(1);
				wrefresh(scrbrd);
				batb->prof->batsmn.onstrike = 1;
			}
			rrb++;
			if(ifro == 0)
				bowller->prof->bowlr.w += 1;
			ps->scr = ps->balls = 0;
			ovr->dcount += 1;
			bowller->prof->bowlr.ovrc += 0.1;
			ovr->bcount += 1;
			bowller->prof->bowlr.ballc += 1;
			if(ovr->bcount == 6){
				tscr->crr = tscr->total / (rrb / 6);
				bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg / (bowller->prof->bowlr.ballc / 6));
				bowller->prof->bowlr.ovrc = bowller->prof->bowlr.ovrc + 0.4;
				checkover(0);
			}
			else{	
				checkover(0);		
				tscr->crr = tscr->total / (rrb / 6);
				bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg / (bowller->prof->bowlr.ballc / 6));
			}
			tscr->projscr = tscr->crr * ovr->n;
			if(ingf == 1)
				totalballs--;
			break;
		case 'w': case 'W'://BALL INPUT : WIDE
			tscr->total += 1;
			bowller->prof->bowlr.rg += 1;
			ex->wides++;
			ps->scr += 1;
			if(bowller->prof->bowlr.ovrc == 0.0)
				goto y;
			bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg / (bowller->prof->bowlr.ballc / 6));
			y:if(overno == 0)
				goto x;
			tscr->crr = tscr->total / (rrb / 6);
			tscr->projscr = tscr->crr * ovr->n;
			x:mvwprintw(extra, 2, 2, "NOBALLS: %d \tWIDES: %d \n", ex->noballs, ex->wides);
			touchline(extra, 2, 0);
			if(ingf == 1)
				target -= 1;
			break;	
		case 'n': case 'N'://BALL INPUT : NO BALL
			tscr->total += 1;
			bowller->prof->bowlr.rg += 1;
			ex->noballs++;
			ps->scr += 1;
			if(bowller->prof->bowlr.ovrc == 0.0)
				goto z;	
			bowller->prof->bowlr.eco = (bowller->prof->bowlr.rg / (bowller->prof->bowlr.ballc / 6));
			z:if(overno == 0)
				goto w;			
			tscr->crr = tscr->total / (rrb / 6);
			tscr->projscr = tscr->crr * (ovr->n);
			w:mvwprintw(extra, 2, 2, "NOBALLS: %d \tWIDES: %d \n", ex->noballs, ex->wides);
			touchline(extra, 2, 0);
			if(ingf == 1)
				target -= 1;	
			break;
		default:
			 break;
	}
	if(bata->prof->batsmn.onstrike == 1){
		wmove(batm, 3, 0);
		wdelch(batm);
		winsch(batm, ' ');
		touchline(batm, 3, 0);
		mvwprintw(batm, 2, 0, "*");
		touchline(batm, 2, 0);	
	}
	if(batb->prof->batsmn.onstrike == 1){
		wmove(batm, 2, 0);
		wdelch(batm);
		winsch(batm, ' ');
		touchline(batm, 2, 0);
		mvwprintw(batm, 3, 0, "*");
		touchline(batm, 3, 0);
	}
	if(ingf == 1){
		if(batflag == 0){
			wattron(scr, A_BOLD);
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\t\tTARGET: %d\n", whichteam(tsA, 0), tscr->total, tscr->wickets, over, tscr->target);
			wattroff(scr, A_BOLD);
			touchline(scr, 1, 0);
			wrefresh(scr);
			wmove(scrbrd, 11, 1);
			wdeleteln(scrbrd);
			winsertln(scrbrd);
			wattron(scrbrd, A_BOLD);
			mvwprintw(scrbrd, 11, 1, "%s NEEDS %d RUS FROM %d BALLS, TO WIN", whichteam(tsA, 0), target, totalballs);
			wattroff(scrbrd, A_BOLD);
			touchline(scrbrd, 11, 1);
		}
		else{
			wattron(scr, A_BOLD);
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\t\tTARGET: %d\n", whichteam(tsB, 0), tscr->total, tscr->wickets, over, tscr->target);
			wattroff(scr, A_BOLD);
			touchline(scr, 1, 0);
			wrefresh(scr);
			wmove(scrbrd, 11, 1);
			wdeleteln(scrbrd);
			winsertln(scrbrd);
			wattron(scrbrd, A_BOLD);
			mvwprintw(scrbrd, 11, 1, "%s NEEDS %d RUS FROM %d BALLS, TO WIN", whichteam(tsB, 0), target, totalballs);
			wattroff(scrbrd, A_BOLD);	
			touchline(scrbrd, 7, 1);
			
		}	
	}
	
	if(ingf == 0){
		if(batflag == 0){
			wattron(scr, A_BOLD);
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\n", whichteam(tsB, 0), tscr->total, tscr->wickets, over);
			wattroff(scr, A_BOLD);
			touchline(scr, 1, 0);
		}
		else{
			wattron(scr, A_BOLD);
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\n", whichteam(tsA, 0), tscr->total, tscr->wickets, over);
			wattroff(scr, A_BOLD);
			touchline(scr, 1, 0);		
		}
	}
	if((tAbat == 1) && (tBbat == 0)){
		if(ingf == 1){
			mvwprintw(tbbasc, 3 + bata->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tbbasc, 3 + bata->prof->batsmn.bpos, 60, "%.0f(%.0f)", bata->prof->batsmn.r, bata->prof->batsmn.b);
			mvwprintw(tbbasc, 3 + batb->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tbbasc, 3 + batb->prof->batsmn.bpos, 60, "%.0f(%.0f)", batb->prof->batsmn.r, batb->prof->batsmn.b);
			touchwin(tbbasc);		
		}
		else
			mvwprintw(tabasc, 3 + bata->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tabasc, 3 + bata->prof->batsmn.bpos, 60, "%.0f(%.0f)", bata->prof->batsmn.r, bata->prof->batsmn.b);
			mvwprintw(tabasc, 3 + batb->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tabasc, 3 + batb->prof->batsmn.bpos, 60, "%.0f(%.0f)", batb->prof->batsmn.r, batb->prof->batsmn.b);
			touchwin(tabasc);
	}
	if((tBbat == 1) && (tAbat == 0)){
		if(ingf == 1){
			mvwprintw(tabasc, 3 + bata->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tabasc, 3 + bata->prof->batsmn.bpos, 60, "%.0f(%.0f)", bata->prof->batsmn.r, bata->prof->batsmn.b);
			mvwprintw(tabasc, 3 + batb->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tabasc, 3 + batb->prof->batsmn.bpos, 60, "%.0f(%.0f)", batb->prof->batsmn.r, batb->prof->batsmn.b);
			touchwin(tabasc);
		}
		else{
			mvwprintw(tbbasc, 3 + bata->prof->batsmn.bpos, 30, "notout");
			mvwprintw(tbbasc, 3 + bata->prof->batsmn.bpos, 60, "%.0f(%.0f)", bata->prof->batsmn.r, bata->prof->batsmn.b);
			mvwprintw(tbbasc, 3 + batb->prof->batsmn.bpos, 30, "notout");	
			mvwprintw(tbbasc, 3 + batb->prof->batsmn.bpos, 60, "%.0f(%.0f)", batb->prof->batsmn.r, batb->prof->batsmn.b);
			touchwin(tbbasc);
		}
	}
	mvwprintw(curr, 0, 1, "CURRENT RUN RATE: %.2f\t\tPARTNERSHIP: %d(%d)\t\t\tPROJECTED SCORE: %d\n", tscr->crr, ps->scr, ps->balls, tscr->projscr);
	touchline(curr, 0, 1);
	mvwprintw(bowl, 2, 42, "\t %.1f  \t\t %d\t\t %.0f    \t   %d\t\t  %.2f", bowller->prof->bowlr.ovrc, bowller->prof->bowlr.m, bowller->prof->bowlr.rg, bowller->prof->bowlr.w, bowller->prof->bowlr.eco);
	touchline(bowl, 2, 1);
	wrefresh(scr);
	wrefresh(batm);
	touchwin(scrbrd);
	wrefresh(scrbrd);
}

//INITIALIZE BOWLING SCORECARD
void bwlscr(WINDOW *w){
	int j = 0, k = 0;
	node *tmp;
	tmp = bltm;
	mvwprintw(w, 3, 0, "BOWLER");
	mvwprintw(w, 3, 35, "OVERS\tMAIDENS \tRUNS \tWICKETS \tECONOMY\n");
	while(j < 11){
		if(tmp->prof->bowlr.bfl == 1){
			mvwprintw(w, 5 + k, 0, "%s %s\t", tmp->fname, tmp->lname);
			mvwprintw(w, 5 + k, 35, " %.1f    \t %d     \t\t %.0f  \t   %d  \t\t  %.2f", tmp->prof->bowlr.ovrc, tmp->prof->bowlr.m, tmp->prof->bowlr.rg, tmp->prof->bowlr.w, tmp->prof->bowlr.eco);
			k++;
		}
		tmp = tmp->next;
		j++;
	}
	wattron(w,A_REVERSE);
	mvwaddstr(w, 39, 103,"> PRESS ENTER TO PROCEED\n");
	wrefresh(w);
	wattroff(w,A_REVERSE);
}

/* 
   FUCNTION FOR DISPLAYING SCORBOARD AND ALL ITS PARAMETERS
   ingf = 0 INDICATES FIRST INNINGS AND ingf = 1 INDICATES SECOND INNINGS
   tAbat, tBbat REPRESENT IF A TEAM BATS. THEY TAKE VALUE 1 IF A TEAM BATS AND 0 IF A TEAM BOWLS
*/		
void scrboard(teamscr *tscr, extras *ex, pship *ps, overs *p){
	obo = newwin(0,0,0,0);
	mvwprintw(obo,1,0,"OVER BY OVER DETAILS");
	wattron(obo,A_REVERSE);
	if(ingf == 1)
		mvwaddstr(obo, 39, 93,">PRESS ENTER 'TWO' TIMES TO EXIT\n");
	else
		mvwaddstr(obo, 39, 103,">PRESS ENTER TO PROCEED\n");
	wattroff(obo,A_REVERSE);
	WINDOW *result;
	wrefresh(owps);
	if(ingf == 1)
		result = newwin(0, 0, 0, 0);
	tabasc = newwin(0, 0, 0, 0);tbbasc = newwin(0, 0, 0, 0);tabosc = newwin(0, 0, 0, 0);tbbosc = newwin(0, 0, 0, 0);
	scrbrd = newwin(0, 0, 0, 0);
	if(tabasc == NULL || tbbasc == NULL || tabosc == NULL || tbbosc == NULL || scrbrd == NULL){
				addstr("Unable to allocate memory for new window.");
				refresh();
				getch();
				endwin();
				exit(0);
			}
	ex = (extras *)malloc(sizeof(struct extras));
	if(ex == NULL){
		wprintw(scrbrd, "!!! MALLLOC FAILED !!!\n");
		wrefresh(scrbrd);
		napms(1000);
		endwin();
		exit(0);
	}
	ps = (pship *)malloc(sizeof(struct partnership));
	if(ps == NULL){
		wprintw(scrbrd, "!!! MALLLOC FAILED !!!\n");
		wrefresh(scrbrd);
		napms(1000);
		endwin();
		exit(0);
	}
	tscr = (teamscr *)malloc(sizeof(struct teamscore));
	if(tscr == NULL){
		wprintw(scrbrd, "!!! MALLLOC FAILED !!!\n");
		wrefresh(scrbrd);
		napms(1000);
		endwin();
		exit(0);
	}
	ex->wides = ex->noballs = 0;
	ps->scr = ps->balls = 0;
	tscr->total = tscr->wickets = tscr->projscr= 0;
	tscr->crr = 0.0;
	if(ingf == 1)
		tscr->target = teamtotal;
	
	wattron(scrbrd,A_REVERSE);
	mvwaddstr(scrbrd,33,2,"PLEASE READ THE BELOW GUIDELINES CAREFULLY.");
	wattroff(scrbrd,A_REVERSE);
	keypad(scrbrd,TRUE);
	int y, x;
	getmaxyx(scrbrd, y, x);
	char menu[8][80]={
			"Melbourne Cricket Ground, MELBOURNE, AUSTRALIA", 
			"Lord's Cricket Ground, LONDON, ENGLAND", 
			"Eden Park, AUCKLAND, NEWZEALAND", 
			"Feroz Shah Kotla Ground, NEWDELHI, INDIA", 
			"New Wanderers Stadium, JOHANNESBURG, SOUTH AFRICA", 
			"Kensington Oval, BRIDGETOWN, WESTINDIES", 
			"Pallekele International Stadium, SRILANKA ", 
			"National Stadium, KARACHI, PAKISTAN"};
	wattron(scrbrd, A_BOLD);
	mvwprintw(scrbrd, 0, ((x-strlen(menu[stadium]))/2)-14, "LIVE FROM THE %s\n", menu[stadium]);
	wattroff(scrbrd, A_BOLD);
	switch(tossflag){
		case 1:
			switch(choice1){
				case 0: 
					wprintw(scrbrd, "\nTOSS : %s OPT TO BAT", whichteam(tsA, 0));
					batflag = 1;
					break;
				case 1: 
					wprintw(scrbrd, "\nTOSS : %s OPT TO BOWL", whichteam(tsA, 0));
					break;
			}
			break;
		case 0: 
			switch(ser){
				case 0: 
					wprintw(scrbrd, "\nTOSS : %s OPT TO BOWL", whichteam(tsB, 0));
					batflag = 1;
					break;
				case 1: 
					wprintw(scrbrd, "\nTOSS : %s OPT TO BAT", whichteam(tsB, 0));
					break; 			
			}
			break;
	}
	scr = subwin(scrbrd, 2, 144, 4, 0);
	if(scr == NULL){
		waddstr(scrbrd,"Unable to allocate memory for new window.");
		wrefresh(scrbrd);
		getch();
		endwin();
		exit(0);
	}
	if(ingf == 1){	
		if((tBbat == 1) && (tAbat == 0)){
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\t\tTARGET: %d\n", whichteam(tsA, 0), tscr->total, tscr->wickets, over, tscr->target);
			wattron(scrbrd, A_BOLD);
			mvwprintw(scrbrd, 11, 1, "%s NEEDS %d RUNS FROM %d BALLS, TO WIN", whichteam(tsA, 0), target, totalballs);
			wattroff(scrbrd, A_BOLD);
		}
		if((tAbat == 1) && (tBbat == 0)){
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\t\tTARGET: %d\n", whichteam(tsB, 0), tscr->total, tscr->wickets, over, tscr->target);
			wattron(scrbrd, A_BOLD);
			mvwprintw(scrbrd, 11, 1, "%s NEEDS %d RUS FROM %d BALLS, TO WIN", whichteam(tsB, 0), target, totalballs);
			wattroff(scrbrd, A_BOLD);
		}	
	}
	if(ingf == 0){
		wattron(scr, A_BOLD);
		if((tBbat == 1) && (tAbat == 0))
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\n", whichteam(tsB, 0), tscr->total, tscr->wickets, over);
		if((tAbat == 1) && (tBbat == 0))
			mvwprintw(scr, 1, 30, "%s : %d - %d\t\tOVERS: %.1f\n", whichteam(tsA, 0), tscr->total, tscr->wickets, over);
		wattroff(scr, A_BOLD);		
	}

	//INITALZE BATTING AND BOWLING SCORECARD ACCORDING TO RESULT OF TOSS
	if((tAbat == 1) && (tBbat == 0)){
		bttm = t = p11 = lA->head;
		bltm = q11 = lB->head;
		if(ingf == 1){
			bltm = p11;
			bttm = t = q11;
			wprintw(tbbasc, "\n'%s' batting score card", whichteam(tsB, 0));
			wattron(tbbasc,A_REVERSE);
			mvwaddstr(tbbasc, 39, 103,"> PRESS ENTER TO PROCEED\n");
			wattroff(tbbasc,A_REVERSE);
			wprintw(tabosc, "\n'%s' bowling score card", whichteam(tsA, 0));
			initbatsc(tbbasc);
		}
		wprintw(tabasc, "\n'%s' batting score card", whichteam(tsA, 0));
		wattron(tabasc,A_REVERSE);
		mvwaddstr(tabasc, 39, 103,"> PRESS ENTER TO PROCEED\n");
		wattroff(tabasc,A_REVERSE);
		wprintw(tbbosc, "\n'%s' bowling score card", whichteam(tsB, 0));
		initbatsc(tabasc);
	}
	if((tBbat == 1) && (tAbat == 0)){
		bttm = t = q11 = lB->head;
		bltm = p11 = lA->head;
		if(ingf == 1){
			bttm = t = p11;
			bltm = q11;
			wprintw(tabasc, "\n'%s' batting score card", whichteam(tsA, 0));
			wattron(tabasc,A_REVERSE);
			mvwaddstr(tabasc, 39, 103,"> PRESS ENTER TO PROCEED\n");
			wattroff(tabasc,A_REVERSE);
			wprintw(tbbosc, "\n'%s' bowling score card", whichteam(tsB, 0));
			initbatsc(tabasc);
		}
		wprintw(tbbasc, "\n'%s' batting score card", whichteam(tsB, 0));
		wattron(tbbasc,A_REVERSE);
		mvwaddstr(tbbasc, 39, 103,"> PRESS ENTER TO PROCEED\n");
		wattroff(tbbasc,A_REVERSE);
		wprintw(tabosc, "\n'%s' bowling score card", whichteam(tsA, 0));
		initbatsc(tbbasc);	
	}

	//INITIALZE SUBWINDOWS ON SCORBOARD
	curr = subwin(scrbrd, 1, 100, 8, 0);
	mvwprintw(curr, 0, 2, "Current Run Rate: %.2f\t\tPartnership: %d(%d)\t\t\tProjected Score: %d\n", tscr->crr, ps->scr, ps->balls, tscr->projscr);
	batm = subwin(scrbrd, 5, 140, 14, 0);	
	mvwprintw(batm, 0, 2, "BATSMAN\t\t\t\t\tRUNS \t\tBALLS  \t\t4's \t  6's    \t STRIKE RATE\n");
	bowl = subwin(scrbrd, 4, 135, 20, 0);	
	mvwprintw(bowl, 0, 2, "BOWLER \t\t\t\t\tOVERS\t\tMAIDENS\t\tRUNS\t  WICKETS\t ECONOMY\n");
	extra = subwin(scrbrd, 3, 30, 26, 1);
	mvwprintw(extra, 0, 2, "EXTRAS");
	mvwprintw(extra, 2, 2, "NOBALLS: %d \tWIDES: %d \n", ex->noballs, ex->wides);
	thisover = subwin(scrbrd, 1, 35, 30, 50);
	mvwprintw(thisover, 0, 0, "THIS OVER: \n");
	mvwprintw(scrbrd, 34, 0,"> AT THE START OF A TEAM INNING PRESS 'ENTER' TO GET THE FIRST BOWLER.\n");
	wprintw(scrbrd,"> AFTER THE END OF AN OVER PLEASE WAIT TO GET THE NEXT BOWLER.\n");
	wprintw(scrbrd,"> THE '*' AT THE STARTING OF BATSMAN'S NAME INDICATES THAT THE BATSMAN IS ON STRIKE.\n");
	wprintw(scrbrd,"> AFTER CHOOSING THE BOWLER PRESS KEY '0 / 1 / 2 / 3 / 4 / 6' for 'DOTBALL / 1 / 2 / 3 / 4 / 6' RUNS RESPECTIVELY.\n");
	wprintw(scrbrd,"> AFTER CHOOSING THE BOWLER PRESS KEY 'w / n / o' for 'WIDES / NOBALL / WICKET' RESPECTIVELY.\n");
	bata = getbatsman(0);
	bata->prof->batsmn.onstrike = 1;
	mvwprintw(batm, 2, 0, "*");
	batb = getbatsman(1);
	batb->prof->batsmn.onstrike = 0;
	wrefresh(scrbrd);
	char ch;
	ch = getch();
	switch(ch){
		case '\n':bowller = getbowler();//GET NEW BOWLER
			bowller->prof->bowlr.bfl = 1;
			break;
		default:
			break;
	}	
	ovr->bcount = 0;
	ovr->dcount = 0;
	z = 0;
	while(overno<ovr->n){
		p[overno].on = overno + 1;
		mvwprintw(obo,overno + 3,0,"OVER %d :", p[overno].on);
		if(ovr->bcount == 6){
			z = 0;
			ovr->dcount = 0;
			ovr->bcount = 0;
			overno++;
			p[overno].on = overno + 1;
			
			//IF NUMBER OF SELETED OVERS HAVE BEEN BOWLED THE END THE INNINGS
			if(overno == ovr->n){
				wattron(scrbrd,A_REVERSE);
				mvwaddstr(scrbrd, 39, 103,"> END OF INNINGS. PRESS ENTER TO PROCEED\n");
				//touchline(scrbrd;
				wrefresh(scrbrd);
				wattroff(scrbrd,A_REVERSE);
				getch();
				break;	
			}

			//CHANGE THE ON STRIKE SYMBOL ('*') AT THE START OF BATSMAN'S NAME
			if(bata->prof->batsmn.onstrike == 1){
				batb->prof->batsmn.onstrike = 1;
				bata->prof->batsmn.onstrike = 0;
				wmove(batm, 2, 0);
				wdelch(batm);
				winsch(batm, ' ');
				touchline(batm, 2, 1);
				mvwprintw(batm, 3, 0, "*");
				touchline(batm, 3, 1);
			}
			else{
				bata->prof->batsmn.onstrike = 1;
				batb->prof->batsmn.onstrike = 0;
				wmove(batm, 3, 0);
				wdelch(batm);
				winsch(batm, ' ');
				touchline(batm, 3, 1);
				mvwprintw(batm, 2, 0, "*");
				touchline(batm, 2, 1);
			}
			wmove(thisover, 0, 0);
			wdeleteln(thisover);
			napms(1000);
			bowller->prof->bowlr.best.w = bowller->prof->bowlr.w;
			bowller->prof->bowlr.best.rg = bowller->prof->bowlr.rg;
			bowller = getbowler();
			bowller->prof->bowlr.bfl = 1;
			mvwprintw(thisover, 0, 0, "THIS OVER: \n");
			touchline(thisover, 1, 0);
			wrefresh(thisover);
		}
		ball_input = getch();					
		oveR[z] = ball_input;
		p[overno].ov[z] = ball_input;
		mvwprintw(obo,overno + 3,9 + (2 * z),"%c", p[overno].ov[z]);
		mvwprintw(thisover, 0, 12 + (2 * z), "%c", oveR[z]);
		touchline(thisover, 1, 0);
		nextball(ball_input, tscr, ex, ps);//GET NEXT BALL INPUT
		if((ingf == 0) && (alldown == 1))
			break;
		if((ingf == 1) && (alldown == 1))
			break;
		if((ingf == 1) && (tscr->total >= tscr->target))
			break;
		z++;
	}
	if(ingf == 1){
		wattron(result, A_BOLD);
		if((overno == ovr->n) && (tscr->total < tscr->target)){
			if((tAbat == 1) && (tBbat == 0))
				mvwprintw(result, 21, 15, "TEAM %s win by %d runs\n", whichteam(tsA, 0), tscr->target - tscr->total);
			else if((tAbat == 0) && (tBbat == 1))
				mvwprintw(result, 21, 15, "TEAM %s win by %d runs\n", whichteam(tsB, 0), tscr->target - tscr->total);
		}
		if((alldown == 1) && (tscr->total < tscr->target)){
			if((tAbat == 1) && (tBbat == 0))
				mvwprintw(result, 21, 15, "TEAM %s win by %d runs\n", whichteam(tsA, 0), tscr->target - tscr->total);
			else if((tAbat == 0) && (tBbat == 1))
				mvwprintw(result, 21, 15, "TEAM %s win by %d runs\n", whichteam(tsB, 0), tscr->target - tscr->total);
		}
		if(tscr->total >= tscr->target){
			if((tAbat == 1) && (tBbat == 0))
				mvwprintw(result, 21, 15, "TEAM %s win by %d wickets\n", whichteam(tsB, 0), 10 - tscr->wickets);
			else if((tAbat == 0) && (tBbat == 1))
				mvwprintw(result, 21, 15, "TEAM %s win by %d wickets\n", whichteam(tsA, 0), 10 - tscr->wickets);	
		}
		wattroff(result, A_BOLD);
		wrefresh(result);
		getch();
	}
	if(ingf == 0){	
		if((tAbat == 1) && (tBbat == 0))
			bwlscr(tbbosc);
		else if((tAbat == 0) && (tBbat == 1))
			bwlscr(tabosc);
		teamtotal = target = tscr->total + 1;
	}
	else if(ingf == 1){
		if((tAbat == 1) && (tBbat == 0))
			bwlscr(tabosc);
		else if((tAbat == 0) && (tBbat == 1))
			bwlscr(tbbosc);
	}
	wrefresh(scrbrd);
	getch(); 
}

//DELETE EXTRA WINNDOWS	
void delwins(){
	delwin(curr);
	delwin(bowl);
	delwin(batm);
	delwin(extra);
	delwin(scr);
	delwin(scrbrd);
}

//MAIN	
int main(){
	int a, i, pos, menuitem;
	char ch;
	FILE *pp;
	node *p = (node *)malloc(sizeof(struct node));
	if(p == NULL){
		printw( "!!! MALLLOC FAILED !!!\n");
		refresh();
		napms(1000);
		endwin();
		exit(0);
	}
	node *w;
	initscr();
	rr:screen(1);
	menuitem = sc1(1);
	switch(menuitem){
		case 0: up:tsA = teamselect(2);
			tsB = teamselect(3);
			if(tsA == tsB){
				errorwin();
				//touchwin(stdscr);
				refresh();
				touchwin(wteamA);
				touchwin(wteamB);
				wrefresh(wteamA);
				wrefresh(wteamB);
				goto up;
			}
			initswap(wswapA);
			initswap(wswapB);
			TEAMname(tsA, 0);
			/*pp = fopen(whichteam(tsA,1),"r+");
			w = lA->head;
			for(i = 0;i<15;i++){
				fprintf(pp, "%s %s %s %d %.0f %.0f %.0f %d %d %.2f %.1f %.0f %d %d %.2f %.2f %d\n", w->attr, w->fname, w->lname, w->prof->matches, w->prof->batsmn.r, w->prof->batsmn.b, w->prof->batsmn.high, w->prof->batsmn.fours, w->prof->batsmn.sixes, w->prof->batsmn.SR, w->prof->bowlr.ovrc, w->prof->bowlr.rg, w->prof->bowlr.m, w->prof->bowlr.w, w->prof->bowlr.eco, w->prof->bowlr.best.rg, w->prof->bowlr.best.w);
				w = w->next;
			}
			fclose(pp);*/
			token++;
			tok++;
			TEAMname(tsB, 0);
			owpsf();
			toss();
			scrboard(tscrA, exA, psA, OA);//FIRST INNNIG SCOREBOARD
			if((tBbat == 1) && (tAbat == 0)){
				wrefresh(tbbasc);
				getch();
				wrefresh(tabosc);
			}
			else if((tBbat == 0) && (tAbat == 1)){
				wrefresh(tabasc);
				getch();
				wrefresh(tbbosc);
			}
			wrefresh(obo);
			getch();
			wclear(obo);
			
			//INITILAIZE PARAMETERS CHANGED AFTER END OF FIRST INNNIGS
			ingf = 1;	
			bwlpos = 0;
			alldown = 0;
			overno = 0;
			over = 0.0;
			z = 0;
			batpos = 1;
			rrb = 0.0;
			delwins();
			scrboard(tscrB, exB, psB, OB);//SECOCND INNNIG SCORBOARD
			if((tBbat == 1) && (tAbat == 0)){
				wrefresh(tabasc);
				getch();
				wrefresh(tbbosc);
			}
			else if((tBbat == 0) && (tAbat == 1)){
				wrefresh(tbbasc);
				getch();
				wrefresh(tabosc);
			}
			wrefresh(obo);
			//getch();
			if(sA > 0){
				while(sA!=-1){	
					swap(&l1, wswapA[sA].p1, wswapA[sA].p2);
					sA--;
				}				
			}
			if(sB > 0){
				while(sB!=-1){
					swap(&l2, wswapB[sB].p1, wswapB[sB].p2);
					sB--;
				}
			}

			//WRTE THE PLAER PROFILES INTO THE FILE
			makestatsl(whichteam(tsA, 1), SA, &l1, 0);
			makestatsl(whichteam(tsB, 1), SB, &l2, 0);	
			break;
		case 1:	//SEARCH A PLAYER PROFILE	
			y: pf = newwin(0, 0, 0, 0);
			stat = newwin(0, 0, 0, 0);
			if(pf == NULL || stat == NULL){
				addstr("Unable to allocate memory for new window.");
				refresh();
				getch();
				endwin();
				exit(0);
			}
			wprintw(pf, "\nTHE PLAYER YOU ARE SEARCHING FOR BELONGS TO WHICH TEAM ?\n");
			screen(11);
			a = sc1(11);
			pp = fopen(TEAMname(a, 1), "r");
			wclear(pf);
			for(i = 0;i < 15;i++){
				fscanf(pp, "%s %s %s", p->attr, p->fname, p->lname);
				wprintw(pf, "%d. %s %s %s\n", i + 1, p->attr, p->fname, p->lname);
			}
			wrefresh(pf);
			fclose(pp);
			wprintw(pf, "\nENTER THE 'POSITION NUMBER' OF THE PLAYER and PRESS 'ENTER'\n");
			wrefresh(pf);
			wscanw(pf, "%d", &pos);
			i = pos - 1;
			SS = makestatsl(whichteam(a, 1), w, NULL, 1);						
			wattron(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "PLAYER PROFILE\n\n");
			wattroff(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "PLAYER NAME: %s %s\nPLAYER ATTRIBUTE: %s\t\t  (BAT: BATSMEN, FBOW:FAST BOWLER, SBOW: SPIN BOWLER, ALL: ALL-ROUNDER)\nMATCHES PLAYED: %d\n", SS[i].fname, SS[i].lname, SS[i].attr, SS[i].prof->matches);
			wattron(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "\nBATTING PROFILE\n\n");
			wattroff(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "RUNS: %.0f\nBALL PLAYED: %.0f\nFOURS: %d\nSIXES: %d\nSTRIKE RATE: %.2f\nBEST SCORE: %.0f\n", SS[i].prof->batsmn.r, SS[i].prof->batsmn.b, SS[i].prof->batsmn.fours, SS[i].prof->batsmn.sixes, SS[i].prof->batsmn.SR, SS[i].prof->batsmn.high);
			wattron(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "\nBOWLING PROFILE\n\n");
			wattroff(stat, A_UNDERLINE | A_BOLD);
			wprintw(stat, "OVERS BOWLED: %.0f\nRUNS GiVEN: %.0f\nMAIDENS: %d\nWICKETS: %d\nECONOMY: %.2f\nBEST BOWLING FIGURES: %d - %.0f", SS[i].prof->bowlr.ovrc, SS[i].prof->bowlr.rg, SS[i].prof->bowlr.m, SS[i].prof->bowlr.w, SS[i].prof->bowlr.eco, SS[i].prof->bowlr.best.w, SS[i].prof->bowlr.best.rg);
			mvwprintw(stat,35,0,"< BACK (PRESS B) >\n< MAIN MENU (PRESS M) >\n< EXIT (PRESS E) >");
			wrefresh(stat);
			y1:ch = getch();
			if(ch == 'b' || ch == 'B'){
				delwin(pf);
				delwin(stat);
				goto y;
			}
			else if(ch == 'm' || ch == 'M'){
				delwin(pf);
				delwin(stat);
				touchwin(stdscr);
				refresh();
				goto rr;
			}
			else if(ch == 'e' || ch == 'E')
				break;
			else
				goto y1;
			break;
		case 2: break;	
	}
	endwin();
	return 0;
}
//u need to pass tscr, ex, pship as function arguments of nextball or scoreboard function


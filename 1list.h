	
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

#include"OVER.h"
typedef struct info{
	int matches;
	batsman batsmn;
	bowler bowlr;
}profile;
typedef struct node {
	char attr[5];
	char fname[20];
	char lname[20];
	struct node *next, *prev;
	struct info *prof;//pointer to the profile of a player 
}node;
typedef struct list {
	struct node *head, *tail;
	int length;
	int initlen;
}list;
void init(list *l);
void insert(list *l, char *attr,char *fname,char *lname, int pos);


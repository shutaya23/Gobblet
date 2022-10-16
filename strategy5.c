#include<stdlib.h>
#include<stdio.h>
#include<string.h>
char* readFile(char *filename);
char* readlog(char *filename);
void writeFile(char *filename, char *content,char *mode);
int max(int n1,int n2,int n3);
int compute_x(int xc,int temps[5][5]);
int compute_y(int yc,int temps[5][5]);
int compute_9(int temps[5][5]);
int compute_10(int temps[5][5]);
void total_score(int temps[5][5]);
void next_score();
int piece[5][5];//棋盤 1~4大小
int piece_temp[5][5];
int piece_next_temp[5][5];
int hand1,hand2,hand3;
char *hand;//手排字串 
int i,j,total=0,max_total=-999999;
int runmode=0;//put,move
int player=0,own=0;
int x,y,c=0,d=0;//下點
int cc=0;//move的大小 
int score[11];//總分
int next_scan=0;
int listen[11];
int test=0;


int main(int argc,char *argv[] )
{
	hand1=hand2=hand3=0;
	memset(listen,0,11*sizeof(int));//初始 
	for(i=1;i<5;i++){
		for(j=1;j<5;j++){		
			piece[i][j]=0;
		}	
	}
	char *filename = argv[1];
    char *filecontent = readFile(filename);
    //算手牌 
	if(player==1){
        hand=strstr(filecontent,"Brown Hand");
		}
	else{
		hand=strstr(filecontent,"Yellow Hand");
	}
	char *h1,*h2,*h3;
	char *dupstr = strdup(hand);
	h1 = strtok(dupstr,"(");
	h1 = strtok(NULL,",");
	hand1=h1[0]-'0';
	h2 = strtok(NULL,",");
	hand2=h2[0]-'0';
	h3 = strtok(NULL,")");
	hand3=h3[0]-'0';
	free(dupstr);	      
    
    //建立當前棋譜 
    char *filename1 = argv[2];
    char *filecontent1 = readlog(filename1);

	c=max(hand1,hand2,hand3);//決定棋大小
	//算當前分
	test=1;
	total_score(piece);//算聽牌 
	test=0;			
	
	memcpy(piece_temp,piece,sizeof(piece));
	memcpy(piece_next_temp,piece,sizeof(piece));
	if(c!=0){//手牌優先 
		int i=0,j=0;
		for(i=1;i<5;i++){
			for(j=1;j<5;j++){		
				if(piece[i][j]==0){
					memcpy(piece_temp,piece,sizeof(piece));					
					if(player==0)
						piece_temp[i][j]=piece_temp[i][j]*10+c;
					else
						piece_temp[i][j]=piece_temp[i][j]*10+c+4;
					total_score(piece_temp);
					next_score();//
					if(total>max_total){
						max_total=total;
						x=i;y=j;
						if(test==1);
					}
				}
				else if((piece[i][j]%10-4<c&&piece[i][j]%10>4&&player==0)||(piece[i][j]%10<c&&piece[i][j]<5&&player==1)){//聽牌 
					if(listen[i]||listen[j+4]||((i==j)&&(listen[i]))||((i+j==5)&&(listen[i+j]))){
						memcpy(piece_temp,piece,sizeof(piece));					
						if(player==0)
							piece_temp[i][j]=piece_temp[i][j]*10+c;
						else
							piece_temp[i][j]=piece_temp[i][j]*10+c+4;
						total_score(piece_temp);
						next_score();//
						if(total>max_total){
							max_total=total;
							x=i;y=j;
						}
					}
				}
			}	
		}
	}
	//move
	int k=1,r=1;
	for(k=1;k<5;k++){
		for(r=1;r<5;r++){
			if(((piece[k][r]%10)<5)&&((piece[k][r]%10)>0)&&player==0){//黃
					int ia=1,ja=1,ta=0;
					ta=piece[k][r]%10;				
					for(ia=1;ia<5;ia++){
						for(ja=1;ja<5;ja++){
							if((((piece[ia][ja]%10)>4)&&(ta>((piece[ia][ja]%10)-4)))||piece[ia][ja]==0||((piece[ia][ja]%10<5)&&(piece[ia][ja]%10<ta))){//有考慮己方小的 
								memcpy(piece_temp,piece,sizeof(piece));
								cc=piece_temp[k][r]%10;//選棋的大小 
								piece_temp[k][r]=piece_temp[k][r]/10;
								piece_temp[ia][ja]=piece_temp[ia][ja]*10+ta;														
								total_score(piece_temp);
								next_score();//
								if(total>max_total){
									max_total=total;
									x=k;y=r;
									c=ia;d=ja;
									runmode=1;
									if(test==1);
								}								
							}				
						}
					}
			}
			else if(piece[k][r]%10>4&&player==1){//棕 
					int ib=1,jb=1,tb=0;
					tb=piece[k][r]%10;				
					for(ib=1;ib<5;ib++){
						for(jb=1;jb<5;jb++){
							if((tb-4)>piece[ib][jb]%10||((piece[ib][jb]%10>4)&&(tb>piece[ib][jb]))){//有考慮蓋自己 
								memcpy(piece_temp,piece,sizeof(piece));
								cc=piece_temp[k][r]%10-4;//選棕棋的大小
								piece_temp[k][r]=piece_temp[k][r]/10;//s??
								piece_temp[ib][jb]=piece_temp[ib][jb]*10+tb;//d													
								total_score(piece_temp);
								next_score();//
								if(total>max_total){
									max_total=total;
									x=k;y=r;
									c=ib;d=jb;
									runmode=1;
									if(test==1);
								}								
							}				
						}
					}

			}  
		}
			
	}			
	//寫入 
	char *filename2 = argv[3];	
	char *strx[] = {"(1,", "(2,", "(3,", "(4,"};
	char *stry[] = {"1,", "2,", "3,", "4,"};
	char *strc[] = {"1)", "2)", "3)", "4)","\n"};
	
	if(runmode==0){
		char *content = "Put\n";
    	writeFile(filename2, content,"w+");				
    	writeFile(filename2, strx[x-1],"a");//x
    	writeFile(filename2, stry[y-1],"a");//y   
   		writeFile(filename2, strc[c-1],"a");//棋 	
	}
	else if(runmode==1){
		char *content = "Move\n";
       	writeFile(filename2, content,"w+");				
    	writeFile(filename2, strx[x-1],"a");//x
   		writeFile(filename2, strc[y-1],"a");//y
		writeFile(filename2,strc[4],"a");//\n
   		writeFile(filename2, strx[c-1],"a");//c
		writeFile(filename2, strc[d-1],"a");//d	
	}
    return 0;
}

void writeFile(char *filename, char *content,char *mode)
{
    FILE *fp;
    fp = fopen(filename,mode);
    fprintf(fp,content);
    fclose(fp);
}

char* readFile(char *filename)
{
    int n=0;
	FILE *fp;
    char temp[1000];
    char *return_string = "";
    fp = fopen(filename, "r+");
	
    if (fp == NULL){
        printf("Can't open file\n");
        return return_string;
    }

    char *temp3 = "";
    while(fgets(temp, 1000, fp)!=NULL)
    {
    	n++;//行數 
        return_string = (char*)malloc((strlen(temp)+strlen(return_string))*sizeof(char));//
        strcpy(return_string,temp3);
        temp3 = (char*)malloc((strlen(temp)+strlen(temp3))*sizeof(char));//
        strcat(return_string,temp);
        strcpy(temp3,return_string);
        if(!strstr(return_string,"Player Yellow"))
			player=1;//brown        		

    }
    fclose(fp);
    return return_string;
}

char* readlog(char *filename)
{
    int n=0;
	FILE *fp;
    char temp[1000];
    char *return_string = "";
    fp = fopen(filename, "r+");	
    if (fp == NULL){
        printf("Can't open file\n");
        return return_string;
    }
    char *temp3 = "";
    while(fgets(temp, 1000, fp)!=NULL)
    { 	
    	int temp2=0,temp4=0,n=0;
        return_string = (char*)malloc((strlen(temp)+strlen(return_string))*sizeof(char));//
        strcpy(return_string,temp3);
        temp3 = (char*)malloc((strlen(temp)+strlen(temp3))*sizeof(char));//
        strcat(return_string,temp);
        strcpy(temp3,return_string);        
        if(strstr(temp,"Yellow")){
        	n++;//棋數
        	fgets(temp, 1000, fp);//下一列 
        	if(strstr(temp,"Put")){
        		fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				c=temp[5]-'0';	
				piece[x][y]=piece[x][y]*10+c;//下棋lsb表外層 
			}
        	else if(strstr(temp,"Move")){//??移動後空白 
				fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';				
				c=temp[6]-'0';
				d=temp[8]-'0'; 
				temp2=piece[x][y]%10; //移動棋
				piece[x][y]=piece[x][y]/10;
				temp4=piece[c][d]%10;
				piece[c][d]=piece[c][d]*10+temp2;
			}
        }
       	else if(strstr(temp,"Brown")){
       		n++;//棋數
			fgets(temp, 1000, fp);//下一列 
        	if(strstr(temp,"Put")){
        		fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				c=temp[5]-'0';
				piece[x][y]=piece[x][y]*10+c+4;
			}
        	else if(strstr(temp,"Move")){
				fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';				
				c=temp[6]-'0';
				d=temp[8]-'0';
				temp2=piece[x][y]%10; //移動棋
				piece[x][y]=piece[x][y]/10;//?
				temp4=piece[c][d]%10;
				piece[c][d]=piece[c][d]*10+temp2;
			}  
    	}
    own=n;//非手邊棋數
    }
    
    fclose(fp);
    return return_string;	
}

int compute_x(int xc,int temps[5][5]){//1~4
	int i,j,count=0,risk=0;
	for(j=1;j<5;j++){
		if(temps[xc][j]%10<5&&temps[xc][j]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[xc][j]%10>4){//有敵人 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	if(next_scan==0){
	score[xc]=score[xc]+count*3;//優勢 
	score[xc]=score[xc]-risk;//劣勢
	if(count==2&&risk==0) 
		score[xc]+=5;//決勝
	if(count==3){
		score[xc]+=100;//決勝
		if(risk==0)
			score[xc]+=10;
	}
	if(count==4)
		score[xc]+=200000;//勝 
	if(risk==3){
		score[xc]-=1500;//敗北
		listen[xc]=1;//聽牌
		if(count==0)
			score[xc]-=300; 
	}
	if(risk==3&&count==1)
		score[xc]+=500;
	if(risk==4)
		score[xc]-=50000;//敗北
	count=risk=0;
	if(test==1);
	}
	else{
		if(count==4){
			fprintf(stderr,"\n risk \n");
			total-=50000;} 
	} 	
	
}
int compute_y(int yc,int temps[5][5]){//5~8
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[i][yc-4]%10<5&&temps[i][yc-4]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[i][yc-4]%10>4){//有敵人 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	if(next_scan==0){
	score[yc]+=count*3;//優勢 
	score[yc]=score[yc]-risk;//劣勢 
	if(count==2&&risk==0) 
		score[yc]+=5;//決勝
	if(count==3){
		score[yc]+=100;//決勝
		if(risk==0)
			score[yc]+=10;
	}
	if(count==4)
		score[yc]+=200000;//決勝 
	if(risk==3){ 
		score[yc]-=1500;//敗北
		listen[yc]=1;//聽牌 
		if(count==0)
			score[yc]-=300;
	}
	if(risk==3&&count==1)
		score[yc]+=500;
	if(risk==4)
		score[yc]-=50000;//敗北 
	count=risk=0;
	if(test==1);
	}
	else{
		if(count==4){
			fprintf(stderr,"\n risk \n");
			total-=50000;} 
	}
}
int compute_9(int temps[5][5]){//9~10
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[i][i]%10<5&&temps[i][i]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[i][i]%10>4){//有敵人 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	if(next_scan==0){
	score[9]+=count*3;//優勢 
	score[9]=score[9]-risk;//劣勢 
	if(count==2&&risk==0) 
		score[9]+=5;//決勝
	if(count==3){
		score[9]+=100;//決勝
		if(risk==0)
			score[9]+=10;
	}
	if(count==4)
		score[9]+=200000;//決勝 
	if(risk==3){
		score[9]-=1500;//敗北
		listen[9]=1;//聽牌
		if(count==0)
			score[9]-=300;
	}
	if(risk==3&&count==1)
		score[9]+=500;
	if(risk==4)
		score[9]-=50000;//敗北 
	count=risk=0;
	if(test==1);
	}
	else{
		if(count==4){ 
			fprintf(stderr,"\n risk \n");
			total-=50000;} 
	}
}
int compute_10(int temps[5][5]){//9~10
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[4-i+1][i]%10<5&&temps[4-i+1][i]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[4-i+1][i]%10>4){//有敵人 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	if(next_scan==0){
	score[10]+=count*3;//優勢 
	score[10]=score[10]-risk;//劣勢
	if(count==2&&risk==0) 
		score[10]+=5;//決勝
	if(count==3){
		score[10]+=100;//決勝
		if(risk==0)
			score[10]+=10;
	}
	if(count==4)
		score[10]+=200000;//決勝 
	if(risk==3){
		score[10]-=1500;//敗北
		listen[10]=1;//聽牌
		if(count==0)
			score[10]-=300;
	}
	if(risk==3&&count==1)
		score[10]+=500;
	if(risk==4)
		score[10]-=50000;//敗北 
	count=risk=0;
	if(test==1);
	}
	else{ 
		if(count==4){
			fprintf(stderr,"\n risk \n");
			total-=50000;} 
	} 
}


void total_score(int temps[5][5]){//算總分
	if(next_scan!=1)
		memset(score,0,11*sizeof(int)); //歸零 
	
	for(i=1;i<5;i++){
		compute_x(i,temps);
		compute_y(i+4,temps);
	}
	compute_9(temps);
	compute_10(temps);
	if(next_scan==0){///
	total=0;
	for(j=1;j<11;j++)//合計1~10線
		if(score[j]!=0)
			total+=score[j];
	fprintf(stderr,"%d score:%d,10:%d\n",player,total,score[10]);
	} 
}

void next_score(){
	next_scan=1;
	if(player==1)
		player=0;
	else if(player==0)
		player=1;
	//move
	int k=1,r=1;
	for(k=1;k<5;k++){
		for(r=1;r<5;r++){
			if(((piece_temp[k][r]%10)<5)&&((piece_temp[k][r]%10)>0)&&player==0){//黃
					int ia=1,ja=1,ta=0;
					ta=piece_temp[k][r]%10;				
					for(ia=1;ia<5;ia++){
						for(ja=1;ja<5;ja++){
							if((((piece_temp[ia][ja]%10)>4)&&(ta>((piece_temp[ia][ja]%10)-4)))||piece_temp[ia][ja]==0){//不考慮己方小的 
								memcpy(piece_next_temp,piece_temp,sizeof(piece_temp));
								cc=piece_next_temp[k][r]%10;//選棋的大小 
								piece_next_temp[k][r]=piece_next_temp[k][r]/10;
								piece_next_temp[ia][ja]=piece_next_temp[ia][ja]*10+ta;														
								total_score(piece_next_temp);
							
							}				
						}
					}
			}
			else if(piece_temp[k][r]%10>4&&player==1){//棕 
					int ib=1,jb=1,tb=0;
					tb=piece_temp[k][r]%10;				
					for(ib=1;ib<5;ib++){
						for(jb=1;jb<5;jb++){
							if((tb-4)>piece_temp[ib][jb]%10){
								memcpy(piece_next_temp,piece_temp,sizeof(piece_temp));
								cc=piece_next_temp[k][r]%10-4;//選棕棋的大小
								piece_next_temp[k][r]=piece_next_temp[k][r]/10;//s??
								piece_next_temp[ib][jb]=piece_next_temp[ib][jb]*10+tb;//d													
								total_score(piece_next_temp);
							}				
						}
					}
			}  
		}
	}
	next_scan=0;
	if(player==1)
		player=0;
	else if(player==0)
		player=1;
}

int max(int n1,int n2,int n3){
	int max=n1;
	if(n2>n1){
		if(n3>n2)
			max=n3;
		else
			max=n2;	
	}
	else if(n3>n1)
		max=n3;
}

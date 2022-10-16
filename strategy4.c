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
int piece[5][5];//棋盤 1~4大小
int piece_temp[5][5];
int hand1,hand2,hand3;
char *hand; 
int i,j,total=0,max_total=-999999;
int runmode=0;//put,move
int player=0,own=0;
int x,y,c=0,d=0;//下點
int cc=0;//move的大小 
int score[11];//總分
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
			if(((piece[k][r]%10)<5)&&((piece[k][r]%10)>0)&&player==0){
					int ia=1,ja=1,ta=0;
					ta=piece[k][r]%10;				
					for(ia=1;ia<5;ia++){
						for(ja=1;ja<5;ja++){
							if((((piece[ia][ja]%10)>4)&&(ta>((piece[ia][ja]%10)-4)))||piece[ia][ja]==0){
								//fprintf(stderr,"\n 1-%d,%d,%d to %d,%d,%d \n",k,r,piece[k][r],ia,ja,piece[ia][ja]);
								memcpy(piece_temp,piece,sizeof(piece));
								cc=piece_temp[k][r]%10;
								piece_temp[k][r]=piece_temp[k][r]/10;
								piece_temp[ia][ja]=piece_temp[ia][ja]*10+ta;														
								total_score(piece_temp);
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
			else if(piece[k][r]%10>4&&player==1){
					int ib=1,jb=1,tb=0;
					tb=piece[k][r]%10;				
					for(ib=1;ib<5;ib++){
						for(jb=1;jb<5;jb++){
							if((tb-4)>piece[ib][jb]%10){
								memcpy(piece_temp,piece,sizeof(piece));
								cc=piece_temp[k][r]%10-4;
								piece_temp[k][r]=piece_temp[k][r]/10;
								piece_temp[ib][jb]=piece_temp[ib][jb]*10+tb;												
								total_score(piece_temp);
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
	char *filename2 = argv[3];	
	char *strx[] = {"(1,", "(2,", "(3,", "(4,"};
	char *stry[] = {"1,", "2,", "3,", "4,"};
	char *strc[] = {"1)", "2)", "3)", "4)","\n"};
	
	if(runmode==0){
		char *content = "Put\n";
    	writeFile(filename2, content,"w+");				
    	writeFile(filename2, strx[x-1],"a");
    	writeFile(filename2, stry[y-1],"a");
   		writeFile(filename2, strc[c-1],"a");	
	}
	else if(runmode==1){
		char *content = "Move\n";
       	writeFile(filename2, content,"w+");				
    	writeFile(filename2, strx[x-1],"a");
   		writeFile(filename2, strc[y-1],"a");
		writeFile(filename2,strc[4],"a");
   		writeFile(filename2, strx[c-1],"a");
		writeFile(filename2, strc[d-1],"a");
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
    	n++;
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
        	n++;
        	fgets(temp, 1000, fp);//下一列 
        	if(strstr(temp,"Put")){
        		fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				c=temp[5]-'0';	
				piece[x][y]=piece[x][y]*10+c;
			}
        	else if(strstr(temp,"Move")){
				fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';				
				c=temp[6]-'0';
				d=temp[8]-'0'; 
				temp2=piece[x][y]%10;
				piece[x][y]=piece[x][y]/10;
				temp4=piece[c][d]%10;
				piece[c][d]=piece[c][d]*10+temp2;
			}
        }
        
       	else if(strstr(temp,"Brown")){
       		n++;
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
				temp2=piece[x][y]%10; 
				piece[x][y]=piece[x][y]/10;
				temp4=piece[c][d]%10;
				piece[c][d]=piece[c][d]*10+temp2;
			}  
    	}
    own=n;
    }
    fclose(fp);
    return return_string;	
}
int compute_x(int xc,int temps[5][5]){
	int i,j,count=0,risk=0;
	for(j=1;j<5;j++){
		if(temps[xc][j]%10<5&&temps[xc][j]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[xc][j]%10>4){ 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	score[xc]=score[xc]+count*3;
	score[xc]=score[xc]-risk;
	if(count==3)
		score[xc]+=100;
	if(count==4)
		score[xc]+=2000;
	if(risk==3){
		score[xc]-=1500;
		listen[xc]=1;
	}
	if(risk==3&&count==1)
		score[xc]+=500;
	if(risk==4)
		score[xc]-=50000;
	count=risk=0;
	if(test==1);
}
int compute_y(int yc,int temps[5][5]){
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[i][yc-4]%10<5&&temps[i][yc-4]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[i][yc-4]%10>4){
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	score[yc]+=count*3;
	score[yc]=score[yc]-risk;
	if(count==3)
		score[yc]+=100;
	if(count==4)
		score[yc]+=2000;
	if(risk==3){ 
		score[yc]-=1500;
		listen[yc]=1;
	}
	if(risk==3&&count==1)
		score[yc]+=500;
	if(risk==4)
		score[yc]-=50000; 
	count=risk=0;
	if(test==1);
}

int compute_9(int temps[5][5]){
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[i][i]%10<5&&temps[i][i]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[i][i]%10>4){
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	score[9]+=count*3;
	score[9]=score[9]-risk;
	if(count==3)
		score[9]+=100;
	if(count==4)
		score[9]+=2000; 
	if(risk==3){
		score[9]-=1500;
		listen[9]=1;
	}
	if(risk==3&&count==1)
		score[9]+=500;
	if(risk==4)
		score[9]-=50000;
	count=risk=0;
	if(test==1);
}
int compute_10(int temps[5][5]){
	int i,j,count=0,risk=0;
	for(i=1;i<5;i++){
		if(temps[4-i+1][i]%10<5&&temps[4-i+1][i]%10>0){
			if(player==0)
				count++;
			else
				risk++;
		}
		else if(temps[4-i+1][i]%10>4){ 
			if(player==0)
				risk++;
			else
				count++;
		}
	}
	score[10]+=count*3;
	score[10]=score[10]-risk; 
	if(count==3)
		score[10]+=100;
	if(count==4)
		score[10]+=2000;
	if(risk==3){
		score[10]-=1500;
		listen[10]=1; 
	}
	if(risk==3&&count==1)
		score[10]+=500;
	if(risk==4)
		score[10]-=50000; 
	count=risk=0;
	if(test==1);
}


void total_score(int temps[5][5]){
	memset(score,0,11*sizeof(int)); //歸零 
	total=0;
	for(i=1;i<5;i++){
		compute_x(i,temps);
		compute_y(i+4,temps);
	}
	compute_9(temps);
	compute_10(temps);
	for(j=1;j<11;j++)//合計1~10線
		if(score[j]!=0)
			total+=score[j];
	fprintf(stderr,"%d score:%d\n",player,total);
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

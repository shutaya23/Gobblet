#include<stdlib.h>
#include<stdio.h>
#include<string.h>
char* readFile(char *filename);
char* readlog(char *filename);
void writeFile(char *filename, char *content,char *mode);
int max(int n1,int n2,int n3);
int compute_x(int xc,int temps[5][5]);
int compute_y(int yc,int temps[5][5]);
void total_score(int temps[5][5]);
int piece[5][5];//棋盤 1~4大小
int piece_temp[5][5];
int hand1,hand2,hand3;
char *hand;//手排字串 
int i,j,total=0,max_total=-99999;
int runmode=0;//put,move
int player,own=0;
int x,y,c=0,d=0;//下點 
int score[11];//總分
int bad_point[11];
int test=0;


int main(int argc,char *argv[] )
{
	hand1=hand2=hand3=0;
	for(i=1;i<5;i++){
		for(j=1;j<5;j++){		
			piece[i][j]=0;
			//fprintf(stderr,"%d ",piece[i][j]);
		}	
	}
	char *filename = argv[1];
    char *filecontent = readFile(filename);
    //算手牌 
	if(player==1){
        hand=strstr(filecontent,"Brown Hand");
        fprintf(stderr,"hand: %s\n",hand);
		}
	else{
		hand=strstr(filecontent,"Yellow Hand");
        fprintf(stderr,"hand: %s\n",hand);
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
	fprintf(stderr,"%d hand: %d %d %d\n",player,hand1,hand2,hand3);
	free(dupstr);	      
    
    //建立當前棋譜 
    char *filename1 = argv[2];
    char *filecontent1 = readlog(filename1);
    //空白策略 
	for(i=1;i<5;i++){
		for(j=1;j<5;j++){		
			if(piece[i][j]==0){
				x=i;y=j;
				break;
			}
		}
	}
	c=max(hand1,hand2,hand3);//決定棋大小
	//if(c) 
	//	piece[x][y]=c;//棋
	
		
	//先手 
	if(own==0){//&&player=0
		for(i=2;i<4;i++){
			for(j=2;j<4;j++){		
				if(piece[i][j]==0){
					//x=i;y=j;
				 	break;}
			}		
		}	
	}
	memcpy(piece_temp,piece,sizeof(piece));
	//if(c!=0){//手牌優先 
		int i=0,j=0;
		for(i=1;i<5;i++){
			for(j=1;j<5;j++){		
				if(piece[i][j]==0){
					memcpy(piece_temp,piece,sizeof(piece));					
					if(player==0)
						piece_temp[i][j]=piece_temp[i][j]*10+c;
					else
						piece_temp[i][j]=piece_temp[i][j]*10+c*2;
					total_score(piece_temp);
					if(total>max_total){
						max_total=total;
						x=i;y=j;
						fprintf(stderr,"\n big %d,%d\n",x,y);
					}
				}
				else if(piece[i][j]<c){
					;//聽牌 
				}
			}	
		}
	//}

	//move
	int k=1,r=1;
	for(k=1;k<5;k++){
		for(r=1;r<5;r++){
			if(piece[k][r]%10<5&&player==0){//黃
					int i=0,j=0,t=0;
					t=piece[k][r]%10;				
					for(i=1;i<5;i++){
						for(j=1;j<5;j++){
							if(piece[i][j]%10>4&&t>piece[i][j]%10/2){
								memcpy(piece_temp,piece,sizeof(piece));
								piece_temp[k][r]/=10;
								piece_temp[i][j]*=10+t;							
								total_score(piece_temp);
								if(total>max_total){
									max_total=total;
									x=k;y=r;
									c=i;d=j;
									runmode=1;
									fprintf(stderr,"\n big move %d,%d to %d,%d\n",x,y,c,d);
								}								
							}				
						}
					}

			} 
		}
	}
			
	
	//算當前分
	test=1;
	fprintf(stderr,"\n now\n");
	total_score(piece);
	test=0;
	
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
		fprintf(stderr,"player:%d %s%s%s",player,strx[x-1],stry[y-1],strc[c-1]);
	}
	else if(runmode==1){
		char *content = "Move\n";
       	writeFile(filename2, content,"w+");				
    	writeFile(filename2, strx[x-1],"a");//x
   		writeFile(filename2, strc[y-1],"a");//y
		writeFile(filename2,strc[4],"a");//\n
   		writeFile(filename2, strx[c-1],"a");//c
		writeFile(filename2, strc[d-1],"a");//d	
		fprintf(stderr,"player:%d %s%s%s",player,strx[x-1],stry[y-1],strc[c-1]);
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
        fprintf(stderr,"couldn't open game.txt:\n ");
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
        if(n==1)
        	if(!strstr(return_string,"Player Yellow"))
				player=1;//brown        		

    }
    fclose(fp);
    fprintf(stderr,"%d line: %s\n ",n,return_string);
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
        fprintf(stderr,"couldn't open game.txt:\n ");
        return return_string;
    }
    char *temp3 = "";
    while(fgets(temp, 1000, fp)!=NULL)
    { 	
    	int temp2,n=0;
        return_string = (char*)malloc((strlen(temp)+strlen(return_string))*sizeof(char));//
        strcpy(return_string,temp3);
        temp3 = (char*)malloc((strlen(temp)+strlen(temp3))*sizeof(char));//
        strcat(return_string,temp);
        strcpy(temp3,return_string);        
        if(strstr(temp,"Yellow")){
        	n++;//棋數
        	fgets(temp, 1000, fp);//下一列 
        	//fprintf(stderr,"find y : ");
        	if(strstr(temp,"Put")){
        		//fprintf(stderr,"Put  ");
        		fgets(temp, 1000, fp);
        		//fprintf(stderr,"%s ",temp);
        		//分割 
				x=temp[1]-'0';
				y=temp[3]-'0';
				c=temp[5]-'0';	
				piece[x][y]=piece[x][y]*10+c;//下棋lsb表外層 
				fprintf(stderr,"%d Put: %d %d %d\n",player,x,y,piece[x][y]);
        		
			}
        	else{
				fprintf(stderr,"Move\n ");
				fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				fgets(temp, 1000, fp);				
				c=temp[1]-'0';
				d=temp[3]-'0'; 
				temp2=piece[x][y]%10; //移動棋
				piece[x][y]/=10;
				piece[c][d]=piece[c][d]*10+temp2*2;
				fprintf(stderr,"%d move: (%d %d) (%d %d)\n",player,x,y,c,d);
			}
        }
        
       	if(strstr(temp,"Brown")){
       		n++;//棋數
			fgets(temp, 1000, fp);//下一列 
        	if(strstr(temp,"Put")){
        		fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				c=temp[5]-'0';
				piece[x][y]=piece[x][y]*10+c*2;
				fprintf(stderr,"%d Put: %d %d %d\n",player,x,y,piece[x][y]);
			}
        	else{
				fprintf(stderr,"Move\n ");
				fgets(temp, 1000, fp);
				x=temp[1]-'0';
				y=temp[3]-'0';
				fgets(temp, 1000, fp);				
				c=temp[1]-'0';
				d=temp[3]-'0';
				temp2=piece[x][y]%10; //移動棋
				piece[x][y]/=10;
				piece[c][d]=piece[c][d]*10+temp2*2;
				fprintf(stderr,"%d move: (%d %d) (%d %d)\n",player,x,y,c,d);
			}  
    	}
    own=n;//非手邊棋數 
    }
    
    fclose(fp);
    //fprintf(stderr,"\n%d line: %s\n ",n,return_string);
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
	score[xc]=score[xc]+count*3;//優勢 
	score[xc]=score[xc]-risk;//劣勢 
	if(count==3)
		score[xc]+=100;//決勝
	if(count==4)
		score[xc]+=2000;//勝 
	if(risk==3)
		score[xc]-=1000;//敗北
	if(risk==3&&count==1)
		score[xc]+=3000;
	if(risk==3&&count==1&&c==4)
		score[xc]+=4000;
	if(risk==4)
		score[xc]-=5000;//敗北
	count=risk=0;
	if(test==1)
		fprintf(stderr,"%d xc:%d %d\n ",player,xc,score[xc]);
	
	
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
	score[yc]+=count*3;//優勢 
	score[yc]=score[yc]-risk;//劣勢 
	if(count==3)
		score[yc]+=100;//決勝
	if(count==4)
		score[yc]+=2000;//決勝 
	if(risk==3)
		score[yc]-=1000;//敗北
	if(risk==3&&count==1)
		score[yc]+=3000;
	if(risk==3&&count==1&&c==4)
		score[yc]+=4000;	
	if(risk==3)
		score[yc]-=5000;//敗北 
	count=risk=0;
	if(test==1)
		fprintf(stderr,"%d yc:%d %d\n ",player,yc,score[yc]);
}

void total_score(int temps[5][5]){//算總分
	memset(score,0,11*sizeof(int)); //歸零 
	total=0;
	for(i=1;i<5;i++){
		compute_x(i,temps);
		compute_y(i+4,temps);
	}
	for(j=1;j<9;j++)//合計1~8線
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#define MAX 40	//maximo de cartas no baralho
#define PLAYERS 8	//maximo de jogadores
#define HP 5	//alterar vida dos jogadores aqui

typedef struct{	//struct das cartas
	int num;
	int symbol;
}carta;

typedef struct{	//struct para jogador e lista para a mão do mesmo
	char nome[MAX/2];
	int vida;
	carta mao[5];
	int fim;
}jogador;

typedef struct{	//struct para o baralho
	int top;
	carta card[MAX];
}pilha;

typedef struct{	//struct para a mesa
	carta v[PLAYERS];
	int inicio,fim,tam;
}fila;

pilha* criar_baralho();	//função para inicializar baralho
void embaralhar(pilha *p);	//função de embaralhar o baralho
jogador criar_jogador(char nome[],int c);	//inicia 1 jogador
fila* criar_mesa();	//função para inicializar a mesa
int vazia_p(pilha *p);	//verifica baralho vazio
int vazia_f(fila *f);	//verifica mesa vazia
int vazia_l(jogador j);	//verifica mao do jogador vazia
int cheia_p(pilha *p);	//verifica baralho cheio
int cheia_f(fila *f);	//verifica mesa cheia
int cheia_l(jogador j);	//verifica mao do jogador cheia
int empilha(pilha *p,carta x);	//empilha no baralho
int despilha(pilha *p,carta *x);	//desempilha do baralho
int enfila(fila *f,carta x);	//enfileira carta na mesa
int desfila(fila *f,carta *x);	//desenfileira carta na mesa
int enlista(jogador j[],int np,int n,carta x);	//emlista na mao do jogador
int deslista_ult(jogador j[],int np,int n,carta *x);	//remove uma carta da ultima posição da mao do jogador
int deslista_pos(jogador j[],int np,int n,carta *x,int pos);	//remove uma carta q o jogador escolher por posição
void sacar(jogador j[],int np,int n,pilha *p);	//entrega 1 carta do baralho para o jogador
void ordenar_mao(jogador j[],int np,int n);	//ordena as cartas da mão em ordem crescente
void converter_card(int num,int sym,char *k,int *l);	//converter os valores de numero e naipe para mostrar ao usuário
void mostrar_mao(jogador j[],int np,int n);	//printa a mão de um jogador
void mostrar_mesa(fila *f);	//printa mesa
void init_jogadores(jogador j[],int np);	//criar jogadores vezes n de jogadores
void limpar_apostas(int bet[],int np);	// limpar lixo de memoria das apostas de cada jogador
int calcular_poder(fila *f,int pos);	//calcula a força das cartas e retorna esse valor
int comparar(fila *f,int p,int q);	//compara o poder de duas cartas e retorna a maior
void limpar_mesa(fila *f,pilha *p);	//descarta as cartas da mesa para o baralho
void limpar_twin(int twin[],int np);	//limpar lixo de memoria dos turnos ganhos de cada jogador
void remover_jogador(jogador j[],int np,int perdedor);	//remove jogador que perder todas as vidas
int alterar_ncartas(int *ncartas,int acress);	//aumenta e diminui o n° de cartas entregues em cada rodada
void fmostrar_mao(FILE *arq,jogador j[],int np,int n);	//printa no arquivo a mao do jogador
void fmostrar_mesa(FILE *arq,fila *f);	//printa no arquivo a mesa
void menu();	//função principal

int main() {
	setlocale(LC_ALL,"Portuguese");
	
	menu();
	
	FILE *arq=fopen("log","a");
	fprintf(arq,"\nFinal do Registro;\n");
	fclose(arq);
	
	return 0;
}

pilha* criar_baralho(){
	int i,j=1;
	pilha *p=(pilha*)malloc(sizeof(pilha));
	p->top=0;
	
	for(j=1;j<5;j++){
		for(i=0;i<10;i++){
			p->card[i+((j-1)*10)].num=i;
			p->card[i+((j-1)*10)].symbol=j;
			p->top++;
		}
	}
	return p;
}

void embaralhar(pilha *p){
	int i,pos;
	carta aux,v[40];
	
	srand((unsigned)time(NULL));
	
	for(i=0;i<MAX;i++){
		despilha(p,&aux);
		v[i]=aux;
	}
	
	for(i=0;i<MAX;i++){
		pos=rand()%40;
		
		aux=v[i];
		v[i]=v[pos];
		v[pos]=aux;
	}
	
	for(i=0;i<MAX;i++){
		empilha(p,v[i]);
	}
}

jogador criar_jogador(char nome[],int c){
	jogador novo;
	
	novo.vida=HP;
	strcpy(novo.nome,nome);
	novo.fim=0;
	
	return novo;
}

fila* criar_mesa(){
	fila *f=(fila*)malloc(sizeof(fila));
	
	f->inicio=0;
	f->fim=0;
	f->tam=0;
	
	return f;
}

int vazia_p(pilha *p){
	if(p->top==0){
		return 1;	//esta vazia
	}
	return 0;
}

int vazia_f(fila *f){
	if(f->tam==0){
		return 1;	//esta vazia
	}
	return 0;
}

int vazia_l(jogador j){
	if(j.fim==0){
		return 1;	//esta vazia
	}
	return 0;
}

int cheia_p(pilha *p){
	if(p->top>39){
		return 1; 	//esta cheia
	}
	return 0;
}

int cheia_f(fila *f){
	if(f->tam==PLAYERS){
		return 1;	//esta cheia
	}
	return 0;
}

int cheia_l(jogador j){
	if(j.fim>4){
		return 1;	//esta cheia
	}
	return 0;
}

int empilha(pilha *p,carta x){
	if(cheia_p(p)==0){
		p->card[p->top]=x;
		p->top++;
		return 1;
	}
	return 0;
}

int despilha(pilha *p,carta *x){
	if(vazia_p(p)==0){
		p->top--;
		*x=p->card[p->top];
		return 1;
	}
	return 0;
}

int enfila(fila *f,carta x){
	if(cheia_f(f)==0){
		f->fim=f->fim%PLAYERS;	//circular posição final
		f->v[f->fim]=x;
		f->fim++;
		f->tam++;
		return 1;
	}
	return 0;
}

int desfila(fila *f,carta *x){
	if(vazia_f(f)==0){
		f->inicio=f->inicio%PLAYERS;	//circular posição inicial
		*x=f->v[f->inicio];
		f->inicio++;
		f->tam--;
		return 1;
	}
	return 0;
}

int enlista(jogador j[],int np,int n,carta x){
	if(cheia_l(j[n])==0){
		j[n].mao[j[n].fim]=x;
		j[n].fim++;
		return 1;
	}
	return 0;
}

int deslista_ult(jogador j[],int np,int n,carta *x){
	if(vazia_l(j[n])==0){
		j[n].fim--;
		*x=j[n].mao[j[n].fim];
		return 1;
	}
	return 0;
}

int deslista_pos(jogador j[],int np,int n,carta *x,int pos){	
	//ps: lembrar de chamar a função com posição em x-1, para ser mais funcional ao usuario
	int i;
	if(vazia_l(j[n])==0){
		*x=j[n].mao[pos];
		for(i=pos+1;i<j[n].fim;i++){
			j[n].mao[i-1]=j[n].mao[i];
		}
		j[n].fim--;
		return 1;
	}
	return 0;
}

void sacar(jogador j[],int np,int n,pilha *p){
	carta sv;
	despilha(p,&sv);
	enlista(j,np,n,sv);
}

void ordenar_mao(jogador j[],int np,int n){	
	int i,k;
	carta aux;
	
	for(i=1;i<j[n].fim;i++){	//ordena os naipes
		aux=j[n].mao[i];
		k=i-1;
		while(aux.symbol<j[n].mao[k].symbol && k>=0){
			j[n].mao[k+1]=j[n].mao[k];
			k--;
		}
		j[n].mao[k+1]=aux;
	}
	
	for(i=1;i<j[n].fim;i++){	//ordena os numeros sem desordenar os naipes
		aux=j[n].mao[i];
		k=i-1;
		while(aux.num<j[n].mao[k].num && k>=0 && aux.symbol<=j[n].mao[k].symbol){
				j[n].mao[k+1]=j[n].mao[k];
				k--;	
		}
		j[n].mao[k+1]=aux;
	}
}

void converter_card(int num,int sym,char *k,int *l){
	switch(num){
		case 0:
			*k='4';
		break;
		case 1:
			*k='5';
		break;
		case 2:
			*k='6';
		break;
		case 3:
			*k='7';
		break;
		case 4:
			*k='Q';
		break;
		case 5:
			*k='J';
		break;
		case 6:
			*k='K';
		break;
		case 7:
			*k='A';
		break;
		case 8:
			*k='2';
		break;
		case 9:
			*k='3';
		break;
	}
	switch(sym){
		case 1:
			*l=4;
		break;
		case 2:
			*l=6;
		break;
		case 3:
			*l=3;
		break;
		case 4:
			*l=5;
		break;
	}
}

void mostrar_mao(jogador j[],int np,int n){
	int i,l;
	char k;
	for(i=0;i<j[n].fim;i++){
		converter_card(j[n].mao[i].num,j[n].mao[i].symbol,&k,&l);
		printf("Carta %d: %c %c\n",i+1,k,l);
	}
}

void mostrar_mesa(fila *f){
	int i,j=f->inicio,l;
	char k;
	if(f->tam==0){
		printf("Vazia\n\n");
	}
	else{
		for(i=0;i<f->tam;i++){
			j%=PLAYERS;
			converter_card(f->v[j].num,f->v[j].symbol,&k,&l);
			printf("Carta %c %c\t",k,l);
			j++;
		}
		printf("\n\n");
	}
}

void init_jogadores(jogador j[],int np){
	char nome[MAX/2];
	int i;
	
	for(i=0;i<np;i++){
		printf("nome do jogador %d: ",i+1);
		setbuf(stdin,NULL);
		gets(nome);
		
		j[i]=criar_jogador(nome,MAX/2);
	}
}

void limpar_apostas(int bet[],int np){
	int i;
	for(i=0;i<np;i++){
		bet[i]=-999;
	}
}

int calcular_poder(fila *f,int pos){
	int poder;
	
	if(f->v[pos].num==0 && f->v[pos].symbol==4){	//zap
		poder=8000;
	}
	else if(f->v[pos].num==3 && f->v[pos].symbol==3){	//copas
		poder=4000;
	}
	else if(f->v[pos].num==7 && f->v[pos].symbol==2){	//espadilha
		poder=2000;
	}
	else if(f->v[pos].num==3 && f->v[pos].symbol==1){	//ouro
		poder=1000;
	}
	else{
		poder=f->v[pos].num;	//numero da carta 4==poder 0 até 3==poder 9
	}
	return poder;
}

int comparar(fila *f,int p,int q){
	int poder[2];
	poder[0]=calcular_poder(f,p);
	poder[1]=calcular_poder(f,q);
	
	if(poder[0]>poder[1]){
		return p;
	}
	else if(poder[0]<poder[1]){
		return q;
	}
	else{	//mesmo numero verifica por naipe
		if(f->v[p].symbol>f->v[q].symbol){
			return p;
		}
		else{
			return q;
		}
	}
}

void limpar_mesa(fila *f,pilha *p){
	carta hold;
	while(f->tam!=0){
		desfila(f,&hold);
		empilha(p,hold);
	}
}

void limpar_twin(int twin[],int np){
	int i;
	for(i=0;i<np;i++){
		twin[i]=0;
	}
}

void remover_jogador(jogador j[],int np,int perdedor){
	int i;
	if(np!=0){	//se ainda tem jogadores
		for(i=perdedor;i<np;i++){
			j[i]=j[i+1];	//deleta perdedor
		}
		//lembrar de reduzir np
	}
}

int alterar_ncartas(int *ncartas,int acress){
	if(acress==0){
		*ncartas=*ncartas-1;
	}
	else{
		*ncartas=*ncartas+1;
	}
	if(*ncartas==1){
		acress=1;
	}
	else if(*ncartas==5){
		acress=0;
	}
	return acress;
}

void fmostrar_mao(FILE *arq,jogador j[],int np,int n){
	int i,l;
	char k;
	for(i=0;i<j[n].fim;i++){
		converter_card(j[n].mao[i].num,j[n].mao[i].symbol,&k,&l);
		fprintf(arq,"Carta %d: %c %d\n",i+1,k,j[n].mao[i].symbol);	//arquivo n mostra os naipes
	}
}

void fmostrar_mesa(FILE *arq,fila *f){
	int i,j=f->inicio,l;
	char k;
	if(f->tam==0){
		fprintf(arq,"Vazia\n\n");
	}
	else{
		for(i=0;i<f->tam;i++){
			j%=PLAYERS;
			converter_card(f->v[j].num,f->v[j].symbol,&k,&l);
			fprintf(arq,"Carta %c %d\t",k,f->v[j].symbol);
			j++;
		}
		fprintf(arq,"\n");
	}
}

void menu(){
	pilha *p=criar_baralho();
	fila *f=criar_mesa();
	int np,i,k,w,win=0,ncartas=5,first=0,last,select,mais_forte,qturno,acress=0,supint;
	char supchar;
	carta aux;
	FILE *arq=fopen("log","w");	//criando registro da partida
	fprintf(arq,"Registro de partida FDP;\n\n");
	printf("**********************************\n\tTrabalho Prático 1\n\n    Feito por João Vitor Neves\n**********************************\n\n");
	printf("Digite o número de jogadores: ");
	scanf("%d",&np);
	
	fprintf(arq,"%d jogadores\n\n",np);
	fclose(arq);
	
	jogador *j=(jogador*)malloc(np*sizeof(jogador)),*podio=(jogador*)malloc(np*sizeof(jogador));	//jogadores e podio / lembrar de dar free
	int perdedores=0;	// auxilia podio
	
	init_jogadores(j,np);
	int credito,*bet=(int*)malloc(np*sizeof(int)),*twin=(int*)malloc(np*sizeof(int));
	
	while(win!=1){
		printf("Rodada #%d\n%d Cartas\n",first+1,ncartas);	//talvez substituir rodada por first+1
		embaralhar(p);
		for(i=first%np;i<np+(first%np);i++){// entregar as cartas / lembrar de sempre usar first%np
			for(k=0;k<ncartas;k++){
				sacar(j,np,i%np,p);	//lembrar de alterar a qtd de cartas no final da rodada
				ordenar_mao(j,np,i%np);
			}
		}
		arq=fopen("log","a");
		fprintf(arq,"Rodada #%d\n%d Cartas\n\n",first+1,ncartas);
		fclose(arq);	//sempre necessário fechar o arquivo, pois o system cls tambem limpa o arquivo
		system("pause");
		system("cls");
		credito=ncartas;	//o credito serve para que sempre aja alguem com chance de perder vida
		limpar_apostas(bet,np);	//define apostas como -999 para n receber lixo das rodadas anteriores
		limpar_twin(twin,np);	//define turnos ganhos como 0 para n receber lixo das rodadas anteriores
		last=first-1;	//determinar ultimo jogador
		if(last==-1){
			last=np-1;
		}
		
		i=first%np;
		for(w=first%np;w<np+(first%np);w++){	//apostas / Em vez de usar duas variaveis, tbm era possivel usar i sempre seguido de %np
			i%=np;
			printf("Vidas: %d\n",j[i].vida);
			mostrar_mao(j,np,i);
			printf("Jogador %s, quantas vitorias você faz: ",j[i].nome);
			while(bet[last%np]==credito || bet[i]>ncartas || bet[i]<0){	//impede que a aposta do ultimo for igual a sobra de creditos, ou as apostas serem um numero n funcional
				scanf("%d",&bet[i]);
				if(bet[last%np]==credito || bet[i]>ncartas || bet[i]<0){
					printf("Quantidade inválida! Tente outro valor.\n");
				}
			}
			credito-=bet[i];
			arq=fopen("log","a");
			fprintf(arq,"Jogador %s\nVidas: %d\n",j[i].nome,j[i].vida);	//printa no arquivo nome, vidas, mao e aposta
			fmostrar_mao(arq,j,np,i);
			fprintf(arq,"Aposta: %d\n\n",bet[i]);
			fclose(arq);
			i++;
			system("cls");
		}
		
		for(i=0;i<ncartas;i++){	//jogar cartas
			printf("Turno %d:\n\n",i+1);
			k=first%np;
			for(w=first%np;w<np+(first%np);w++){	//mostra a mão e a situação da mesa
				k%=np;
				printf("Mesa:\n");
				mostrar_mesa(f);
				printf("Jogador %s;\nEscolha carta para jogar na mesa:\n",j[k].nome);
				mostrar_mao(j,np,k);
				do{
					scanf("%d",&select);
					if(select>ncartas || select<1){
						printf("Opção inválida, selecione outra carta:\n");
					}
					printf("\n");
				}while(select>ncartas || select<1);	//imposibilita de escolher uma opção inviável
				deslista_pos(j,np,k,&aux,select-1);
				enfila(f,aux);
				k++;
				system("cls");
			}
			arq=fopen("log","a");
			fprintf(arq,"Turno %d\nMesa:\n",i+1);	//printa no arquivo a mesa no final do turno
			fmostrar_mesa(arq,f);
			mais_forte=f->inicio%PLAYERS;
			k=f->inicio+1;
			qturno=first;
			for(w=0;w<f->tam-1;w++){	//comparações
				k%=PLAYERS;
				converter_card(f->v[mais_forte].num,f->v[mais_forte].symbol,&supchar,&supint);	//converte cartas para mostrar ao usuario
				printf("%c %c é ",supchar,supint);	//prints são testes, remover dps
				mais_forte=comparar(f,mais_forte,k);
				converter_card(f->v[k].num,f->v[k].symbol,&supchar,&supint);
				if(mais_forte!=k){
					printf("mais forte que %c %c.\n",supchar,supint);
				}
				else if(mais_forte==k){
					printf("mais fraco que %c %c.\n",supchar,supint);
					qturno=(first+w+1)%np;	//configura a nova posição do mais forte
				}
				k++;
			}
			twin[qturno]++;	//contabiliza n de turnos ganhos
			printf("\nJogador %s ganhou este turno!\n",j[qturno].nome);
			fprintf(arq,"Jogador %s ganhou este turno!\n\n",j[qturno].nome);
			fclose(arq);
			limpar_mesa(f,p);	//devolve cartas para o baralho
			system("pause");
			system("cls");
			printf("\n");
		}
		i=first%np;
		arq=fopen("log","a");
		for(w=first%np;w<np+(first%np);w++){	//remover vida de apostas falhas
			i%=np;
			printf("Jogador %s apostou %d e ganhou %d turnos.\n",j[i].nome,bet[i],twin[i]);
			fprintf(arq,"Jogador %s apostou %d e ganhou %d turnos.\n",j[i].nome,bet[i],twin[i]);
			if(twin[i]!=bet[i]){	//compara as jogadas ganhos com a aposta
				printf("jogador %s perde uma vida!\n",j[i].nome);
				fprintf(arq,"jogador %s perde uma vida!\n",j[i].nome);
				j[i].vida--;	//mostra hp no inicio das rodadas
			}
			printf("\n");
			i++;
		}
		fprintf(arq,"\n");
		//talvez criar pódio;acrescentar rodada;verificar vencedor;alterar quantidade de cartas
		i=first%np;
		for(w=first%np;w<np+(first%np);w++){	//verifica perdedor / vida==0	/ fazer todas as verificações de perdedores antes de decrementar np
			i%=np;
			if(j[i].vida<=0 && np!=1){
				podio[perdedores]=j[i];
				remover_jogador(j,np,i);
				np--;	//diminui jogadores
				printf("jogador %s foi removido do jogo\n",podio[perdedores].nome);
				fprintf(arq,"jogador %s foi removido do jogo\n",podio[perdedores].nome);
				perdedores++;	//avança podio
				w=(first-1)%np;	//no fim da repetição o contador é acrescido de 1
				i=first-1;	//resetar a contagem de posições considerando acrescimo no fim do codigo
			}
			i++;
		}
		fclose(arq);
		if(np==1){	//se sobrou apenas um jogador
			podio[perdedores]=j[0];
			printf("Jogador %s ganhou o jogo com %d vidas restantes.Parabéns!!\n\n",podio[perdedores].nome,podio[perdedores].vida);
			arq=fopen("log","a");
			fprintf(arq,"Jogador %s ganhou o jogo com %d vidas restantes.Parabéns!!\n\nPódio:\n",podio[perdedores].nome,podio[perdedores].vida);
			for(i=perdedores;i>=0;i--){	//imprime o podio no final do arquivo
				fprintf(arq,"%d° Lugar: %s\n",(perdedores+1)-i,podio[i].nome);
			}
			fclose(arq);
			free(j);	//liberar todos os ponteiros
			free(f);
			free(p);
			free(bet);
			free(twin);
			free(podio);
			win=1;	//sair do loop principal
		}
		acress=alterar_ncartas(&ncartas,acress);	//altera quantidade de cartas a ser distribuidas na proxima rodada
		first++;	//avança rodada e primeiro jogador
		
		system("pause");
		system("cls");
	}
}

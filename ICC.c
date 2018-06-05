#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#define NUMP 2
#define NUMT 3
#define TAMNOMES 100

/*Desenvolvido por, na ordem de atribuição de notas:
1 - Igor Guedes Rodrigues (9771654);
2 - Augusto Ribeiro Castro (9771421);
3 - Alexandre Batistella Bellas (9763168).
*/

/*----------------------------//Variáveis Globais//------------------------*/
char nomes[] = "./Alunos/Nomes.txt";

/*-------------------------------//Structs//-------------------------------*/

struct informacoes{
    char nome[TAMNOMES];
    unsigned int numero_usp;
    double nota_prova[NUMP];
    double nota_trab[NUMT];
    int posicao[NUMT];
    char nome_grupo[NUMT][TAMNOMES];
    int contola_nome[NUMT];//Booleana, começa em 0 e quando um nome de grupo for inserido, recebe 1.
};

/*---------------------------//INSTRUÇÕES DO ARQUIVO//---------------------/
Legenda: Nx - numero do grupo da pessoa no trabalho x
         NOTAx - nota da pessoa no trabalho x (já sem o desconto incluido)
         POSx - posicao da pessoa no trabalho x
         NotaPy - nota da pessoa na prova y

(Nome) (N1) (NOTA1) (POS1) (N2) (NOTA2) (POS2) (N3) (NOTA3) (POS3) (NotaP1) (NotaP2)
*/

/*-------------------------------//Funções//-------------------------------*/
//Limpar tela do prompt
void limpar_tela(){
    system("cls");
}

//Para abrir as artes da interface textual:
void arte(char *nome) {
    //Contador
    int i;

    //Abrir arquivo
    FILE *arq;
    arq = fopen(nome, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo. O programa sera encerrado.\n");
        exit(0);
    }

    //Contar tamanho do arquivo
    fseek(arq, 0, SEEK_END);
    int size = ftell(arq);

    //Voltar ao começo do arquivo
    fseek(arq, 0, SEEK_SET);

    //Ler arquivo e printar na tela
    for (i = 0; i < size; i++) {
        printf("%c", fgetc(arq));
        //Opcao de efeito de digitacao
        //Sleep(15);
    }
    fclose(arq);

    //Ha um bugzinho quando vc fecha o arquivo, dando um espaço a mais. A gnt burla ele com um enter
    printf("\n");
}

int numero_de_alunos(){
    FILE *arquivo = fopen(nomes, "rb");
    if (!arquivo){
        printf("Nao foi possivel abrir o arquivo Nomes.txt. Erro x0963219.\nConsulte o administrador.");
        exit(-1);
    }
    fseek(arquivo, 0, SEEK_END);
    int size = ftell(arquivo);
    fclose(arquivo);
    int cont = size/(sizeof(struct informacoes));
    return cont;
}

void incluir_alunos() {
    FILE *arquivo = fopen(nomes, "ab");
    if (!arquivo){
        printf("Nao foi possivel abrir o arquivo Nomes.txt. Erro x09124619.\nConsulte o administrador.");
        exit(-1);
    }
    printf("Primeiramente, digite o nome do aluno que voce deseja incluir:\n");
    struct informacoes novoaluno;
    scanf("%[A-Z a-z 0-9]", &novoaluno.nome);
    fflush(stdin);
    printf("\nAgora, insira o numero USP de seu aluno: \n");
    scanf(" %d", &novoaluno.numero_usp);
    fflush(stdin);
    int i;
    for (i = 0; i < NUMT; i++) {
        if (i < NUMP) {
            novoaluno.nota_prova[i] = -1;
            novoaluno.nota_trab[i] = -1;
            novoaluno.posicao[i] = -1;
            novoaluno.contola_nome[i] = 0;
            strcpy(novoaluno.nome_grupo[i], "vazio");
        }
        else {
            novoaluno.nota_trab[i] = -1;
            novoaluno.posicao[i] = -1;
            novoaluno.contola_nome[i] = 0;
            strcpy(novoaluno.nome_grupo[i], "vazio");
        }
    }
    int k = fwrite(&novoaluno, sizeof(struct informacoes), 1, arquivo);
    if (!k)
        printf("Inseriu errado.\n");
    else{
        limpar_tela();
        printf("Aluno incluido com sucesso!\n\n");
    }
    fclose(arquivo);
}

void carrega_turma (struct informacoes turma[], int cont) {
    int i;
    int parametro = 0;
    FILE *arquivo = fopen(nomes, "rb");
    if (!arquivo){
        printf("Nao foi possivel abrir o arquivo Nomes.txt. Erro m512452.\nConsulte o administrador.");
        exit(-1);
    }
    for (i = 0; i < cont; i++) {
        parametro += fread(&turma[i], sizeof(struct informacoes), 1, arquivo);
    }
    if (parametro != cont)
        printf("Leu errado.\n");
    fclose(arquivo);
}

void printa_registrados (int cont, struct informacoes turma[]) {
    int i;
    printf("Estes sao os nomes dos alunos registrados:\n");
    for (i = 0; i < cont; i++) {
        printf("%s \n", turma[i].nome);
    }
}

int encontra_aluno (char aux[], int cont, struct informacoes turma[]) {
    int i = 0;
    while (strcmp(aux, turma[i].nome)) {
        if (i == cont) {
            limpar_tela();
            printf("O nome escolhido nao foi encontrado.\n\n");
            return -1;
        }
        i++;
    }
    return i;
}

void remover_alunos(int cont) {
    struct informacoes *turma = (struct informacoes*)(malloc(cont*sizeof(struct informacoes)));
    if (!turma){
        printf("Erro ao alocar memoria. Erro x105123582.\nConsulte o administrador.");
        exit(-1);
    }
    carrega_turma(turma, cont);
    printa_registrados(cont, turma);
    printf("\nInsira o nome completo do aluno que deseja remover:\n");
    char aux[TAMNOMES];
    scanf("%[A-Z a-z 0-9]", aux);
    fflush(stdin);
    int i = encontra_aluno(aux, cont, turma);
    if (i < 0)
        return;
    int k;
    int p = 0;
    FILE *arq = fopen(nomes, "wb");
    if (!arq){
        printf("Erro ao abrir o arquivo Nomes.txt. Erro o4120532.\nConsulte o administrador.");
        exit(-1);
    }
    for (k = 0; k < cont; k++) {
        if (k != i) {
            p += fwrite(&turma[k], sizeof(struct informacoes), 1, arq);
        }
    }
    if (p != cont-1)
        printf("Excluiu errado.\n");
    else{
        limpar_tela();
        printf("Excluido com sucesso!\n\n");
    }
    fclose(arq);
    free(turma);
}

void consultar_alunos(int cont){
    struct informacoes *turma = (struct informacoes*)(malloc(cont*sizeof(struct informacoes)));
    if (!turma){
        printf("O arquivo nao conseguiu alocar memoria. Erro x105123582.\nConsulte o administrador.");
        exit(-1);
    }
    carrega_turma(turma, cont);
    printa_registrados(cont, turma);
    printf("\nDigite o nome completo do aluno que deseja consultar:\n");
    char aux[TAMNOMES];
    scanf("%[A-Z a-z 0-9]", aux);
    fflush(stdin);
    int i = encontra_aluno(aux, cont, turma);
    if (i < 0)
        return;
    printf("\n\nNome: %s\nNumero USP: %d\n", turma[i].nome, turma[i].numero_usp);
    int j;
    for (j = 0; j < NUMP; j++) {
        if (turma[i].nota_prova[j] >= 0)
            printf("Nota P%d: %.1lf    \n", j+1, turma[i].nota_prova[j]);
        else
            printf("\nAinda nao ha notas da P%d para esse aluno!\n", j+1);
    }
    printf("\n");
    for (j = 0; j < NUMT; j++){
        if (turma[i].contola_nome[j])
            printf("Grupo no Trabalho %d: %s   Nota Trabalho %d: %.1lf   Posicao no Trabalho %d: %d\n", j+1, turma[i].nome_grupo[j], j+1, turma[i].nota_trab[j], j+1, turma[i].posicao[j]);
        else
            printf("Nao ha notas para esse aluno no trabalho %d.\n", j+1);
    }
    printf("\n");
    system("PAUSE");
    fflush(stdin);
    limpar_tela();
    free(turma);
}

//Caso queira mexer com os alunos da disciplina
void opcao_alunos(){
    int cont;
    while (1){
        cont = numero_de_alunos();
        if (cont == 0){
            printf("Voce ainda nao possui alunos registrados na disciplina.\n"
                   "Registre um agora mesmo!\n\n");
            incluir_alunos();
            cont = numero_de_alunos();
        }
        printf("Voce tem %d alunos registrados na disciplina.\n"
               "O que deseja fazer?\n\n", cont);
        printf("-Digite 'I' para incluir alunos;\n"
                "-Digite 'R' para remover alunos;\n"
                "-Digite 'C' para consultar alunos;\n"
                "-Digite 'V' para voltar ao menu principal.\n");
        char opcao;
        scanf("%c", &opcao);
        fflush(stdin);
        limpar_tela();
        switch (opcao){
            case 'I':
            case 'i':
                incluir_alunos();
                break;
            case 'R':
            case 'r':
                remover_alunos(cont);
                break;
            case 'C':
            case 'c':
                consultar_alunos(cont);
                break;
            case 'V':
            case 'v':
                limpar_tela();
                return;
        }
    }
}

//Caso queira mexer com as notas de provas dos alunos da disciplina
void opcao_provas(){
    int cont = numero_de_alunos();
    struct informacoes *turma = (struct informacoes*)(malloc(cont*sizeof(struct informacoes)));
    if (!turma){
        printf("Nao foi possivel alocar memoria. Erro m01256428.\nConsulte o menino da informatica.");
        exit(-1);
    }
    carrega_turma(turma, cont);
    printa_registrados(cont, turma);
    printf("\nDigite o nome completo do aluno desejado:\n");
    char aux[TAMNOMES];
    scanf("%[A-Z a-z 0-9]", aux);
    fflush(stdin);
    int j = encontra_aluno(aux, cont, turma);
    if (j < 0)
        return;
    limpar_tela();
    int i;
    printf("Digite qual prova voce deseja conferir: 1 a %d.\n", NUMP);
    scanf("%d", &i);
    fflush(stdin);
    while (i > NUMP) {
        printf("Apenas %d provas podem ser conferidas. Tente novamente: \n", NUMP);
        scanf("%d", &i);
        fflush(stdin);
    }
    limpar_tela();
    char opcao;
    printf("A respeito da prova %d e do aluno %s,\n"
           "-Digite 'I' para incluir nota;\n"
           "-Digite 'R' para remover nota;\n"
           "-Digite 'C' para consultar nota;\n"
           "-Digite 'V' para voltar ao menu principal.\n", i, turma[j].nome);
    scanf("%c", &opcao);
    fflush(stdin);
    limpar_tela();
    if (opcao == 'V' || opcao == 'v'){
        return;
    }
    printf("Nome do aluno: %s\n\n", turma[j].nome);
    switch (opcao) {
        case 'C':
        case 'c':
            if (turma[j].nota_prova[i-1] == -1){
                printf("Ainda nao ha notas para esse aluno. Deseja incluir agora?\nDigite sim ou nao.\n");
                char opcao2[4];
                scanf("%s", opcao2);
                fflush(stdin);
                if (!strcmp(opcao2, "sim")){
                    printf("\nDigite a nota do %s\n", turma[j].nome);
                    scanf("%lf", &turma[j].nota_prova[i-1]);
                    limpar_tela();
                    printf("Nota Inserida!\n\n");
                }
            }
            else{
                printf("A nota do aluno desejado na prova %d eh: %.1lf\n\n", i, turma[j].nota_prova[i-1]);
            }
            break;
        case 'R':
        case 'r':
            turma[j].nota_prova[i-1] = -1;
            printf("Nota removida!\n\n");
            break;
        case 'I':
        case 'i':
            printf("Digite a nota do aluno selecionado: \n");
            scanf("%lf", &turma[j].nota_prova[i-1]);
            limpar_tela();
            printf("Nota inserida!\n\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
    int p = 0;
    int k;
    FILE *arq = fopen(nomes, "wb");
    for (k = 0; k < cont; k++) {
        p += fwrite(&turma[k], sizeof(struct informacoes), 1, arq);
    }
    if (p != cont)
        printf("Excluiu errado.\n");
    fclose (arq);
    free(turma);
    fflush(stdin);
    system("PAUSE");
    limpar_tela();
}

void grupos_trab (int cont, struct informacoes turma[], int i, int j) {
    int k;
    int aux = 0;
    for (k=0; k<cont; k++) {
        if (!strcmp(turma[k].nome_grupo[i-1], turma[j].nome_grupo[i-1]))
            aux++;
    }
    struct informacoes *ordena = (struct informacoes*)(malloc(aux*sizeof(struct informacoes)));
    if (!ordena)
        printf("Erro ao alocar memoria");
    aux = 0;
    for (k=0; k<cont; k++) {
        if (!strcmp(turma[k].nome_grupo[i-1], turma[j].nome_grupo[i-1])) {
            ordena[aux] = turma[k];
            aux++;
        }
    }
    int x, y, pos;
    struct informacoes troca;
    for (x=0; x<aux-1; x++) {
        pos = x;
        for (y=x+1; y<aux; y++) {
            if (ordena[pos].posicao[i-1] > ordena[y].posicao[i-1])
                pos = y;
        }
        if (pos != x) {
            troca = ordena[x];
            ordena[x] = ordena[pos];
            ordena[pos] = troca;
        }
    }
    for (x=0; x<aux; x++) {
        printf("Aluno: %s - %d\n"
               "Posicao no grupo: %d\n"
               "Nota do aluno: %.1lf\n", ordena[x].nome, ordena[x].numero_usp, ordena[x].posicao[i-1], ordena[x].nota_trab[i-1]);
    }
}

//Caso queira mexer com as notas de trabalhos dos alunos da disciplina
void opcao_trabalhos(){
    int cont = numero_de_alunos();
    struct informacoes *turma = (struct informacoes*)(malloc(cont*sizeof(struct informacoes)));
    if (!turma){
        printf("Nao foi possivel alocar memoria. Erro p1296593.\nConsulte o menino da informatica.");
        exit (-1);
    }
    carrega_turma(turma, cont);
    int i;
    printf("Digite qual trabalho voce deseja: 1 a %d.\n", NUMT);
    scanf("%d", &i);
    fflush(stdin);
    while (i > NUMT) {
        printf("O formato atual soh possui %d trabalhos. Tente novamente: \n", NUMT);
        scanf("%d", &i);
        fflush(stdin);
    }
    char opcao;
    limpar_tela();
    printf("Escolha o que voce executara a respeito do trabalho %d:\n"
           "-Digite 'C' para consultar notas;\n"
           "-Digite 'R' para remover notas;\n"
           "-Digite 'I' pra incluir notas.\n"
           "-Digite 'V' para voltar ao menu principal.\n", i);
    scanf("%c", &opcao);
    fflush(stdin);
    limpar_tela();
    if (opcao == 'V' || opcao == 'v'){
        return;
    }
    printa_registrados(cont, turma);
    printf("\nDigite o nome do aluno desejado:\n");
    char aux[TAMNOMES];
    scanf("%[A-Z a-z 0-9/,.-]", aux);
    fflush(stdin);
    limpar_tela();
    int j = encontra_aluno(aux, cont, turma);
    if (j < 0)
        return;
    switch (opcao) {
        case 'C':
        case 'c':
            if (turma[j].contola_nome[i-1]) {
                printf("A nota do aluno desejado no trabalho %d eh: %.1lf.\n"
                       "A posicao do aluno nesse trabalho foi: %d\n"
                       "O nome do grupo do aluno nesse trabalho eh: %s\n", i, turma[j].nota_trab[i-1], turma[j].posicao[i-1], turma[j].nome_grupo[i-1]);
                printf("Deseja consultar os integrantes do grupo do aluno no trabalho requisitado?\nDigite sim ou nao.\n");
                char opcao2[4];
                scanf("%s", opcao2);
                if (!strcmp(opcao2, "sim")) {
                    limpar_tela();
                    int k;
                    printf("Configuracao do grupo %s:\n", turma[j].nome_grupo[i-1]);
                    grupos_trab(cont, turma, i, j);
                    printf("\n");
                    system("PAUSE");
                    limpar_tela();
                }
            }
            else {
                printf("A nota desejada ainda nao foi incluida neste sistema.\n\n");
                system("PAUSE");
            }
            break;
        case 'R':
        case 'r':
            turma[j].nota_trab[i-1] = -1;
            turma[j].posicao[i-1] = -1;
            turma[j].contola_nome[i-1] = 0;
            strcpy(turma[j].nome_grupo[i-1], "vazio");
            printf("Nota removida!\n");
            break;
        case 'I':
        case 'i':
            printf("Digite a posicao do aluno no trabalho %d:\n", i);
            scanf("%d", &turma[j].posicao[i-1]);
            fflush(stdin);
            printf("Agora insira a nota de valor integral dada ao grupo:\n");
            scanf("%lf", &turma[j].nota_trab[i-1]);
            fflush(stdin);
            turma[j].nota_trab[i-1] = turma[j].nota_trab[i-1]*pow(0.9, turma[j].posicao[i-1]-1);
            printf("A nota do aluno no trabalho %d - considerando a posicao - eh: %.1lf\n", i, turma[j].nota_trab[i-1]);
            printf("Por fim, insira o nome do grupo do aluno neste trabaho:\n");
            scanf("%100[A-Z a-z 0-9 @-)]", &turma[j].nome_grupo[i-1]);
            limpar_tela();
            turma[j].contola_nome[i-1] = 1;
            printf("Nota incluida com sucesso!\n\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
    int p = 0;
    int k;
    FILE *arq = fopen(nomes, "wb");
    for (k=0; k<cont; k++) {
        p += fwrite(&turma[k], sizeof(struct informacoes), 1, arq);
    }
    if (p != cont)
        printf("Excluiu errado.\n");
    fclose (arq);
    free(turma);
    fflush(stdin);
}

int main(){
    //Apresentacao
    arte("./Artes/apresentacao.txt");
    //Sleep(2000);

    //Variaveis booleanas
    int mantenedor = 1;

    //Opcoes
    while (mantenedor){
        arte("./Artes/opcoes.txt");
        char opcao;
        scanf("%c", &opcao);
        fflush(stdin);
        limpar_tela();
        switch(opcao){
            case 'A':
            case 'a':
                opcao_alunos();
                break;
            case 'P':
            case 'p':
                opcao_provas();
                break;
            case 'T':
            case 't':
                opcao_trabalhos();
                break;
            case 'I':
            case 'i':
                limpar_tela();
                arte("./Artes/informacoes.txt");
                system("PAUSE");
                limpar_tela();
                break;
            case 'Q':
            case 'q':
                arte("./Artes/sair.txt");
                Sleep(4000);
                exit(0);
            default:
                printf("Comando incorreto!\n\n");
                break;
        }
    }
    return 0;
}

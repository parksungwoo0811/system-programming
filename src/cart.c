#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_LINE_LENGTH 256

// 메시지 큐 구조체 정의
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

// 상품 정보를 저장하는 구조체
typedef struct {
    int id;
    char name[100];
    int price;
    char size[5];
    char color[20];
} Product;

// 파일 정보 출력 함수
void print_cart_file_info() {
    const char *filename = "data/cart.txt";
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("장바구니 파일 정보를 가져올 수 없습니다.");
        return;
    }

    printf("장바구니 파일 정보:\n");
    printf("파일 크기: %ld bytes\n", file_stat.st_size);
    printf("최근 접근 시간: %s", ctime(&file_stat.st_atime));
    printf("최근 수정 시간: %s", ctime(&file_stat.st_mtime));
    printf("\n");
}

// products.txt에서 상품 정보를 읽어오는 함수
Product* read_product_info(int product_id) {
    FILE *file = fopen("data/products.txt", "r"); // 상품 정보가 담긴 파일 열기
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    Product *product = malloc(sizeof(Product)); // 동적 메모리 할당

    // 파일에서 각 라인을 읽어오고 상품 정보 파싱
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[100];
        int price;
        char size[5];
        char color[20];

        // 각 라인에서 상품 정보를 읽어와 구조체에 저장
        if (sscanf(line, "%d, %99[^,], %d, %4[^,], %19[^\n]", &id, name, &price, size, color) == 5) {
            if (id == product_id) {
                product->id = id;
                strcpy(product->name, name);
                product->price = price;
                strcpy(product->size, size);
                strcpy(product->color, color);
                fclose(file);
                return product;
            }
        }
    }

    fclose(file);
    free(product);
    return NULL;
}

// 장바구니에 상품 추가하는 함수
void add_to_cart(int product_id) {
    // 상품 정보를 읽어옴
    Product *product = read_product_info(product_id);
    if (product == NULL) {
        printf("해당 상품이 존재하지 않습니다.\n");
        return;
    }

    // 장바구니에 추가
    FILE *file = fopen("data/cart.txt", "a");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        free(product);
        return;
    }

    // 상품 정보를 파일에 쓰기
    fprintf(file, "상품 ID: %d, 이름: %s, 가격: %d, 크기: %s, 색상: %s\n", product->id, product->name, product->price, product->size, product->color);
    fclose(file);
    free(product);

    printf("상품이 장바구니에 추가되었습니다.\n");
    printf("\n");
}

// 장바구니 보기 함수
void view_cart() {
    FILE *file = fopen("data/cart.txt", "r"); // 장바구니 파일 열기
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return;
    }

    char line[256];
    printf("===============\n");
    printf("장바구니 내용:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // 각 라인 출력
    }
    printf("===============\n");

    fclose(file);
}

// 장바구니 관리 함수
void manage_cart() {
    int choice, product_id;
    printf("1. 장바구니에 상품 추가\n");
    printf("2. 장바구니 보기\n");
    printf("\n");
    printf("선택: ");
    scanf("%d", &choice); // 사용자 입력 받기

    switch (choice) {
        case 1:
            printf("\n");
            printf("추가할 상품 ID: ");
            scanf("%d", &product_id); // 상품 ID 입력 받기
            add_to_cart(product_id); // 장바구니에 상품 추가
            break;
        case 2:
            view_cart(); // 장바구니 보기
            break;
        default:
            printf("잘못된 선택입니다.\n"); // 잘못된 선택일 경우 메시지 출력
    }
}

void exec_ls() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork 실패");
        return;
    } else if (pid == 0) {
        execlp("ls", "ls", "-l", "data", NULL);
        perror("exec 실패");
        exit(1);
    } else {
        wait(NULL);
        printf("데이터 디렉토리 목록이 나열되었습니다.\n");
    }
}

void use_pipe() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("파이프 생성 실패");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork 실패");
        return;
    } else if (pid == 0) {
        close(pipefd[1]);
        char buffer[128];
        read(pipefd[0], buffer, sizeof(buffer));
        printf("메인 프로세스로부터 받은 메시지: %s\n", buffer);
        close(pipefd[0]);
        exit(0);
    } else {
        close(pipefd[0]);
        const char *message = "새로운 상품이 입고되었습니다!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        wait(NULL);
    }
}

void use_message_queue() {
    key_t key;
    int msgid;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    message.msg_type = 1;
    strcpy(message.msg_text, "장바구니에 새로운 상품이 추가되었습니다!");

    msgsnd(msgid, &message, sizeof(message), 0);

    printf("메시지가 전송되었습니다: %s\n", message.msg_text);
}

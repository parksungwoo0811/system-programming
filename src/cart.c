#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// 상품 정보를 저장하는 구조체
typedef struct {
    int id;           // 상품 ID
    char name[100];   // 상품 이름
    int price;        // 상품 가격
    char size[5];     // 상품 크기
    char color[20];   // 상품 색상
} Product;

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

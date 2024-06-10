#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "cart.h"
#include "payment.h"
#include "user.h"

// 함수 프로토타입 선언
void print_cart_file_info();
void exec_ls();
void use_pipe();
void use_message_queue();

int main() {
    // 시스템 초기화
    initialize();

    // 메인 메뉴 루프
    while (1) {
        int choice;
        printf("1. 상품 조회\n");
        printf("2. 장바구니 보기\n");
        printf("3. 결제하기\n");
        printf("4. 장바구니 파일 정보 출력\n");
        printf("5. 데이터 디렉토리 목록 나열\n");
        printf("6. 파이프 사용\n");
        printf("7. 메시지 큐 사용\n");
        printf("8. 종료\n");
        printf("\n");
        printf("선택: ");
        scanf("%d", &choice); // 사용자 입력 받기
        printf("\n");

        switch (choice) {
            case 1:
                display_products(); // 상품 조회 기능 호출
                break;
            case 2:
                manage_cart(); // 장바구니 관리 기능 호출
                break;
            case 3:
                process_payment(); // 결제 기능 호출
                break;
            case 4:
                print_cart_file_info(); // 장바구니 파일 정보 출력
                break;
            case 5:
                exec_ls(); // 데이터 디렉토리 목록 나열
                break;
            case 6:
                use_pipe(); // 파이프 사용
                break;
            case 7:
                use_message_queue(); // 메시지 큐 사용
                break;
            case 8:
                printf("쇼핑을 종료합니다.\n"); // 종료 메시지 출력
                exit(0); // 프로그램 종료
            default:
                printf("잘못된 선택입니다.\n"); // 잘못된 선택

        }
    }

    return 0;
}

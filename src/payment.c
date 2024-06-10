#include "payment.h" // 결제 관련 헤더 파일 포함
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_payment() {
    FILE *file = fopen("data/cart.txt", "r"); // 장바구니 파일 열기
    if (file == NULL) {
        perror("파일을 열 수 없습니다."); // 파일 열기 실패 시 오류 메시지 출력
        return;
    }

    char line[256]; // 한 줄씩 읽어올 버퍼
    int total_price = 0; // 총 결제 금액을 저장할 변수 초기화
    printf("===============\n");
    printf("장바구니 내용:\n");
    while (fgets(line, sizeof(line), file)) { // 파일에서 한 줄씩 읽어옴
        printf("%s", line); // 각 라인을 출력하여 확인

        int price; // 상품 가격을 저장할 변수
        char *price_str = strstr(line, "가격:"); // 라인에서 "가격:" 문자열을 찾음
        if (price_str != NULL) { // 찾은 경우
            sscanf(price_str, "가격: %d,", &price); // 가격 정보를 읽어옴
            total_price += price; // 총 결제 금액에 가격을 더함
        }
    }

    fclose(file); // 파일 닫기
    printf("===============\n");
    printf("총 결제 금액: %d원\n", total_price); // 총 결제 금액 출력

    // 결제 여부 확인
    char choice;
    printf("결제를 진행하시겠습니까? (y/n): ");
    scanf(" %c", &choice); // 결제 여부 입력 받음

    if (choice == 'y' || choice == 'Y') { // 결제 진행하는 경우
        // 결제 완료 후 장바구니 초기화
        file = fopen("data/cart.txt", "w"); // 파일을 쓰기 모드로 열기
        if (file == NULL) {
            perror("파일을 열 수 없습니다."); // 파일 열기 실패 시 오류 메시지 출력
            return;
        }

        fclose(file); // 파일 닫기
        printf("결제가 완료되었습니다. 장바구니가 초기화되었습니다.\n"); // 결제 완료 메시지 출력
    } else if (choice == 'n' || choice == 'N') { // 결제 취소하는 경우
        printf("결제가 취소되었습니다.\n"); // 결제 취소 메시지 출력
    } else { // 잘못된 입력인 경우
        printf("잘못된 입력입니다. 결제가 취소되었습니다.\n"); // 잘못된 입력 메시지 출력
    }
    printf("\n");
}

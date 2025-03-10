/*
 * itoa.c
 *
 *  Created on: 2 Jun 2024
 *      Author: ahmed
 */


#include <stdbool.h>
#include <string.h>

// Helper function to reverse a string in place
static void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/*turn an int to a string*/
void itoa_FCN(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        str[0] = '\0'; // Invalid base
        return;
    }

    int i = 0;
    bool isNegative = false;

    // Handle zero case
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        isNegative = true;
        value = -value;
    }

    // Process the integer part
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }

    // Add negative sign for base 10
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    reverse(str, i);
}

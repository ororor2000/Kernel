#include <stdio.h>
#include <string.h>

typedef struct LED {
    int pin;
    int mode;
} LED;

int main()
{
    char *string,*found;

    string = strdup("21,2");
    printf("Original string: '%s'\n",string);


    LED led;
    memset(&led, 0, sizeof(led));

    found = strsep(&string, ",");
    kstrtol(found, 0, led.pin);

    found = strsep(&string, ",");
    kstrtol(found, 0, led.pin);


    printf("pin=%d, mode=%d", led.pin, led.mode);

    return 0;
}
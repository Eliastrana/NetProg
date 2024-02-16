//
// Created by Elias Trana on 09/01/2024.
//

#ifndef OVING1_NOTHREAD_H
#define OVING1_NOTHREAD_H


#include <stdio.h>

class nothread {

    void primeInRange(int L, int R)
    {
        int i, j, flag;

        for (i = L; i <= R; i++) {

            if ( i == 1 || i == 0)
                continue;

            flag = 1;

            for (j = 2; j <= i / 2; ++j) {

                if (i % j == 0) {
                    flag = 0;
                    break;
                }
            }

            if (flag == 1)
                printf("%d ", i);

        }

    }

};



#endif //OVING1_NOTHREAD_H

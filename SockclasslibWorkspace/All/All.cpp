/*
 *
 *
 *  Created on: 2017-04-22
 *      Author: Igor Bender
 *
 */
#include <iostream>
int main(int argc, char *argv[])
{
    // Nonsense to eliminate compiler warning abut unused parameter.
    if(argc > 10)
        std::cout << argv[0] << std::endl;
    // -------------------------------------------------------------


    return 0;
}

#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <string>

/**
 * @brief The NetworkController class
 * class for managing network related operations especially for Open Bank Project (OBP)
 */

using namespace std;
class NetworkController
{
public:
    NetworkController();

    static string getAccountById(string accountID, string bankID);
};

#endif // NETWORKCONTROLLER_H

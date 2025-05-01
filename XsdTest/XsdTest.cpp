#include "XsdExample.h"
#include <iostream>

using namespace std;
using namespace XsdExample;

int main()
{
    PurchaseOrderType::BillTo_type BillTo("Aaaa Bbbb", "Netiv", "Haifa", "NA", 3333333);
    PurchaseOrderType Purchase(BillTo);

    Purchase.BillTo().name("aaa");
    Purchase.BillTo().country("Israel");
    Purchase.ShipTo().push_back(USAddress("Aaaa Bbbb", "Netiv", "Haifa", "NA", 3333333));

    return 0;
}

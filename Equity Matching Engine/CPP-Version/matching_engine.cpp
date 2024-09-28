#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long int ll;

class Order
{
    private:
    ll OrderID;
    ll Timestamp;
    string Symbol;
    char Side;
    char OrderType;
    float Price;
    ll Quantity;
    public:
    Order(){};
    Order(ll id,ll time,string symbol,char type,char side,float price,ll quantity)
    {
        OrderID=id;
        Timestamp=time;
        Symbol=symbol;
        Side=side;
        OrderType=type;
        Price=price;
        Quantity=quantity;
    };
    ~Order(){};

    ll getID(){return OrderID;}
    ll getTimestamp(){return Timestamp;}
    ll getQuantity(){return Quantity;}
    float getPrice(){return Price;}
    char getSide(){return Side;}
    char getType(){return OrderType;}
    string getSymbol(){return Symbol;}
    
    void setPrice(float x){ Price=x;}
    void setQuantity(ll x){ Quantity=x;}
};

vector<Order> OrderBook;
map<string,vector<pair<Order,Order>>> Matchbook;

/////helpers/////

bool isChar(string s)
{
     if(s=="B"||s=="S"||s=="M"||s=="L"||s=="I")
         return true;
     return false;        
}

bool isInt(string s)
{
    if(s.empty()||(!isdigit(s[0]))) 
        return false ;
    char * p ;
    strtol(s.c_str(), &p, 10) ;
    return (*p == 0) ;
}

bool isFloat(string s)
{
    std::istringstream iss(s);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}

bool comp(Order order1,Order order2)
{
    if(order1.getSymbol()<=order2.getSymbol())
        return true;
    else
        return false;
}

string to_stringp(float a_value)
{
    std::ostringstream out;
    out << std::fixed;
    out << std::setprecision(2);
    out << a_value;
    string s=out.str();
    return s;
}
vector<string> splitOrderStr(string s)
{
    string delimiter=",";
    vector<string> components;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        components.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    components.push_back(s);
    return components;
}
//////////


////New///////
vector<string> processNew(string queryNew)
{
    vector<string> response;
    string ans;
    string ID;
    
    char op,type,side;
    string sym;
    float price;
    ll id,timestamp,quantity;
    
    vector<string> elements=splitOrderStr(queryNew);
    
    ID=elements[1];

    //check validity
    if(elements.size()>8)
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isInt(elements[1]))
        id=(int)stoi(elements[1]);
    else
    {        
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isInt(elements[2]))    
        timestamp=(ll)stoi(elements[2]);
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(elements[3].size()!=0)
        sym=elements[3];
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isChar(elements[4]))
        type=elements[4][0];
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isChar(elements[5]))
        side=elements[5][0];
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isFloat(elements[6]))
        price=stof(elements[6]);
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    if(isInt(elements[7]))
        quantity=(ll)stoi(elements[7]);
    else
    {
        ans=ID+" - Reject - 303 - Invalid order details";
        response.push_back(ans);
        return response;
    }
    
    
    for(int i=0;i<OrderBook.size();i++)
    {
        if(id==OrderBook[i].getID())
        {
            ans=ID+" - Reject - 303 - Invalid order details";
            response.push_back(ans);
            return response;
        }
    }
    Order newOrder(id,timestamp,sym,type,side,price,quantity);
    OrderBook.push_back(newOrder);
    //if(elements[5]=="B")
    //    OrderBuy.push_back(newOrder);
    //else
    //    OrderSell.push_back(newOrder);
    ans=ID+" - Accept";
    response.push_back(ans);
    return response;
}


//////Amend//////    
vector<string> processAmend(string queryAmend)
{
    vector<string> response;
    string ans;
    string ID;

    char type,side;
    string sym;
    float price;
    ll id,timestamp,quantity;
    
    vector<string> elements=splitOrderStr(queryAmend);
    
    ID=elements[1];

    //check validity
    if(elements.size()!=8)
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    if(isInt(elements[1]))
        id=(int)stoi(elements[1]);
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    if(isInt(elements[2]))    
        timestamp=(ll)stoi(elements[2]);
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    if(elements[3]!="")
        sym=elements[3];
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    if(isChar(elements[4]))
        type=elements[4][0];
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }
    if(isChar(elements[5]))
        side=elements[5][0];
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    if(isFloat(elements[6]))
        price=stof(elements[6]);
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }
    if(isInt(elements[7]))
        quantity=(ll)stoi(elements[7]);
    else
    {
        ans=ID+" - AmendReject - 101 - Invalid amendment details";
        response.push_back(ans);
        return response;
    }

    for(int i=0;i<OrderBook.size();i++)
    {
        if(id==OrderBook[i].getID())
        {
            if(sym!=OrderBook[i].getSymbol())
            {
                ans=ID+" - AmendReject - 101 - Invalid amendment details";
                response.push_back(ans);
                return response;
            }
            if(type!=OrderBook[i].getType())
            {
                ans=ID+" - AmendReject - 101 - Invalid amendment details";
                response.push_back(ans);
                return response;
            }
            if(side!=OrderBook[i].getSide())
            {
                ans=ID+" - AmendReject - 101 - Invalid amendment details";
                response.push_back(ans);
                return response;
            }
            if((price==OrderBook[i].getPrice())&&(quantity==OrderBook[i].getQuantity()))
            {
                ans=ID+" - AmendReject - 101 - Invalid amendment details";
                response.push_back(ans);
                return response;
            }

            if(price!=OrderBook[i].getPrice())
                OrderBook[i].setPrice(price);
            if(quantity!=OrderBook[i].getQuantity())
                OrderBook[i].setQuantity(quantity);
            ans = ID+" - AmendAccept";
            response.push_back(ans);
            return response;
        }
    }    

    ans=ID + " - AmendReject - 404 - Order does not exist";
    response.push_back(ans);
    return response;
}


//////Match//////
vector<string> processMatch(string queryMatch)
{
    vector<string> response;
    vector<string> elements=splitOrderStr(queryMatch);
    if(elements.size()==3)
    {
        string sym=elements[2];
        for(int i=0;i<OrderBook.size();i++)
        {
            Order currentOrder=OrderBook[i];
            char side1=currentOrder.getSide();
            char type1=currentOrder.getType();
            string sym1=currentOrder.getSymbol();
            if(sym==sym1)
            {
                if(type1=='M')
                {
                    if(side1=='B')
                    {
                        float minPrice=(float)INT_MAX;
                        int ind=-1;
                        for(int j=0;j<OrderBook.size();j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                    if(OrderBook[j].getPrice()<minPrice)
                                    {
                                        minPrice=OrderBook[j].getPrice();
                                        ind=j;
                                    }
                            }
                        }
                        if(ind!=-1)
                        {
                            Order order1=OrderBook[i];
                            Order order2=OrderBook[ind];
                            order1.setPrice(order2.getPrice());

                            if(order1.getQuantity()>order2.getQuantity())
                            {
                                ll min_qty=order2.getQuantity();
                                OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                order1.setQuantity(min_qty);
                                OrderBook.erase(OrderBook.begin()+ind);
                                if(ind<i)
                                {
                                    i--;
                                }

                            }
                            else if(order1.getQuantity()<order2.getQuantity())
                            {
                                ll min_qty=order1.getQuantity();
                                OrderBook[ind].setQuantity(OrderBook[ind].getQuantity()-min_qty);
                                order2.setQuantity(min_qty);
                                OrderBook.erase(OrderBook.begin()+i);
                                i--;
                            }
                            else
                            {
                                OrderBook.erase(OrderBook.begin()+i);
                                i--;
                                OrderBook.erase(OrderBook.begin()+ind);
                                if(ind<i)
                                    i--;
                            }

                            pair<Order,Order> matched=make_pair(order1,order2);
                                Matchbook[sym].push_back(matched);
                        }

                    }
                    else
                    {
                        float maxPrice=0;
                        int ind=-1;
                        for(int j=0;j<OrderBook.size();j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[i].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                    if(OrderBook[j].getPrice()>maxPrice)
                                    {
                                        maxPrice=OrderBook[j].getPrice();
                                        ind=j;
                                    }
                            }
                        }
                        if(ind!=-1)
                        {
                            Order order1=OrderBook[i];
                            Order order2=OrderBook[ind];
                            order1.setPrice(order2.getPrice());

                            if(order1.getQuantity()>order2.getQuantity())
                            {
                                ll min_qty=order2.getQuantity();
                                OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                order1.setQuantity(min_qty);
                                OrderBook.erase(OrderBook.begin()+ind);
                                if(ind<i)
                                {
                                    i--;
                                }

                            }
                            else if(order1.getQuantity()<order2.getQuantity())
                            {
                                ll min_qty=order1.getQuantity();
                                OrderBook[ind].setQuantity(OrderBook[ind].getQuantity()-min_qty);
                                order2.setQuantity(min_qty);
                                OrderBook.erase(OrderBook.begin()+i);
                                i--;
                            }
                            else
                            {
                                OrderBook.erase(OrderBook.begin()+i);
                                i--;
                                OrderBook.erase(OrderBook.begin()+ind);
                                if(ind<i)
                                {
                                    i--;
                                }
                            }

                            pair<Order,Order> matched=make_pair(order1,order2);
                                Matchbook[sym].push_back(matched);
                        }
                    }
                }
                else if(type1=='L')
                {
                    if(side1=='B')
                    {
                        float bestPrice=0.0;
                        int ind=-1;
                        for(int j=0;j<OrderBook.size();j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                if(OrderBook[j].getPrice()<=OrderBook[i].getPrice())
                                {
                                    bestPrice=OrderBook[j].getPrice();
                                    Order order1=OrderBook[i];
                                    Order order2=OrderBook[j];
                                    order1.setPrice(order2.getPrice());

                                    if(order1.getQuantity()>order2.getQuantity())
                                    {
                                        ll min_qty=order2.getQuantity();
                                        OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                        order1.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            j--;

                                    }
                                    else if(order1.getQuantity()<order2.getQuantity())
                                    {
                                        ll min_qty=order1.getQuantity();
                                        OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                        order2.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            i--;
                                    }
                                    else
                                    {
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            i--;
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            j--;
                                    }

                                    pair<Order,Order> matched=make_pair(order1,order2);
                                        Matchbook[sym].push_back(matched);
                                }
                            }
                        }

                    }
                    else
                    {
                        float bestPrice=0.0;
                        int ind=-1;
                        for(int j=0;j<OrderBook.size();j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[i].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                if(OrderBook[j].getPrice()>=OrderBook[i].getPrice())
                                {
                                    bestPrice=OrderBook[j].getPrice();
                                    Order order1=OrderBook[i];
                                    Order order2=OrderBook[j];
                                    order1.setPrice(order2.getPrice());

                                    if(order1.getQuantity()>order2.getQuantity())
                                    {
                                        ll min_qty=order2.getQuantity();
                                        OrderBook[i].setQuantity(min_qty);
                                        order1.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            j--;

                                    }
                                    else if(order1.getQuantity()<order2.getQuantity())
                                    {
                                        ll min_qty=order1.getQuantity();
                                        OrderBook[j].setQuantity(min_qty);
                                        order2.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            i--;
                                    }
                                    else
                                    {
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            i--;
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            j--;
                                    }

                                    pair<Order,Order> matched=make_pair(order1,order2);
                                        Matchbook[sym].push_back(matched);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(side1=='B')
                    {
                        float bestPrice=0.0;
                        int ind=-1;
                        for(int j=0;j<i;j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                if(OrderBook[j].getPrice()<=OrderBook[i].getPrice())
                                {
                                    bestPrice=OrderBook[j].getPrice();
                                    Order order1=OrderBook[i];
                                    Order order2=OrderBook[j];
                                    order1.setPrice(order2.getPrice());

                                    if(order1.getQuantity()>order2.getQuantity())
                                    {
                                        ll min_qty=order2.getQuantity();
                                        OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                        order1.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            j--;

                                    }
                                    else if(order1.getQuantity()<order2.getQuantity())
                                    {
                                        ll min_qty=order1.getQuantity();
                                        OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                        order2.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            i--;
                                    }
                                    else
                                    {
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            i--;
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;
                                            j--;
                                        }
                                        else
                                            j--;
                                    }

                                    pair<Order,Order> matched=make_pair(order1,order2);
                                        Matchbook[sym].push_back(matched);
                                }
                            }
                        }

                    }
                    else
                    {
                        float bestPrice=0.0;
                        int ind=-1;
                        for(int j=0;j<i;j++)
                        {
                            char type2=OrderBook[j].getType();
                            char side2=OrderBook[j].getSide();

                            if(sym==OrderBook[i].getSymbol()&&side1!=side2&&type1==type2)
                            {
                                if(OrderBook[j].getPrice()>=OrderBook[i].getPrice())
                                {
                                    bestPrice=OrderBook[j].getPrice();
                                    Order order1=OrderBook[i];
                                    Order order2=OrderBook[j];
                                    order1.setPrice(order2.getPrice());

                                    if(order1.getQuantity()>order2.getQuantity())
                                    {
                                        ll min_qty=order2.getQuantity();
                                        OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                        order1.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            j--;

                                    }
                                    else if(order1.getQuantity()<order2.getQuantity())
                                    {
                                        ll min_qty=order1.getQuantity();
                                        OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                        order2.setQuantity(min_qty);
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            i--;
                                    }
                                    else
                                    {
                                        OrderBook.erase(OrderBook.begin()+i);
                                        if(i<j)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            i--;
                                        OrderBook.erase(OrderBook.begin()+j);
                                        if(j<i)
                                        {
                                            i--;j--;
                                        }
                                        else
                                            j--;
                                    }

                                    pair<Order,Order> matched=make_pair(order1,order2);
                                        Matchbook[sym].push_back(matched);
                                }
                            }
                        }
                    }
                }
            }

        }
    }
    else
    {
        for(int i=0;i<OrderBook.size();i++)
        {
            Order currentOrder=OrderBook[i];
            char side1=currentOrder.getSide();
            char type1=currentOrder.getType();
            string sym1=OrderBook[i].getSymbol();
            if(type1=='M')
            {
                if(side1=='B')
                {
                    float minPrice=(float)INT_MAX;
                    int ind=-1;
                    for(int j=0;j<OrderBook.size();j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                                if(OrderBook[j].getPrice()<minPrice)
                                {
                                    minPrice=OrderBook[j].getPrice();
                                    ind=j;
                                }
                        }
                    }
                    if(ind!=-1)
                    {
                        Order order1=OrderBook[i];
                        Order order2=OrderBook[ind];
                        order1.setPrice(order2.getPrice());

                        if(order1.getQuantity()>order2.getQuantity())
                        {
                            ll min_qty=order2.getQuantity();
                            OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                            order1.setQuantity(min_qty);
                            OrderBook.erase(OrderBook.begin()+ind);
                            if(ind<i)
                            {
                                i--;
                            }

                        }
                        else if(order1.getQuantity()<order2.getQuantity())
                        {
                            ll min_qty=order1.getQuantity();
                            OrderBook[ind].setQuantity(OrderBook[ind].getQuantity()-min_qty);
                            order2.setQuantity(min_qty);
                            OrderBook.erase(OrderBook.begin()+i);
                            i--;
                        }
                        else
                        {
                            OrderBook.erase(OrderBook.begin()+i);
                            i--;
                            OrderBook.erase(OrderBook.begin()+ind);
                            if(ind<i)
                                i--;
                        }

                        pair<Order,Order> matched=make_pair(order1,order2);
                            Matchbook[sym1].push_back(matched);
                    }

                }
                else
                {
                    float maxPrice=0;
                    int ind=-1;
                    for(int j=0;j<OrderBook.size();j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                                if(OrderBook[j].getPrice()>maxPrice)
                                {
                                    maxPrice=OrderBook[j].getPrice();
                                    ind=j;
                                }
                        }
                    }
                    if(ind!=-1)
                    {
                        Order order1=OrderBook[i];
                        Order order2=OrderBook[ind];
                        order1.setPrice(order2.getPrice());

                        if(order1.getQuantity()>order2.getQuantity())
                        {
                            ll min_qty=order2.getQuantity();
                            OrderBook[i].setQuantity(min_qty);
                            order1.setQuantity(min_qty);
                            OrderBook.erase(OrderBook.begin()+ind);
                            if(ind<i)
                            {
                                i--;
                            }

                        }
                        else if(order1.getQuantity()<order2.getQuantity())
                        {
                            ll min_qty=order1.getQuantity();
                            OrderBook[ind].setQuantity(OrderBook[ind].getQuantity()-min_qty);
                            order2.setQuantity(min_qty);
                            OrderBook.erase(OrderBook.begin()+i);
                            i--;
                        }
                        else
                        {
                            OrderBook.erase(OrderBook.begin()+i);
                            i--;
                            OrderBook.erase(OrderBook.begin()+ind);
                            if(ind<i)
                            {
                                i--;
                            }
                        }

                        pair<Order,Order> matched=make_pair(order1,order2);
                        Matchbook[sym1].push_back(matched);
                    }
                }
            }
            else if(type1=='L')
            {
                if(side1=='B')
                {
                    float bestPrice=0.0;
                    int ind=-1;
                    for(int j=0;j<OrderBook.size();j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                            if(OrderBook[j].getPrice()<=OrderBook[i].getPrice())
                            {
                                bestPrice=OrderBook[j].getPrice();
                                Order order1=OrderBook[i];
                                Order order2=OrderBook[j];
                                order1.setPrice(order2.getPrice());

                                if(order1.getQuantity()>order2.getQuantity())
                                {
                                    ll min_qty=order2.getQuantity();
                                    OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                    order1.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        j--;

                                }
                                else if(order1.getQuantity()<order2.getQuantity())
                                {
                                    ll min_qty=order1.getQuantity();
                                    OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                    order2.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        i--;
                                }
                                else
                                {
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        i--;
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        j--;
                                }

                                pair<Order,Order> matched=make_pair(order1,order2);
                                    Matchbook[sym1].push_back(matched);
                            }
                        }
                    }

                }
                else
                {
                    float bestPrice=0.0;
                    int ind=-1;
                    for(int j=0;j<OrderBook.size();j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                            if(OrderBook[j].getPrice()>=OrderBook[i].getPrice())
                            {
                                bestPrice=OrderBook[j].getPrice();
                                Order order1=OrderBook[i];
                                Order order2=OrderBook[j];
                                order1.setPrice(order2.getPrice());

                                if(order1.getQuantity()>order2.getQuantity())
                                {
                                    ll min_qty=order2.getQuantity();
                                    OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                    order1.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        j--;

                                }
                                else if(order1.getQuantity()<order2.getQuantity())
                                {
                                    ll min_qty=order1.getQuantity();
                                    OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                    order2.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        i--;
                                }
                                else
                                {
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        i--;
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        j--;
                                }

                                pair<Order,Order> matched=make_pair(order1,order2);
                                Matchbook[sym1].push_back(matched);
                            }
                        }
                    }
                }
            }
            else
            {
                if(side1=='B')
                {
                    float bestPrice=0.0;
                    int ind=-1;
                    for(int j=0;j<i;j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                            if(OrderBook[j].getPrice()<=OrderBook[i].getPrice())
                            {
                                bestPrice=OrderBook[j].getPrice();
                                Order order1=OrderBook[i];
                                Order order2=OrderBook[j];
                                order1.setPrice(order2.getPrice());

                                if(order1.getQuantity()>order2.getQuantity())
                                {
                                    ll min_qty=order2.getQuantity();
                                    OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                    order1.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        j--;

                                }
                                else if(order1.getQuantity()<order2.getQuantity())
                                {
                                    ll min_qty=order1.getQuantity();
                                    OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                    order2.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        i--;
                                }
                                else
                                {
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        i--;
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;
                                        j--;
                                    }
                                    else
                                        j--;
                                }

                                pair<Order,Order> matched=make_pair(order1,order2);
                                Matchbook[sym1].push_back(matched);
                            }
                        }
                    }

                }
                else
                {
                    float bestPrice=0.0;
                    int ind=-1;
                    for(int j=0;j<i;j++)
                    {
                        char type2=OrderBook[j].getType();
                        char side2=OrderBook[j].getSide();

                        if(sym1==OrderBook[j].getSymbol()&&side1!=side2&&type1==type2)
                        {
                            if(OrderBook[j].getPrice()>=OrderBook[i].getPrice())
                            {
                                bestPrice=OrderBook[j].getPrice();
                                Order order1=OrderBook[i];
                                Order order2=OrderBook[j];
                                order1.setPrice(order2.getPrice());

                                if(order1.getQuantity()>order2.getQuantity())
                                {
                                    ll min_qty=order2.getQuantity();
                                    OrderBook[i].setQuantity(OrderBook[i].getQuantity()-min_qty);
                                    order1.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        j--;

                                }
                                else if(order1.getQuantity()<order2.getQuantity())
                                {
                                    ll min_qty=order1.getQuantity();
                                    OrderBook[j].setQuantity(OrderBook[j].getQuantity()-min_qty);
                                    order2.setQuantity(min_qty);
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        i--;
                                }
                                else
                                {
                                    OrderBook.erase(OrderBook.begin()+i);
                                    if(i<j)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        i--;
                                    OrderBook.erase(OrderBook.begin()+j);
                                    if(j<i)
                                    {
                                        i--;j--;
                                    }
                                    else
                                        j--;
                                }

                                pair<Order,Order> matched=make_pair(order1,order2);
                                    Matchbook[sym1].push_back(matched);
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    map<string,vector<pair<Order,Order>>> ::iterator it;
    if(elements.size()==3)
    {
        string sym=elements[2];
        it=Matchbook.find(sym);
        if(it!=Matchbook.end())
        {
            vector<pair<Order,Order>> v=it->second;
            int count=v.size();
            for(int i=0;i<count;i++)
            {
                string ans=sym;
                Order order1=v[i].first;
                Order order2=v[i].second;
            ans+="|"+to_string(order1.getID())+","+order1.getType()+","+to_string(order1.getQuantity())+","+to_stringp(order1.getPrice());
                ans+="|"+to_stringp(order2.getPrice())+","+to_string(order2.getQuantity())+","+order2.getType()+","+to_string(order2.getID());
                response.push_back(ans);
            }
        }
        else
        {
            string ans="";
            response.push_back(ans);
        }
    }
    else
    {
        for(it=Matchbook.begin();it!=Matchbook.end();it++)
        {
            vector<pair<Order,Order>> v=it->second;
            int count=v.size();
            string ans="";
            if(count==0)
                response.push_back(ans);
            else
            {
                string symb=it->first;
                for(int i=0;i<count;i++)
                {
                    ans=symb;
                    Order order1=v[i].first;
                    Order order2=v[i].second;
                    ans+="|"+to_string(order1.getID())+","+order1.getType()+","+to_string(order1.getQuantity())+","+to_stringp(order1.getPrice());
                    ans+="|"+to_stringp(order2.getPrice())+","+to_string(order2.getQuantity())+","+order2.getType()+","+to_string(order2.getID());
                    response.push_back(ans);
                }
            }
        }
    }
    
    return response;
}

//////Cancel//////
vector<string> processCancel(string queryCancel)
{
    vector<string> response;
    string ans;
    string ID;
    
    vector<string> elements=splitOrderStr(queryCancel);
    
    ID=elements[1];
    int id=stoi(ID);
    ll timestamp=stoi(elements[2]);
    for(int i=0;i<OrderBook.size();i++)
    {
        if(id==OrderBook[i].getID())
        {
            OrderBook.erase(OrderBook.begin()+i);
            ans = ID+" - CancelAccept";
            response.push_back(ans);
            return response;
        }
    }
    ans = ID+ " - CancelReject - 404 - Order does not exist";
    response.push_back(ans);
    return response;
}



vector<string> processQueries(vector<string> queries) {
    // Write your code here.
    vector<string> res;
    vector<string> v;
    int num_queries=queries.size();
    for(int i=0;i<num_queries;i++)
    {
        if(queries[i][0]=='N')
            res=processNew(queries[i]);
        else if(queries[i][0]=='A')
            res=processAmend(queries[i]);
        else if(queries[i][0]=='X')
            res=processCancel(queries[i]);
        else if(queries[i][0]=='M')
            res=processMatch(queries[i]);
        int n=res.size();
        if(n>0)
        {
            for(int t=0;t<n;t++)
            {
                if(res[t]!="")
                    v.push_back(res[t]);
            }
        }
    }
    return v;
}


int main()
{
    ofstream fout("output.txt");

    int queries_size;
    cin >> queries_size;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> queries(queries_size);
    for (int queries_i = 0; queries_i < queries_size; queries_i++) {
        string queries_item;
        getline(cin, queries_item);

        queries[queries_i] = queries_item;
    }

    vector<string> response = processQueries(queries);

    for (int response_itr = 0; response_itr < response.size(); response_itr++) {
        fout << response[response_itr];

        if (response_itr != response.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}
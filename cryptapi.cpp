#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void coinprice(std::string coin, std::string fiat)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) 
    {
        std::string URLbuilder = "https://api.cryptapi.io/"+fiat+"/convert/?value=1&from="+coin;
        curl_easy_setopt(curl, CURLOPT_URL, URLbuilder.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj); 
    std::cout << obj["value_coin"].asString() << std::endl;
}

void blockchainfee(std::string coin)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) 
    {
        std::string URLbuilder = "https://api.cryptapi.io/"+coin+"/estimate/";
        curl_easy_setopt(curl, CURLOPT_URL, URLbuilder.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj); 
    std::cout << obj["estimated_cost"].asString() << std::endl; //Blockchain fees based on crypto
    std::cout << obj["estimated_cost_currency"]["USD"].asString() << std::endl; //Blockchain fees based on FIAT
}

void priceconvert(std::string from_coin, std::string amount, std::string to_equivalent)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) 
    {
        std::string URLbuilder = "https://api.cryptapi.io/"+to_equivalent+"/convert/?value="+amount+"&from="+from_coin;
        curl_easy_setopt(curl, CURLOPT_URL, URLbuilder.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj); 
    std::cout << obj["value_coin"].asString() << std::endl; 
    std::cout << obj["exchange_rate"].asString() << std::endl; 
}

void handlepayment(std::string coin, std::string paymentaddress, std::string callbackURL)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) 
    {
        std::string URLbuilder = "https://api.cryptapi.io/"+coin+"/create/?callback="+callbackURL+"&address="+paymentaddress;
        curl_easy_setopt(curl, CURLOPT_URL, URLbuilder.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj); 
    std::cout << obj["minimum_transaction_coin"].asString() << std::endl; //Calculate minumim transaction allowed for this coin
    std::cout << obj["address_in"].asString() << std::endl; //Address to provide
    std::cout << obj["address_out"].asString() << std::endl; //Address to forward payment to
    std::cout << obj["callback_url"].asString() << std::endl; //Backend url to handle server-side actions after payment
}

int main(void)
{
  //Example of functions call

  coinprice("btc","eur");
  blockchainfee("btc");
  priceconvert("ltc","100","btc");
  handlepayment("btc","3FZbgi29cpjq2GjdwV8eyHuJJnkLtktZc5","https://www.testpage.com/?paymentid=x");

  return 0;
}
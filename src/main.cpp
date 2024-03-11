#include <Geode/Geode.hpp>

using namespace geode::prelude;


std::string str_replace(std::string haystack, std::string needle, std::string replacement) {
	std::string input = std::string(haystack.c_str());
    std::string replace_word = needle; 
    std::string replace_by = replacement; 
  
    size_t pos = input.find(replace_word); 
  
    while (pos != std::string::npos) {
        input.replace(pos, replace_word.size(), replace_by); 

        pos = input.find(replace_word, 
                         pos + replace_by.size()); 
    }

	return input;
}


void proxySend(cocos2d::extension::CCHttpClient* self, cocos2d::extension::CCHttpRequest* req) {
	log::info("WOAH THE REQUEST URL IS {}", req->getUrl());

	auto new_request_url = std::string(req->getUrl());
	new_request_url = str_replace(new_request_url, "https://audio.ngfiles.com", "https://newgrounds.auby.pro");

	log::info("NEW REQUEST URL IS {}", new_request_url);

	req->setUrl(new_request_url.c_str());

	self->send(req);
}


$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
			geode::addresser::getNonVirtual(&cocos2d::extension::CCHttpClient::send)
        ),
        &proxySend,
        "cocos2d::extension::CCHttpClient::send",
        tulip::hook::TulipConvention::Thiscall
    );
}
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;


class $modify(ProxyLevelInfoLayer, LevelInfoLayer) {
	bool init(GJGameLevel* level, bool smth) {
		if (!LevelInfoLayer::init(level, smth)) return false;

		web::AsyncWebRequest()
			.fetch("https://check.auby.pro")
			.text()
			.then([](std::string const& result) {
				log::debug("Proxy OK");
				/*
				Notification::create(
					std::string("Newgrounds Proxy OK"), 
					CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png")
				)->show();*/
			})
			.expect([](std::string const& error) {
				log::error("Proxy ERROR: {}", error);
				Notification::create(
					std::string("Newgrounds Proxy Error: ") + error, 
					CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png")
				)->show();
			});

		return true;
	}
};


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


void proxySend(CCHttpClient* self, CCHttpRequest* req) {
	auto new_request_url = std::string(req->getUrl());
	new_request_url = str_replace(new_request_url, "audio.ngfiles.com", "newgrounds.auby.pro");
	new_request_url = str_replace(new_request_url, "http://", "https://");

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
#include "scraper.hpp"

#include "entry.hpp"
#include "parser.hpp"

#include "util/downloader.hpp"
#include <iostream>

namespace entreedief
{
	scraper::scraper()
	{}

	void scraper::scrape()
	{
		downloader dl("entreedief/1.0");
		
		parser::parse(dl.fetch("https://www.entree.nu/woningaanbod/actueel-aanbod/"), [](entry e){
			std::cout << e.id << ": " << e.price << " EUR https://www.entree.nu/woningaanbod/actueel-aanbod/details/?dwellingID=" << e.id << std::endl;
		});
	}
}

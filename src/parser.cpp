#include "parser.hpp"

#include <stdexcept>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace entreedief
{
	parser::parser(parser::callback_t f)
	: f(f)
	, rec()
	{}

	void parser::startElement(const std::string& /*namespaceURI*/, const std::string& /*localName*/, const std::string& qName, const parser::AttributesT& atts)
	{
		if(rec)
			rec.get().startElement();
	
		if(qName == "script" && atts.getValue("type") == "text/javascript")
			rec = html_recorder([&](std::string ch) {
				const static boost::regex r(".*ZigObject_Data\\('object-[0-9]+', (\\{[^}]*\\})\\);.*");
				boost::smatch m;
	
				if(!boost::regex_match(ch, m, r))
					return;
				
				const static boost::regex r_price(".*price: ([0-9.]+),.*");
				boost::smatch m_price;
				
				if(!boost::regex_match(m[1].str(), m_price, r_price))
					throw std::runtime_error("Could not parse price");
					
				const static boost::regex r_id(".*id: ([0-9]+),.*");
				boost::smatch m_id;
				
				if(!boost::regex_match(m[1].str(), m_id, r_id))
					throw std::runtime_error("Could not parse id");
					
				const static boost::regex r_municipality(".*municipalityName: unescape\\('Nijmegen'\\),.*");
				boost::smatch m_municipality;
				
				if(!boost::regex_match(m[1].str(), m_municipality, r_municipality))
					return; // Outside of Nijmegen
					
				const static boost::regex r_model(".*modelGroupID: '3',.*");
				boost::smatch m_model;
				
				if(!boost::regex_match(m[1].str(), m_model, r_model))
					return; // Not correct model
				
				f({boost::lexical_cast<size_t>(m_id[1]), boost::lexical_cast<float>(m_price[1])});
			});
	}
	
	void parser::characters(const std::string& ch)
	{
		if(rec)
			rec.get().characters(ch);
	}
	
	void parser::endElement(const std::string& /* namespaceURI */, const std::string& /* localName */, const std::string& /* qName */)
	{
		if(rec && rec.get().endElement())
			rec = boost::none;
	}
}

#pragma once

#include <boost/optional.hpp>

#include "entry.hpp"

#include "util/html_parser.hpp"
#include "util/html_recorder.hpp"

namespace entreedief
{
	class parser : public html_parser::default_handler
	{
		public:
			typedef std::function<void(entry)> callback_t;
	
		private:
			callback_t f;
			boost::optional<html_recorder> rec;
	
			parser(callback_t f);
		public:
			parser(parser&) = delete;
			void operator=(parser&) = delete;
			
			template<typename T>
			static void parse(T source, callback_t f)
			{
				parser p(f);
				html_parser::parse(source, p);
			}
			
			virtual void startElement(const std::string& namespaceURI, const std::string& localName, const std::string& qName, const AttributesT& atts);
			virtual void characters(const std::string& ch);
			virtual void endElement(const std::string& namespaceURI, const std::string& localName, const std::string& qName);
	};
}

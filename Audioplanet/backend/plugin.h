//  backend/plugin.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_PLUGIN_H
#define BACKEND_PLUGIN_H


#include <string>

#include "system/debug.h"
#include "system/exception.h"


namespace ap
{

	class Plugin
	{
	public:
		typedef void * Handle;


		class Info
		{
	    public:
			friend class Plugin;


			inline Info(const std::string & name,            // Dialog name
			            const std::string & description,     // About box fields
			            const std::string & author,          //
			            const std::string & copyright,       //
			            const std::string & date);           //

			virtual ~Info() {}
		
			// Called by Audioplanet so that the plug-in can inform Audioplanet of what it does
			virtual void query() const = 0;


		private:
			std::string name_;
			std::string description_;
			std::string author_;
			std::string copyright_;
			std::string date_;
		};


		void open(const std::string & fileName);
		void close();

		const std::string & name() { return info_->name; }


	private:
		Handle handle_;
		Info * info_;
	};


	//
	// Plugin::Info class inline methods
	//


	inline Plugin::Info::Info(const std::string & name,             // Dialog name
	                          const std::string & description,      // About box fields
	                          const std::string & author,           //
	                          const std::string & copyright,        //
	                          const std::string & date) :           //

		name_(name),
		description_(description),
		author_(author),
		copyright_(copyright),
		date_(date) {}


	//
	// Plugin class inline methods
	//


	inline const std::string & Plugin::name()
	{
		trace();
		return info_->name_;
	}
		

}  // namespace AP


#endif  // BACKEND_PLUGIN_H

// Local Variables:
// mode:C++
// End:

//  clips.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <set>

#include <fcntl.h>  // O_RDONLY

#include "backend/clip.h"
#include "backend/threadbookkeeping.h"
#include "backend/preferences.h"
#include "gnome-frontend/dialogs.h"
#include "backend/applicationerrors.h"
#include "backend/audioplanet.h"
#include "backend/filter.h"
#include "backend/base/process.h"
#include "backend/base/debug.h"
#include "backend/base/log.h"

#include "boost/lexical_cast.hpp"


namespace frontend
{

    static std::set<IClip *> clipList;


    //
    // initialize, cleanup
    //


    static void cleanup() throw()
    {
	try
	{
	    trace();
	    closeAll();
	}
	catch (Exception & e)
	{
	    root.error(e.what());
	}
    }


    void initialize() EXCEPTIONS
    {
	trace();
	if (std::atexit(cleanup) != 0) throw AtexitException(LOCATION);
    }


    //
    // add, remove
    //


    void add(IClip * clip) EXCEPTIONS
    {
	trace();
	clipList.insert(clip);
    }


    void remove(IClip * clip) EXCEPTIONS
    {
	clipList.erase(clip);
    }


    //
    // save, saveAs
    //


    void save(IClip * clip) EXCEPTIONS
    {
	trace();
	
	
	std::string fileName = clip->fileName();
	if (fileName == "") saveAs(clip);
    }


    void saveAs(IClip * clip) EXCEPTIONS
    {
	trace();
	
	
	//save_as_dialog dialog(this);
	//dialog.show();
    }


    //
    // open
    //


    void open(const std::string & fileName) EXCEPTIONS
    {
	root.debug("OPEN");
	root.debug(static_cast<std::string>("File name: ") + fileName);
	
	
	std::string fileType = gnome_mime_type_or_default_of_file(fileName.c_str(), "");
	if (fileType == "")
	{
	    root.error("Unknown file format");
	    return;
	}
	root.debug(static_cast<std::string>("MIME type: ") + fileType);
	if (!AP::openHandlerRegistered(fileType))
	{
	    root.error("The file format is not supported by any open handler");
	    return;
	}
	
	
	::File childStdin(fileName, O_RDONLY);
	::File parentReadFile;
	Process::Id pid = Process::createWithOutputPipe(childStdin, parentReadFile);
	childStdin.close();  // childStdin is only needed by createPiped as source for standard input, so it's safe to close it now
	if (pid != 0)
	{
	    // In the parent's thread of execution
		
	    AP::AudioFormat audioFormat;
	    AP::readFormat(parentReadFile, audioFormat);
		
	    IClip * clip;
	    if (audioFormat.domain() == AP::AudioFormat::TIME_DOMAIN)
	    {
		switch (audioFormat.sampleFormat())
		{
		case AP::AudioFormat::INT_16:
		    clip = new TdClip<Int16>(fileName, fileName);
		    break;
		case AP::AudioFormat::INT_32:
		    clip = new TdClip<Int32>(fileName, fileName);
		    break;
		case AP::AudioFormat::FLOAT_32:
		    clip = new TdClip<Float32>(fileName, fileName);
		    break;
		case AP::AudioFormat::FLOAT_64:
		    clip = new TdClip<Float64>(fileName, fileName);
		    break;
		default:
		    throw FlowOfControlException(LOCATION);
		}
	    }
	    else
		throw InvalidFormat(LOCATION);  // NOT IMPLEMENTED
		
	    // consumer will close parentReadFile
	    clip->consumer().setInputDescriptor(parentReadFile);
	    clip->consumer().start();
	}
	else
	{
	    // In the child's thread of execution
		
	    // Let the plug-in read the audio format from the file header
	    AP::AudioFormat audioFormat;
	    AP::getFormat(fileType, audioFormat);
		
	    ::File stdoutFile(STDOUT_FILENO);
	    AP::sendFormat(stdoutFile, audioFormat);
		
	    AP::FilterHandle openHandle = AP::findOpenHandler(fileType);
	    AP::Filter * openFilter = AP::createFilterInstance(openHandle, audioFormat);
	    openFilter->initialize();
	    unsigned int numChannels = openFilter->maxOutputPins();
		
	    root.debug("Number of channels: " +
		       boost::lexical_cast<std::string>(numChannels));
		
	    openFilter->setNumInputPins(0);
	    openFilter->setNumOutputPins(numChannels);
		
	    openFilter->start();
	    openFilter->join();

	    root.debug("Finished reading " + fileName);
		
	    delete openFilter;
	    Process::terminate(0);  // This will close child's file descriptors
	}
    }


    //
    // close, closeAll
    //


    // Prompt for saving changes and close the clip.
    // Return 0 if succesful, nonzero otherwise
    int close(IClip * clip) EXCEPTIONS
    {
	root.debug("CLOSE");
	root.debug(static_cast<std::string>("File name: ") + clip->fileName());
	
	
	if (clip->modified())
	{
	    std::string message;
	    std::string fileName = clip->fileName();
	    if (fileName != "") message = "Save changes to file " + fileName +
				    " before closing?";
	    else message = "Save audio clip to a file before closing?";
		
	    using namespace GUI;
	    switch (messageDialog(message, mtConfirmation, mbYes | mbNo | mbCancel, 0))
	    {
	    case mrYes:
	    case mrNo:
		delete clip;
		return 0;
	    default:
		throw FlowOfControlException(LOCATION);
	    }
	}
	else
	{
	    delete clip;
	    return 0;
	}
	
	return 1;
    }


    // Close all the clips.
    // Return 0 if succesful, nonzero otherwise
    int closeAll() EXCEPTIONS
    {
	trace();
	
	
	while (clipList.size() > 0)
	{
	    int result = close(*(clipList.begin()));
	    if (result != 0) return result;
	}
	
	return 0;
    }


    class ClipResourceAcquisition
    {
    public:

	ClipResourceAcquisition()
	{
	}
		
	~ClipResourceAcquisition()
	{
	    closeAll();
	}
    };


    static ClipResourceAcquisition clipResourceAcquisition;

}

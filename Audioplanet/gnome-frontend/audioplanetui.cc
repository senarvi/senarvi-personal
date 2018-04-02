#include "audioplanetui.h"
#include "audiowindow.h"


namespace frontend
{
    
	static AudioplanetUI * theInstance = 0;


    AudioplanetUI::AudioplanetUI(int & argc,
        char ** & argv,
        const std::string & rcFileName) :
        
        Gtkmmext::UI(std::string("audioplanet"), &argc, &argv, rcFileName.c_str())
        
    {
        starting.connect(slot(*this, &AudioplanetUI::startupHandler));
        stopping.connect(slot(*this, &AudioplanetUI::shutdownHandler));

		new AudioWindow();
		new AudioWindow();
    }


	AudioplanetUI & AudioplanetUI::create(int & argc,
						   char ** & argv,
						   const std::string & rcFileName)
	{
		if (theInstance == 0)
			theInstance = new AudioplanetUI(argc, argv, rcFileName);
		return *theInstance;
	}


	AudioplanetUI & AudioplanetUI::instance()
	{
/*		if (theInstance == 0)
			throw NotInstantiatedException();*/
		return *theInstance;
	}


    void AudioplanetUI::startupHandler()
    {
        std::cout << "Starting up GUI." << std::endl;
    }
    
    
    void AudioplanetUI::shutdownHandler()
    {
        std::cout << "GUI shut down." << std::endl;
    }

}


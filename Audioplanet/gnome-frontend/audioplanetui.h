#ifndef AUDIOPLANETUI_H
#define AUDIOPLANETUI_H


#include <vector>
#include <string>

#include <gtkmmext/gtk_ui.h>
#include <boost/utility.hpp>


namespace frontend
{

    class AudioplanetUI : public Gtkmmext::UI, private boost::noncopyable
    {
    public:
		static AudioplanetUI & create(int & argc,
									  char ** & argv,
									  const std::string & rcFileName);
        static AudioplanetUI & instance();
        
        
    private:
        AudioplanetUI(int & argc,
                char ** & argv,
                const std::string & rcFileName);

        // Will be called just before we enter the main loop.
        void startupHandler();
        // Will be called just after we return from the main loop.
        void shutdownHandler();
    };
    
}

#endif  // AUDIOPLANETUI_H

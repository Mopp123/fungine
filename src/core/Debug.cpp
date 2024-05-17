
#include <stdio.h>
#include "Debug.h"

namespace fungine
{
	void Debug::log(const std::string& message, int errorLevel)
	{
		switch (errorLevel)
		{
		case DEBUG__ERROR_LEVEL__NONE:														break;
		case DEBUG__ERROR_LEVEL__WARNING:		printf("\nDebug: /_\\ Warning!\n");			break;
		case DEBUG__ERROR_LEVEL__ERROR:			printf("\nDebug: (x) Error!\n");			break;
		case DEBUG__ERROR_LEVEL__FATAL_ERROR:	printf("\nDebug: (!X!) Fatal error!\n");	break;
		default:
			break;
		}

		printf("%s\n", message.c_str());
	}

	void Debug::notify_on_destroy(const std::string& objName)
	{
#ifdef DEBUG__MODE_FULL
		log(objName + " destroyed!");
#endif
	}
}
